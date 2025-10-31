#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "GuitarVoicingEngine.h"
#include "PatternGenerator.h"

/**
 * @brief Main audio processor for the MIDI Guitar Voicings plugin
 */
class MIDIGuitarVoicingsProcessor : public juce::AudioProcessor
{
public:
    MIDIGuitarVoicingsProcessor();
    ~MIDIGuitarVoicingsProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return JucePlugin_Name; }

    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return true; }
    bool isMidiEffect() const override { return true; }
    double getTailLengthSeconds() const override { return 0.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int index) override { juce::ignoreUnused(index); }
    const juce::String getProgramName(int index) override { juce::ignoreUnused(index); return {}; }
    void changeProgramName(int index, const juce::String& newName) override { juce::ignoreUnused(index, newName); }

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    // Parameter access
    juce::AudioProcessorValueTreeState& getParameters() { return m_parameters; }
    
    // Current state getters
    PatternGenerator::PatternType getCurrentPattern() const;
    float getHumanization() const;
    float getTempo() const;
    bool isEnabled() const;

private:
    juce::AudioProcessorValueTreeState m_parameters;
    
    GuitarVoicingEngine m_voicingEngine;
    PatternGenerator m_patternGenerator;
    
    std::atomic<float>* m_enabledParam = nullptr;
    std::atomic<float>* m_patternTypeParam = nullptr;
    std::atomic<float>* m_humanizationParam = nullptr;
    std::atomic<float>* m_tempoParam = nullptr;
    
    // For tracking chord changes
    std::vector<int> m_currentChord;
    int m_lastPosition = 0;
    
    // For pattern generation timing
    struct ScheduledNote
    {
        int midiNote;
        int samplePosition;
        float velocity;
        int duration;
    };
    
    std::vector<ScheduledNote> m_scheduledNotes;
    
    void processInputMidi(juce::MidiBuffer& midiMessages, int numSamples);
    void generateAndSchedulePattern(const std::vector<int>& midiNotes, int startSample);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MIDIGuitarVoicingsProcessor)
};
