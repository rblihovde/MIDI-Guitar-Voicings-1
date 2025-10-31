#include "GuitarVoicingEngine.h"
#include <algorithm>
#include <cmath>

GuitarVoicingEngine::GuitarVoicingEngine()
{
    // Standard guitar tuning: E2, A2, D3, G3, B3, E4
    m_tuning = {40, 45, 50, 55, 59, 64}; // MIDI note numbers
}

void GuitarVoicingEngine::setTuning(const std::array<int, NUM_STRINGS>& tuning)
{
    m_tuning = tuning;
}

int GuitarVoicingEngine::getMidiNote(int stringIndex, int fret) const
{
    if (stringIndex < 0 || stringIndex >= NUM_STRINGS)
        return -1;
    
    return m_tuning[stringIndex] + fret;
}

std::vector<GuitarVoicingEngine::GuitarNote> GuitarVoicingEngine::findPositions(int midiNote) const
{
    std::vector<GuitarNote> positions;
    
    for (int string = 0; string < NUM_STRINGS; ++string)
    {
        int fret = midiNote - m_tuning[string];
        
        if (fret >= 0 && fret <= NUM_FRETS)
        {
            positions.push_back({string, fret, midiNote, 0.8f});
        }
    }
    
    return positions;
}

float GuitarVoicingEngine::calculatePlayability(const Voicing& voicing) const
{
    if (voicing.notes.empty())
        return 0.0f;
    
    // Find min and max fret positions (excluding open strings)
    int minFret = NUM_FRETS + 1;
    int maxFret = -1;
    
    for (const auto& note : voicing.notes)
    {
        if (note.fretNumber > 0)
        {
            minFret = std::min(minFret, note.fretNumber);
            maxFret = std::max(maxFret, note.fretNumber);
        }
    }
    
    // If all open strings, highly playable
    if (maxFret < 0)
        return 1.0f;
    
    // Calculate fret span
    int fretSpan = maxFret - minFret;
    
    // Penalize large spans
    float spanScore = 1.0f - (float)fretSpan / (m_maxFretSpan + 2);
    spanScore = std::max(0.0f, spanScore);
    
    // Check for string skipping (more realistic)
    int usedStrings = 0;
    std::array<bool, NUM_STRINGS> stringUsed = {false};
    
    for (const auto& note : voicing.notes)
    {
        if (!stringUsed[note.stringIndex])
        {
            stringUsed[note.stringIndex] = true;
            usedStrings++;
        }
    }
    
    // Prefer adjacent strings
    float stringScore = (float)usedStrings / voicing.notes.size();
    
    // Penalize very high positions (harder to play)
    float positionScore = 1.0f - (float)minFret / NUM_FRETS;
    positionScore = std::max(0.3f, positionScore);
    
    return (spanScore * 0.5f + stringScore * 0.3f + positionScore * 0.2f);
}

std::vector<GuitarVoicingEngine::Voicing> GuitarVoicingEngine::generateVoicings(const std::vector<int>& midiNotes)
{
    std::vector<Voicing> voicings;
    
    if (midiNotes.empty())
        return voicings;
    
    // For each possible position on the fretboard
    for (int position = 0; position <= NUM_FRETS - m_maxFretSpan; ++position)
    {
        Voicing voicing;
        voicing.position = position;
        
        // Try to voice all notes within this position
        for (int midiNote : midiNotes)
        {
            std::vector<GuitarNote> positions = findPositions(midiNote);
            
            // Find best string for this note in this position
            GuitarNote* bestNote = nullptr;
            float bestScore = -1.0f;
            
            for (auto& pos : positions)
            {
                // Check if this fret is within reach of current position
                if (pos.fretNumber == 0 || 
                    (pos.fretNumber >= position && pos.fretNumber <= position + m_maxFretSpan))
                {
                    // Check if string is already used
                    bool stringTaken = false;
                    for (const auto& existing : voicing.notes)
                    {
                        if (existing.stringIndex == pos.stringIndex)
                        {
                            stringTaken = true;
                            break;
                        }
                    }
                    
                    if (!stringTaken)
                    {
                        // Prefer lower strings for lower notes
                        float score = 1.0f - std::abs(pos.stringIndex - (5 - (midiNote - 40) / 12)) / 6.0f;
                        
                        if (score > bestScore)
                        {
                            bestScore = score;
                            bestNote = &pos;
                        }
                    }
                }
            }
            
            if (bestNote != nullptr)
            {
                voicing.notes.push_back(*bestNote);
            }
        }
        
        // Only add voicings that voice all notes
        if (voicing.notes.size() == midiNotes.size())
        {
            voicing.difficulty = calculatePlayability(voicing);
            voicings.push_back(voicing);
        }
    }
    
    // Sort by playability (higher is better)
    std::sort(voicings.begin(), voicings.end(), 
        [](const Voicing& a, const Voicing& b) {
            return a.difficulty > b.difficulty;
        });
    
    return voicings;
}

GuitarVoicingEngine::Voicing GuitarVoicingEngine::getBestVoicing(const std::vector<int>& midiNotes, int lastPosition)
{
    auto voicings = generateVoicings(midiNotes);
    
    if (voicings.empty())
    {
        // Return empty voicing
        return Voicing();
    }
    
    // If we have a last position, prefer voicings close to it (smooth voice leading)
    if (lastPosition > 0 && voicings.size() > 1)
    {
        std::sort(voicings.begin(), voicings.end(),
            [lastPosition](const Voicing& a, const Voicing& b) {
                float distA = std::abs(a.position - lastPosition);
                float distB = std::abs(b.position - lastPosition);
                float scoreA = a.difficulty - distA * 0.1f;
                float scoreB = b.difficulty - distB * 0.1f;
                return scoreA > scoreB;
            });
    }
    
    return voicings[0];
}
