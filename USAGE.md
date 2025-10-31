# MIDI Guitar Voicings - Usage Guide

## Overview

MIDI Guitar Voicings is a MIDI effect plugin that transforms simple MIDI chord input into realistic guitar voicings and playing patterns. It's perfect for producers and composers who want authentic guitar MIDI without manually programming every note position.

## Getting Started

### Installation

1. Build the plugin following the instructions in README.md
2. Copy the plugin to your DAW's plugin folder:
   - **VST3**: Place in your VST3 folder
   - **AU** (macOS): Place in `~/Library/Audio/Plug-Ins/Components/`
   - **Standalone**: Run directly from the build folder

### Basic Workflow

1. **Load the Plugin** as a MIDI effect in your DAW
2. **Create a MIDI track** and route it through the plugin
3. **Play or program chords** - the plugin will automatically voice them for guitar
4. **Choose a pattern** to apply realistic playing styles

## User Interface

![MIDI Guitar Voicings UI](https://github.com/user-attachments/assets/db69413d-e5ed-45a1-b271-1238a9cf3f64)

### Controls

#### Enable Toggle
- Turn the plugin processing on/off
- When disabled, MIDI passes through unchanged

#### Pattern Selector
Choose from 4 realistic guitar playing styles:

- **Strum**: Traditional strumming with slight string-to-string timing
  - Simulates downstroke/upstroke picking
  - Natural string-to-string timing offset
  - Velocity variation per string

- **Fingerpick**: Bass note followed by higher strings
  - Bass note on the beat
  - Treble strings spread across the beat
  - Perfect for folk and acoustic styles

- **Arpeggio**: Notes played sequentially from low to high
  - Smooth note progression
  - Adjustable timing based on tempo
  - Great for ambient and melodic passages

- **Tremolo**: Rapid repetition of all notes
  - Fast 32nd note patterns
  - Subtle velocity variations
  - Ideal for intensity and tension

#### Humanization Knob (0-100%)
Controls the amount of natural variation:
- **0%**: Perfect, mechanical timing and velocity
- **30%**: Subtle variations (recommended starting point)
- **50%**: Moderate human feel
- **100%**: Maximum variation (may sound loose)

Affects:
- Note timing (up to 20ms variation)
- Velocity differences between notes
- Creates a more organic, performed feel

#### Tempo Knob (40-240 BPM)
Sets the timing reference for patterns:
- Used to calculate note durations
- Affects pattern spacing
- Syncs with your project tempo for best results

## Usage Examples

### Example 1: Acoustic Strumming

1. Set **Pattern** to "Strum"
2. Set **Humanization** to 30-40%
3. Set **Tempo** to match your project
4. Input simple triads (e.g., C-E-G)
5. Result: Natural strummed chords with slight timing variations

### Example 2: Fingerstyle Folk

1. Set **Pattern** to "Fingerpick"
2. Set **Humanization** to 40-50%
3. Play chord progressions slowly
4. Result: Bass notes on the beat with melodic treble strings

### Example 3: Ambient Arpeggios

1. Set **Pattern** to "Arpeggio"
2. Set **Humanization** to 20-30%
3. Use slower tempo or longer note values
4. Result: Flowing, harp-like guitar patterns

### Example 4: Tremolo Picking

1. Set **Pattern** to "Tremolo"
2. Set **Humanization** to 25-35%
3. Use for building tension or intensity
4. Result: Rapid-fire repetitive picking

## Tips & Best Practices

### Input MIDI
- **Use simple chords**: 3-4 notes work best
- **Avoid extreme ranges**: Stay within guitar range (E2-E5)
- **Don't overplay**: Let the plugin add the detail
- **Use sustained notes**: The plugin handles the rhythm

### Voicing Behavior
- The plugin calculates playable guitar fingerings
- It respects fret span limitations (max 4-fret stretch)
- Smooth voice leading between chord changes
- Lower notes prefer lower strings

### Pattern Selection
- **Strum**: Use for rhythm guitar, chords, comping
- **Fingerpick**: Use for ballads, singer-songwriter styles
- **Arpeggio**: Use for pads, ambient textures, intros
- **Tremolo**: Use for intensity, leads, mandolin-style parts

### Humanization Guidelines
- **Tight rhythm sections**: Use 10-20%
- **Natural performances**: Use 30-40%
- **Loose, organic feel**: Use 50-70%
- **Experimental/drunk**: Use 80-100%

### Combining with Other Plugins
1. **Before the plugin**: Use MIDI chord generators, scale plugins
2. **After the plugin**: Use guitar amp sims, reverb, compression
3. **Route to guitar VST**: Connect output to any guitar sample library

## DAW-Specific Tips

### Logic Pro
- Insert as MIDI FX on a software instrument track
- Use with Alchemy's guitar patches
- Automate parameters for dynamic performances

### Ableton Live
- Place before any instrument
- Map parameters to MIDI controllers
- Use with Collision or Tension for physical modeling

### FL Studio
- Load in the MIDI Out slot
- Route to any VST instrument
- Use with Slayer or other guitar synths

## Troubleshooting

### No Output
- Check that **Enable** is turned on
- Verify MIDI is reaching the plugin
- Ensure output is routed to an instrument

### Sounds Too Mechanical
- Increase **Humanization** to 30-40%
- Try different patterns
- Vary your input timing slightly

### Timing Issues
- Match **Tempo** to your project BPM
- Check if DAW tempo is synced
- Adjust Humanization if notes feel too loose

### Notes Outside Guitar Range
- The plugin works best with MIDI notes E2 (MIDI 40) to E5 (MIDI 76)
- Extreme notes may not voice properly
- Transpose input if needed

## Technical Details

### Guitar Configuration
- **Standard Tuning**: E2, A2, D3, G3, B3, E4
- **6 Strings**: Low E to high E
- **24 Frets**: Full range available
- **Max Fret Span**: 4 frets (like a real hand)

### Voicing Algorithm
1. Analyzes input MIDI notes
2. Finds all possible string/fret combinations
3. Filters for playability (hand position, fret span)
4. Scores each voicing for difficulty
5. Applies smooth voice leading
6. Selects optimal fingering

### Pattern Generation
- Patterns calculated in real-time
- Timing based on sample rate and tempo
- Velocity mapped to MIDI (0-127)
- Note durations calculated per pattern type

## Advanced Usage

### Automation
All parameters can be automated:
- Automate **Pattern** for dynamic arrangement changes
- Automate **Humanization** for more/less tight sections
- Automate **Tempo** for ritardando effects (not recommended)

### MIDI Mapping
- Map **Enable** to a footswitch for live performance
- Map **Pattern** to a knob for quick style changes
- Map **Humanization** for dynamic expression control

### Processing Audio
This is a MIDI effect only - it doesn't process audio. Route the MIDI output to a guitar VST or sampler for sound.

## Support & Feedback

For issues, feature requests, or questions:
- GitHub Issues: [Report a bug or request a feature]
- Documentation: Check README.md for build instructions
- Examples: See the repository for audio demos (coming soon)

---

**Version**: 1.0.0  
**Last Updated**: October 2025  
**Plugin Format**: VST3, AU, Standalone
