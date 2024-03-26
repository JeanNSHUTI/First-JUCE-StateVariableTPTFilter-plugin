/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
JucebasicfilterpluginAudioProcessorEditor::JucebasicfilterpluginAudioProcessorEditor (JucebasicfilterpluginAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize (400, 300);

    myCutoffSlider.setLookAndFeel(&myLookAndFeel);
    myResonanceSlider.setLookAndFeel(&myLookAndFeel);
    myLfoDepthSlider.setLookAndFeel(&myLookAndFeel);
    myLookAndFeel.setColour(juce::Slider::backgroundColourId, juce::Colours::black);
    myLookAndFeel.setColour(juce::Slider::thumbColourId, juce::Colours::palegreen);
    myLookAndFeel.setColour(juce::Slider::trackColourId, juce::Colours::seagreen);
    myLookAndFeel.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::seagreen);
    myLookAndFeel.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::black);
    addAndMakeVisible(myCutoffSlider);
    myCutoffSlider.setSliderStyle(juce::Slider::LinearHorizontal);

    myCutoffSliderAttachmentptr.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.myValueTreeState, "cutoff", myCutoffSlider));
    addAndMakeVisible(myCutoffLabel);
    myCutoffLabel.attachToComponent(&myCutoffSlider, false);

    addAndMakeVisible(myResonanceSlider);
    myResonanceSlider.setSliderStyle(juce::Slider::LinearVertical);
    myResonanceSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);

    myResonanceSliderAttachmentptr.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.myValueTreeState, "resonance", myResonanceSlider));
    addAndMakeVisible(myResonanceLabel);
    myResonanceLabel.attachToComponent(&myResonanceSlider, false);

    addAndMakeVisible(myLfoDepthSlider);
    myLfoDepthSlider.setSliderStyle(juce::Slider::Rotary);
    myLfoDepthSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);

    myLfoDepthSliderAttachmentptr.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.myValueTreeState, "lfoDepth", myLfoDepthSlider));
    addAndMakeVisible(myLfoDepthLabel);
    myLfoDepthLabel.attachToComponent(&myLfoDepthSlider, true);

    myTypeComboBox.setLookAndFeel(&myLookAndFeel);
    myLookAndFeel.setColour(juce::ComboBox::backgroundColourId, juce::Colours::black);
    myLookAndFeel.setColour(juce::ComboBox::textColourId, juce::Colours::palegreen);
    myLookAndFeel.setColour(juce::ComboBox::outlineColourId, juce::Colours::seagreen);
    myLookAndFeel.setColour(juce::ComboBox::arrowColourId, juce::Colours::seagreen);
    addAndMakeVisible(myTypeComboBox);
    myTypeComboBox.addItem("LowPass", 1);
    myTypeComboBox.addItem("HighPass", 2);
    myTypeComboBox.addItem("BandPass", 3);

    myTypeComboBoxAttachmentptr.reset(new juce::AudioProcessorValueTreeState::ComboBoxAttachment(audioProcessor.myValueTreeState, "type", myTypeComboBox));

    myBypassToggleButton.setLookAndFeel(&myLookAndFeel);
    myLookAndFeel.setColour(juce::ToggleButton::textColourId, juce::Colours::white);
    myLookAndFeel.setColour(juce::ToggleButton::tickColourId, juce::Colours::palegreen);
    myLookAndFeel.setColour(juce::ToggleButton::tickDisabledColourId, juce::Colours::seagreen);
    addAndMakeVisible(myBypassToggleButton);

    myToggleButtonAttachmentptr.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(audioProcessor.myValueTreeState, "bypass", myBypassToggleButton));

    addAndMakeVisible(myTitleLabel);

}

JucebasicfilterpluginAudioProcessorEditor::~JucebasicfilterpluginAudioProcessorEditor()
{
}

//==============================================================================
void JucebasicfilterpluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    //g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void JucebasicfilterpluginAudioProcessorEditor::resized()
{
    myCutoffSlider.setBounds(50, 50, 350, 50);
    myResonanceSlider.setBounds(0, 50, 50, 150);
    myTypeComboBox.setBounds(50, 100, 100, 50);
    myBypassToggleButton.setBounds(50, 150, 200, 50);
    myLfoDepthSlider.setBounds(50, 200, 100, 100);
    myTitleLabel.setBounds(200, 0, 100, 50);
}
