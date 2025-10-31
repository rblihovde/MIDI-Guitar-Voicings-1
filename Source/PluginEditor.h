#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include "PluginProcessor.h"

/**
 * @brief Modern dark mode UI for the MIDI Guitar Voicings plugin
 */
class MIDIGuitarVoicingsEditor : public juce::AudioProcessorEditor,
                                  private juce::Timer
{
public:
    MIDIGuitarVoicingsEditor(MIDIGuitarVoicingsProcessor&);
    ~MIDIGuitarVoicingsEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    void timerCallback() override;
    
    MIDIGuitarVoicingsProcessor& m_processor;
    
    // Custom dark theme colors
    struct DarkTheme
    {
        static constexpr juce::uint32 background = 0xff1a1a1a;
        static constexpr juce::uint32 panel = 0xff252525;
        static constexpr juce::uint32 border = 0xff3a3a3a;
        static constexpr juce::uint32 accent = 0xff00a8ff;
        static constexpr juce::uint32 accentHover = 0xff33b8ff;
        static constexpr juce::uint32 text = 0xffe0e0e0;
        static constexpr juce::uint32 textDim = 0xff909090;
        static constexpr juce::uint32 success = 0xff00ff88;
        static constexpr juce::uint32 warning = 0xffffaa00;
    };
    
    // UI Components
    juce::Label m_titleLabel;
    
    juce::ToggleButton m_enableButton;
    juce::Label m_enableLabel;
    
    juce::ComboBox m_patternSelector;
    juce::Label m_patternLabel;
    
    juce::Slider m_humanizationSlider;
    juce::Label m_humanizationLabel;
    juce::Label m_humanizationValue;
    
    juce::Slider m_tempoSlider;
    juce::Label m_tempoLabel;
    juce::Label m_tempoValue;
    
    // Parameter attachments
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> m_enableAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> m_patternAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_humanizationAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_tempoAttachment;
    
    // Custom look and feel
    class DarkLookAndFeel : public juce::LookAndFeel_V4
    {
    public:
        DarkLookAndFeel()
        {
            setColour(juce::Slider::thumbColourId, juce::Colour(DarkTheme::accent));
            setColour(juce::Slider::trackColourId, juce::Colour(DarkTheme::accent));
            setColour(juce::Slider::backgroundColourId, juce::Colour(DarkTheme::border));
            setColour(juce::Slider::textBoxTextColourId, juce::Colour(DarkTheme::text));
            setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(DarkTheme::panel));
            setColour(juce::Slider::textBoxOutlineColourId, juce::Colour(DarkTheme::border));
            
            setColour(juce::ComboBox::backgroundColourId, juce::Colour(DarkTheme::panel));
            setColour(juce::ComboBox::textColourId, juce::Colour(DarkTheme::text));
            setColour(juce::ComboBox::outlineColourId, juce::Colour(DarkTheme::border));
            setColour(juce::ComboBox::buttonColourId, juce::Colour(DarkTheme::accent));
            setColour(juce::ComboBox::arrowColourId, juce::Colour(DarkTheme::text));
            
            setColour(juce::ToggleButton::textColourId, juce::Colour(DarkTheme::text));
            setColour(juce::ToggleButton::tickColourId, juce::Colour(DarkTheme::accent));
            setColour(juce::ToggleButton::tickDisabledColourId, juce::Colour(DarkTheme::border));
            
            setColour(juce::Label::textColourId, juce::Colour(DarkTheme::text));
            
            setColour(juce::PopupMenu::backgroundColourId, juce::Colour(DarkTheme::panel));
            setColour(juce::PopupMenu::textColourId, juce::Colour(DarkTheme::text));
            setColour(juce::PopupMenu::highlightedBackgroundColourId, juce::Colour(DarkTheme::accent));
            setColour(juce::PopupMenu::highlightedTextColourId, juce::Colour(DarkTheme::text));
        }
        
        void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                            float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
                            juce::Slider& slider) override
        {
            juce::ignoreUnused(slider);
            auto radius = juce::jmin(width / 2, height / 2) - 10.0f;
            auto centreX = x + width * 0.5f;
            auto centreY = y + height * 0.5f;
            auto rx = centreX - radius;
            auto ry = centreY - radius;
            auto rw = radius * 2.0f;
            auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
            
            // Background arc
            g.setColour(juce::Colour(DarkTheme::border));
            g.drawEllipse(rx, ry, rw, rw, 2.0f);
            
            // Value arc
            juce::Path valueArc;
            valueArc.addCentredArc(centreX, centreY, radius, radius,
                                  0.0f, rotaryStartAngle, angle, true);
            
            g.setColour(juce::Colour(DarkTheme::accent));
            g.strokePath(valueArc, juce::PathStrokeType(3.0f));
            
            // Pointer
            juce::Path pointer;
            auto pointerLength = radius * 0.6f;
            auto pointerThickness = 3.0f;
            pointer.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
            pointer.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));
            
            g.setColour(juce::Colour(DarkTheme::text));
            g.fillPath(pointer);
        }
    };
    
    DarkLookAndFeel m_darkLookAndFeel;
    
    void setupLabel(juce::Label& label, const juce::String& text, float fontSize = 14.0f);
    void setupSlider(juce::Slider& slider, juce::Label& valueLabel);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MIDIGuitarVoicingsEditor)
};
