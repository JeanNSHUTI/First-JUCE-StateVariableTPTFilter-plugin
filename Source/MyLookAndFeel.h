/*
  ==============================================================================

    MyLookAndFeel.h
    Created: 26 Mar 2024 4:45:59pm
    Author:  Jean-René Nshuti
    based on JUCE GUI tutorial by Oliver Hancock

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>


class MyLookAndFeel : public juce::LookAndFeel_V4
{
public:
    MyLookAndFeel();
    ~MyLookAndFeel();
    juce::Label* createSliderTextBox(juce::Slider&) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MyLookAndFeel)
};
