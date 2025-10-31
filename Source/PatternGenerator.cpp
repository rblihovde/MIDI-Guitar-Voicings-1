#include "PatternGenerator.h"
#include <algorithm>

PatternGenerator::PatternGenerator()
{
    m_random.setSeedRandomly();
}

float PatternGenerator::getHumanizedVelocity(float baseVelocity)
{
    if (m_humanization < 0.01f)
        return baseVelocity;
    
    float variation = m_humanization * 0.2f;
    float randomOffset = (m_random.nextFloat() - 0.5f) * variation;
    
    return juce::jlimit(0.0f, 1.0f, baseVelocity + randomOffset);
}

double PatternGenerator::getHumanizedTiming(double baseTiming)
{
    if (m_humanization < 0.01f)
        return baseTiming;
    
    double variation = m_humanization * 0.02; // Up to 20ms at max humanization
    double randomOffset = (m_random.nextDouble() - 0.5) * variation;
    
    return baseTiming + randomOffset;
}

std::vector<PatternGenerator::NoteEvent> PatternGenerator::generatePattern(
    const GuitarVoicingEngine::Voicing& voicing,
    PatternType pattern,
    double tempo)
{
    switch (pattern)
    {
        case PatternType::Strum:
            return generateStrum(voicing, tempo);
        case PatternType::Fingerpick:
            return generateFingerpick(voicing, tempo);
        case PatternType::Arpeggio:
            return generateArpeggio(voicing, tempo);
        case PatternType::Tremolo:
            return generateTremolo(voicing, tempo);
        default:
            return generateStrum(voicing, tempo);
    }
}

std::vector<PatternGenerator::NoteEvent> PatternGenerator::generateStrum(
    const GuitarVoicingEngine::Voicing& voicing,
    double tempo)
{
    std::vector<NoteEvent> events;
    
    if (voicing.notes.empty())
        return events;
    
    // Sort notes by string (for strum direction)
    std::vector<GuitarVoicingEngine::GuitarNote> sortedNotes = voicing.notes;
    std::sort(sortedNotes.begin(), sortedNotes.end(),
        [this](const auto& a, const auto& b) {
            return m_downstroke ? (a.stringIndex > b.stringIndex) : (a.stringIndex < b.stringIndex);
        });
    
    // Calculate strum timing
    double secondsPerBeat = 60.0 / tempo;
    double strumDuration = 0.02 + (m_humanization * 0.03); // 20-50ms depending on humanization
    double timePerString = strumDuration / sortedNotes.size();
    
    for (size_t i = 0; i < sortedNotes.size(); ++i)
    {
        NoteEvent event;
        event.midiNote = sortedNotes[i].midiNote;
        event.timeOffset = getHumanizedTiming(i * timePerString);
        
        // Velocity variation - slightly louder on accented strings
        float baseVelocity = 0.7f + (i == 0 ? 0.1f : 0.0f);
        event.velocity = getHumanizedVelocity(baseVelocity);
        
        // Note duration
        event.duration = secondsPerBeat * 0.5; // Eighth note length
        
        events.push_back(event);
    }
    
    return events;
}

std::vector<PatternGenerator::NoteEvent> PatternGenerator::generateFingerpick(
    const GuitarVoicingEngine::Voicing& voicing,
    double tempo)
{
    std::vector<NoteEvent> events;
    
    if (voicing.notes.empty())
        return events;
    
    double secondsPerBeat = 60.0 / tempo;
    
    // Fingerpicking pattern: bass note first, then higher strings
    std::vector<GuitarVoicingEngine::GuitarNote> sortedNotes = voicing.notes;
    std::sort(sortedNotes.begin(), sortedNotes.end(),
        [](const auto& a, const auto& b) {
            return a.stringIndex > b.stringIndex; // Low to high
        });
    
    // Bass note on beat 1
    if (!sortedNotes.empty())
    {
        NoteEvent bassEvent;
        bassEvent.midiNote = sortedNotes[0].midiNote;
        bassEvent.timeOffset = getHumanizedTiming(0.0);
        bassEvent.velocity = getHumanizedVelocity(0.75f);
        bassEvent.duration = secondsPerBeat * 0.75;
        events.push_back(bassEvent);
    }
    
    // Higher strings spread across the beat
    for (size_t i = 1; i < sortedNotes.size(); ++i)
    {
        NoteEvent event;
        event.midiNote = sortedNotes[i].midiNote;
        
        double beatOffset = 0.25 + (i - 1) * 0.25;
        event.timeOffset = getHumanizedTiming(secondsPerBeat * beatOffset);
        event.velocity = getHumanizedVelocity(0.65f);
        event.duration = secondsPerBeat * 0.5;
        
        events.push_back(event);
    }
    
    return events;
}

std::vector<PatternGenerator::NoteEvent> PatternGenerator::generateArpeggio(
    const GuitarVoicingEngine::Voicing& voicing,
    double tempo)
{
    std::vector<NoteEvent> events;
    
    if (voicing.notes.empty())
        return events;
    
    // Sort notes by pitch (low to high)
    std::vector<GuitarVoicingEngine::GuitarNote> sortedNotes = voicing.notes;
    std::sort(sortedNotes.begin(), sortedNotes.end(),
        [](const auto& a, const auto& b) {
            return a.midiNote < b.midiNote;
        });
    
    double secondsPerBeat = 60.0 / tempo;
    double timePerNote = (secondsPerBeat * 4.0) / sortedNotes.size(); // Spread over 4 beats
    
    for (size_t i = 0; i < sortedNotes.size(); ++i)
    {
        NoteEvent event;
        event.midiNote = sortedNotes[i].midiNote;
        event.timeOffset = getHumanizedTiming(i * timePerNote);
        event.velocity = getHumanizedVelocity(0.7f);
        event.duration = timePerNote * 1.5; // Slight overlap
        
        events.push_back(event);
    }
    
    return events;
}

std::vector<PatternGenerator::NoteEvent> PatternGenerator::generateTremolo(
    const GuitarVoicingEngine::Voicing& voicing,
    double tempo)
{
    std::vector<NoteEvent> events;
    
    if (voicing.notes.empty())
        return events;
    
    double secondsPerBeat = 60.0 / tempo;
    double noteLength = secondsPerBeat / 8.0; // 32nd notes
    
    // Rapid repetition of all notes (tremolo picking)
    for (int repeat = 0; repeat < 8; ++repeat)
    {
        for (const auto& note : voicing.notes)
        {
            NoteEvent event;
            event.midiNote = note.midiNote;
            event.timeOffset = getHumanizedTiming(repeat * noteLength);
            
            // Slight velocity variation for realism
            float baseVel = 0.65f + (repeat % 2 == 0 ? 0.05f : 0.0f);
            event.velocity = getHumanizedVelocity(baseVel);
            event.duration = noteLength * 0.9;
            
            events.push_back(event);
        }
    }
    
    return events;
}
