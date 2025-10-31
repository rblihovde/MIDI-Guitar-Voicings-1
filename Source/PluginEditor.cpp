#include "PluginProcessor.h"
#include "PluginEditor.h"

MIDIGuitarVoicingsEditor::MIDIGuitarVoicingsEditor(MIDIGuitarVoicingsProcessor& p)
    : AudioProcessorEditor(&p), m_processor(p)
{
    setLookAndFeel(&m_darkLookAndFeel);
    
    // Set up title
    setupLabel(m_titleLabel, "MIDI Guitar Voicings", 24.0f);
    m_titleLabel.setFont(juce::Font(24.0f, juce::Font::bold));
    m_titleLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(m_titleLabel);
    
    // Enable button
    m_enableButton.setButtonText("Enabled");
    m_enableButton.setColour(juce::ToggleButton::tickColourId, juce::Colour(DarkTheme::success));
    addAndMakeVisible(m_enableButton);
    
    setupLabel(m_enableLabel, "Enable", 14.0f);
    addAndMakeVisible(m_enableLabel);
    
    m_enableAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        m_processor.getParameters(), "enabled", m_enableButton);
    
    // Pattern selector
    m_patternSelector.addItem("Strum", 1);
    m_patternSelector.addItem("Fingerpick", 2);
    m_patternSelector.addItem("Arpeggio", 3);
    m_patternSelector.addItem("Tremolo", 4);
    addAndMakeVisible(m_patternSelector);
    
    setupLabel(m_patternLabel, "Pattern", 14.0f);
    addAndMakeVisible(m_patternLabel);
    
    m_patternAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        m_processor.getParameters(), "pattern", m_patternSelector);
    
    // Humanization slider
    m_humanizationSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    m_humanizationSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    setupSlider(m_humanizationSlider, m_humanizationValue);
    addAndMakeVisible(m_humanizationSlider);
    
    setupLabel(m_humanizationLabel, "Humanization", 14.0f);
    m_humanizationLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(m_humanizationLabel);
    
    addAndMakeVisible(m_humanizationValue);
    
    m_humanizationAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        m_processor.getParameters(), "humanization", m_humanizationSlider);
    
    // Tempo slider
    m_tempoSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    m_tempoSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    setupSlider(m_tempoSlider, m_tempoValue);
    addAndMakeVisible(m_tempoSlider);
    
    setupLabel(m_tempoLabel, "Tempo (BPM)", 14.0f);
    m_tempoLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(m_tempoLabel);
    
    addAndMakeVisible(m_tempoValue);
    
    m_tempoAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        m_processor.getParameters(), "tempo", m_tempoSlider);
    
    // Start timer for value updates
    startTimerHz(30);
    
    setSize(600, 400);
}

MIDIGuitarVoicingsEditor::~MIDIGuitarVoicingsEditor()
{
    setLookAndFeel(nullptr);
}

void MIDIGuitarVoicingsEditor::paint(juce::Graphics& g)
{
    // Background
    g.fillAll(juce::Colour(DarkTheme::background));
    
    // Main panel
    auto bounds = getLocalBounds();
    auto panelBounds = bounds.reduced(20);
    
    g.setColour(juce::Colour(DarkTheme::panel));
    g.fillRoundedRectangle(panelBounds.toFloat(), 10.0f);
    
    g.setColour(juce::Colour(DarkTheme::border));
    g.drawRoundedRectangle(panelBounds.toFloat(), 10.0f, 2.0f);
    
    // Decorative accent line
    g.setColour(juce::Colour(DarkTheme::accent));
    g.fillRoundedRectangle(panelBounds.getX() + 10, panelBounds.getY() + 60, 
                          panelBounds.getWidth() - 20, 2, 1.0f);
}

void MIDIGuitarVoicingsEditor::resized()
{
    auto bounds = getLocalBounds().reduced(40, 40);
    
    // Title
    m_titleLabel.setBounds(bounds.removeFromTop(50));
    
    bounds.removeFromTop(20); // Spacing after accent line
    
    // Top section with enable and pattern
    auto topSection = bounds.removeFromTop(80);
    
    auto enableSection = topSection.removeFromLeft(topSection.getWidth() / 2);
    m_enableLabel.setBounds(enableSection.removeFromTop(25));
    m_enableButton.setBounds(enableSection.withHeight(30));
    
    auto patternSection = topSection.reduced(20, 0);
    m_patternLabel.setBounds(patternSection.removeFromTop(25));
    m_patternSelector.setBounds(patternSection.withHeight(30));
    
    bounds.removeFromTop(30); // Spacing
    
    // Knobs section
    auto knobsSection = bounds.removeFromTop(180);
    
    int knobWidth = knobsSection.getWidth() / 2;
    
    // Humanization knob
    auto humanSection = knobsSection.removeFromLeft(knobWidth);
    m_humanizationSlider.setBounds(humanSection.removeFromTop(140).reduced(20));
    m_humanizationLabel.setBounds(humanSection.removeFromTop(20));
    m_humanizationValue.setBounds(humanSection.removeFromTop(20));
    
    // Tempo knob
    auto tempoSection = knobsSection;
    m_tempoSlider.setBounds(tempoSection.removeFromTop(140).reduced(20));
    m_tempoLabel.setBounds(tempoSection.removeFromTop(20));
    m_tempoValue.setBounds(tempoSection.removeFromTop(20));
}

void MIDIGuitarVoicingsEditor::timerCallback()
{
    // Update value labels
    float humanization = m_processor.getHumanization();
    m_humanizationValue.setText(juce::String(humanization * 100.0f, 0) + "%", 
                               juce::dontSendNotification);
    
    float tempo = m_processor.getTempo();
    m_tempoValue.setText(juce::String(tempo, 1) + " BPM",
                        juce::dontSendNotification);
}

void MIDIGuitarVoicingsEditor::setupLabel(juce::Label& label, const juce::String& text, float fontSize)
{
    label.setText(text, juce::dontSendNotification);
    label.setFont(juce::Font(fontSize));
    label.setColour(juce::Label::textColourId, juce::Colour(DarkTheme::text));
    label.setJustificationType(juce::Justification::centredLeft);
}

void MIDIGuitarVoicingsEditor::setupSlider(juce::Slider& slider, juce::Label& valueLabel)
{
    slider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(DarkTheme::accent));
    slider.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(DarkTheme::border));
    slider.setColour(juce::Slider::thumbColourId, juce::Colour(DarkTheme::text));
    
    valueLabel.setFont(juce::Font(14.0f));
    valueLabel.setColour(juce::Label::textColourId, juce::Colour(DarkTheme::textDim));
    valueLabel.setJustificationType(juce::Justification::centred);
}
