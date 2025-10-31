#pragma once

#include <JuceHeader.h>
#include <vector>
#include <array>

/**
 * @brief Engine for calculating realistic guitar voicings
 * 
 * This class handles the logic for determining how chords should be
 * voiced on a guitar, taking into account string tuning, fret positions,
 * and playability constraints.
 */
class GuitarVoicingEngine
{
public:
    GuitarVoicingEngine();
    
    // Guitar string configuration (standard tuning: E2, A2, D3, G3, B3, E4)
    static constexpr int NUM_STRINGS = 6;
    static constexpr int NUM_FRETS = 24;
    
    struct GuitarNote
    {
        int stringIndex;    // 0-5 (0 = lowest E string)
        int fretNumber;     // 0-24
        int midiNote;       // MIDI note number
        float velocity;     // 0.0-1.0
    };
    
    struct Voicing
    {
        std::vector<GuitarNote> notes;
        float difficulty;   // 0.0-1.0 (playability rating)
        int position;       // Fret position of the chord
    };
    
    /**
     * @brief Generate guitar voicings for a set of MIDI notes
     * @param midiNotes Input MIDI note numbers
     * @return Vector of possible voicings, sorted by playability
     */
    std::vector<Voicing> generateVoicings(const std::vector<int>& midiNotes);
    
    /**
     * @brief Find the best voicing based on previous position
     * @param midiNotes Input MIDI notes
     * @param lastPosition Previous fret position for smooth voice leading
     * @return Best voicing
     */
    Voicing getBestVoicing(const std::vector<int>& midiNotes, int lastPosition = 0);
    
    /**
     * @brief Set the tuning for each string
     * @param tuning Array of MIDI note numbers for each string
     */
    void setTuning(const std::array<int, NUM_STRINGS>& tuning);
    
    /**
     * @brief Get the current tuning
     */
    const std::array<int, NUM_STRINGS>& getTuning() const { return m_tuning; }
    
    /**
     * @brief Set the maximum fret span (hand stretch limit)
     */
    void setMaxFretSpan(int span) { m_maxFretSpan = span; }
    
private:
    std::array<int, NUM_STRINGS> m_tuning;
    int m_maxFretSpan = 4; // Maximum fret span for one hand position
    
    /**
     * @brief Calculate if a voicing is physically playable
     */
    float calculatePlayability(const Voicing& voicing) const;
    
    /**
     * @brief Get the MIDI note for a string and fret combination
     */
    int getMidiNote(int stringIndex, int fret) const;
    
    /**
     * @brief Find all possible positions for a given MIDI note on the fretboard
     */
    std::vector<GuitarNote> findPositions(int midiNote) const;
};
