#pragma once

#include <juce_core/juce_core.h>
#include "GuitarVoicingEngine.h"
#include <vector>

/**
 * @brief Generates realistic guitar picking patterns
 * 
 * This class creates various playing styles including strumming,
 * fingerpicking, and arpeggios with natural timing and velocity variations.
 */
class PatternGenerator
{
public:
    enum class PatternType
    {
        Strum,
        Fingerpick,
        Arpeggio,
        Tremolo
    };
    
    struct NoteEvent
    {
        int midiNote;
        double timeOffset;      // Seconds from pattern start
        float velocity;         // 0.0-1.0
        double duration;        // Seconds
    };
    
    PatternGenerator();
    
    /**
     * @brief Generate a pattern from a voicing
     * @param voicing The guitar voicing to play
     * @param pattern Type of pattern to generate
     * @param tempo BPM for timing calculations
     * @return Vector of note events with timing
     */
    std::vector<NoteEvent> generatePattern(
        const GuitarVoicingEngine::Voicing& voicing,
        PatternType pattern,
        double tempo = 120.0);
    
    /**
     * @brief Set the humanization amount (timing and velocity variations)
     * @param amount 0.0 = perfect timing, 1.0 = very human
     */
    void setHumanization(float amount) { m_humanization = juce::jlimit(0.0f, 1.0f, amount); }
    
    /**
     * @brief Set the strum direction
     */
    void setStrumDirection(bool downstroke) { m_downstroke = downstroke; }
    
    /**
     * @brief Set the accent pattern for velocity variations
     */
    void setAccentPattern(const std::vector<bool>& pattern) { m_accentPattern = pattern; }
    
private:
    float m_humanization = 0.3f;
    bool m_downstroke = true;
    std::vector<bool> m_accentPattern;
    juce::Random m_random;
    
    std::vector<NoteEvent> generateStrum(const GuitarVoicingEngine::Voicing& voicing, double tempo);
    std::vector<NoteEvent> generateFingerpick(const GuitarVoicingEngine::Voicing& voicing, double tempo);
    std::vector<NoteEvent> generateArpeggio(const GuitarVoicingEngine::Voicing& voicing, double tempo);
    std::vector<NoteEvent> generateTremolo(const GuitarVoicingEngine::Voicing& voicing, double tempo);
    
    float getHumanizedVelocity(float baseVelocity);
    double getHumanizedTiming(double baseTiming);
};
