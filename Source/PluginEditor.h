/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class JucebasicfilterpluginAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    JucebasicfilterpluginAudioProcessorEditor (JucebasicfilterpluginAudioProcessor&);
    ~JucebasicfilterpluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    JucebasicfilterpluginAudioProcessor& audioProcessor;

    juce::Slider myCutoffSlider;
    juce::Slider myResonanceSlider;
    juce::Slider myLfoDepthSlider;
    juce::ComboBox myTypeComboBox;
    juce::ToggleButton myBypassToggleButton{"BYPASS"};

    juce::Label myResonanceLabel{ {}, "RES"};
    juce::Label myCutoffLabel{ {}, "FREQ"};
    juce::Label myTitleLabel{ {}, "FILTER"};
    juce::Label myLfoDepthLabel{ {}, "LFO DEPTH"};

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> myCutoffSliderAttachmentptr;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> myResonanceSliderAttachmentptr;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> myLfoDepthSliderAttachmentptr;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> myTypeComboBoxAttachmentptr;
    std::unique_ptr < juce::AudioProcessorValueTreeState::ButtonAttachment> myToggleButtonAttachmentptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JucebasicfilterpluginAudioProcessorEditor)
};
