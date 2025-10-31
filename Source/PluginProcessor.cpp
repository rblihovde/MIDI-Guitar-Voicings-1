#include "PluginProcessor.h"
#include "PluginEditor.h"

MIDIGuitarVoicingsProcessor::MIDIGuitarVoicingsProcessor()
    : AudioProcessor(BusesProperties()
                     .withInput("Input", juce::AudioChannelSet::stereo(), true)
                     .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      m_parameters(*this, nullptr, "Parameters",
      {
          std::make_unique<juce::AudioParameterBool>(
              "enabled",
              "Enabled",
              true),
          
          std::make_unique<juce::AudioParameterChoice>(
              "pattern",
              "Pattern Type",
              juce::StringArray{"Strum", "Fingerpick", "Arpeggio", "Tremolo"},
              0),
          
          std::make_unique<juce::AudioParameterFloat>(
              "humanization",
              "Humanization",
              juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f),
              0.3f),
          
          std::make_unique<juce::AudioParameterFloat>(
              "tempo",
              "Tempo",
              juce::NormalisableRange<float>(40.0f, 240.0f, 0.1f),
              120.0f)
      })
{
    m_enabledParam = m_parameters.getRawParameterValue("enabled");
    m_patternTypeParam = m_parameters.getRawParameterValue("pattern");
    m_humanizationParam = m_parameters.getRawParameterValue("humanization");
    m_tempoParam = m_parameters.getRawParameterValue("tempo");
}

MIDIGuitarVoicingsProcessor::~MIDIGuitarVoicingsProcessor()
{
}

const juce::String MIDIGuitarVoicingsProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MIDIGuitarVoicingsProcessor::acceptsMidi() const
{
    return true;
}

bool MIDIGuitarVoicingsProcessor::producesMidi() const
{
    return true;
}

bool MIDIGuitarVoicingsProcessor::isMidiEffect() const
{
    return true;
}

double MIDIGuitarVoicingsProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MIDIGuitarVoicingsProcessor::getNumPrograms()
{
    return 1;
}

int MIDIGuitarVoicingsProcessor::getCurrentProgram()
{
    return 0;
}

void MIDIGuitarVoicingsProcessor::setCurrentProgram(int index)
{
    juce::ignoreUnused(index);
}

const juce::String MIDIGuitarVoicingsProcessor::getProgramName(int index)
{
    juce::ignoreUnused(index);
    return {};
}

void MIDIGuitarVoicingsProcessor::changeProgramName(int index, const juce::String& newName)
{
    juce::ignoreUnused(index, newName);
}

void MIDIGuitarVoicingsProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    juce::ignoreUnused(sampleRate, samplesPerBlock);
    m_scheduledNotes.clear();
}

void MIDIGuitarVoicingsProcessor::releaseResources()
{
}

bool MIDIGuitarVoicingsProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    // Support any layout since we're primarily a MIDI effect
    return true;
}

void MIDIGuitarVoicingsProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    
    // Clear audio buffer (we're a MIDI effect)
    buffer.clear();
    
    if (!isEnabled())
    {
        // Pass MIDI through unchanged
        return;
    }
    
    // Update pattern generator settings
    m_patternGenerator.setHumanization(getHumanization());
    
    // Process incoming MIDI
    processInputMidi(midiMessages, buffer.getNumSamples());
}

void MIDIGuitarVoicingsProcessor::processInputMidi(juce::MidiBuffer& midiMessages, int numSamples)
{
    juce::MidiBuffer processedMidi;
    std::vector<int> currentNotes;
    
    // Collect all note-on messages in this block
    for (const auto metadata : midiMessages)
    {
        auto message = metadata.getMessage();
        
        if (message.isNoteOn())
        {
            currentNotes.push_back(message.getNoteNumber());
        }
        else if (message.isNoteOff())
        {
            // Remove from current notes
            auto it = std::find(currentNotes.begin(), currentNotes.end(), message.getNoteNumber());
            if (it != currentNotes.end())
            {
                currentNotes.erase(it);
            }
        }
    }
    
    // If we have new notes, generate a voicing and pattern
    if (!currentNotes.empty() && currentNotes != m_currentChord)
    {
        m_currentChord = currentNotes;
        
        // Get best voicing
        auto voicing = m_voicingEngine.getBestVoicing(m_currentChord, m_lastPosition);
        
        if (!voicing.notes.empty())
        {
            m_lastPosition = voicing.position;
            
            // Generate pattern
            auto pattern = m_patternGenerator.generatePattern(
                voicing,
                getCurrentPattern(),
                getTempo());
            
            // Schedule the pattern notes
            for (const auto& event : pattern)
            {
                int sampleOffset = static_cast<int>(event.timeOffset * getSampleRate());
                int noteDuration = static_cast<int>(event.duration * getSampleRate());
                
                if (sampleOffset < numSamples)
                {
                    // Add note on
                    juce::uint8 velocity = static_cast<juce::uint8>(event.velocity * 127);
                    processedMidi.addEvent(
                        juce::MidiMessage::noteOn(1, event.midiNote, velocity),
                        sampleOffset);
                    
                    // Schedule note off
                    if (sampleOffset + noteDuration < numSamples)
                    {
                        processedMidi.addEvent(
                            juce::MidiMessage::noteOff(1, event.midiNote),
                            sampleOffset + noteDuration);
                    }
                }
            }
        }
    }
    
    // Replace the MIDI buffer with our processed version
    midiMessages.swapWith(processedMidi);
}

void MIDIGuitarVoicingsProcessor::generateAndSchedulePattern(const std::vector<int>& midiNotes, int startSample)
{
    juce::ignoreUnused(midiNotes, startSample);
    // This could be used for more advanced scheduling in the future
}

bool MIDIGuitarVoicingsProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* MIDIGuitarVoicingsProcessor::createEditor()
{
    return new MIDIGuitarVoicingsEditor(*this);
}

void MIDIGuitarVoicingsProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = m_parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void MIDIGuitarVoicingsProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    
    if (xmlState != nullptr)
    {
        if (xmlState->hasTagName(m_parameters.state.getType()))
        {
            m_parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
        }
    }
}

PatternGenerator::PatternType MIDIGuitarVoicingsProcessor::getCurrentPattern() const
{
    int patternIndex = static_cast<int>(*m_patternTypeParam);
    return static_cast<PatternGenerator::PatternType>(patternIndex);
}

float MIDIGuitarVoicingsProcessor::getHumanization() const
{
    return *m_humanizationParam;
}

float MIDIGuitarVoicingsProcessor::getTempo() const
{
    return *m_tempoParam;
}

bool MIDIGuitarVoicingsProcessor::isEnabled() const
{
    return *m_enabledParam > 0.5f;
}

// This creates new instances of the plugin
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MIDIGuitarVoicingsProcessor();
}
