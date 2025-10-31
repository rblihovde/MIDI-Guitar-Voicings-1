# MIDI Guitar Voicings

A professional JUCE VST3/AU plugin that creates realistic MIDI patterns and voicings that sound like guitar picking.

## Features

- **Realistic Guitar Voicings**: Automatically converts MIDI input into playable guitar chord voicings
- **Multiple Playing Styles**: 
  - Strum (downstroke/upstroke)
  - Fingerpicking patterns
  - Arpeggios
  - Tremolo picking
- **Humanization**: Natural timing and velocity variations for realistic performance
- **Modern Dark Mode UI**: Sleek, professional interface optimized for music production
- **DAW Integration**: Works as VST3/AU plugin in Logic Pro, Ableton, FL Studio, etc.
- **Standalone Mode**: Can run as standalone MIDI processor

## Technical Details

The plugin uses advanced algorithms to:
- Calculate physically playable guitar voicings based on standard tuning (E-A-D-G-B-E)
- Respect fret span limitations and string selection constraints
- Apply smooth voice leading between chord changes
- Generate realistic picking patterns with natural timing variations

## Requirements

- macOS 10.13+ or Windows 10+
- VST3 or AU compatible DAW (e.g., Logic Pro, Ableton Live, FL Studio)
- CMake 3.15 or higher (for building from source)
- C++17 compatible compiler

## Building from Source

### Prerequisites

```bash
# Install CMake (if not already installed)
# macOS:
brew install cmake

# Ubuntu/Debian:
sudo apt-get install cmake build-essential

# Windows: Download from cmake.org
```

### Build Instructions

```bash
# Clone the repository
git clone https://github.com/rblihovde/MIDI-Guitar-Voicings-1.git
cd MIDI-Guitar-Voicings-1

# Create build directory
mkdir build
cd build

# Configure and build
cmake ..
cmake --build . --config Release

# The built plugins will be in:
# build/MIDIGuitarVoicings_artefacts/Release/
```

## Installation

### VST3 (Windows/macOS)
Copy the `.vst3` file to:
- **macOS**: `~/Library/Audio/Plug-Ins/VST3/`
- **Windows**: `C:\Program Files\Common Files\VST3\`

### AU (macOS only)
Copy the `.component` file to:
- `~/Library/Audio/Plug-Ins/Components/`

## Usage

1. **Load the plugin** in your DAW as a MIDI effect/processor
2. **Send MIDI chords** to the plugin (from a MIDI keyboard or clip)
3. **Select a pattern** from the dropdown (Strum, Fingerpick, Arpeggio, or Tremolo)
4. **Adjust humanization** to control timing/velocity variations
5. **Set tempo** to match your project
6. The plugin outputs realistic guitar MIDI patterns

## Parameters

- **Enabled**: Toggle the plugin processing on/off
- **Pattern Type**: Choose between Strum, Fingerpick, Arpeggio, or Tremolo
- **Humanization**: 0-100% - Controls natural timing and velocity variations
- **Tempo**: 40-240 BPM - Affects pattern timing calculations

## License

[Add your license here]

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## Credits

Built with [JUCE Framework](https://juce.com/)
