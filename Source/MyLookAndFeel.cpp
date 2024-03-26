/*
  ==============================================================================

    MyLookAndFeel.cpp
    Created: 26 Mar 2024 4:45:59pm
    Author:  Admin

  ==============================================================================
*/

#include "MyLookAndFeel.h"

MyLookAndFeel::MyLookAndFeel()
{

}

MyLookAndFeel::~MyLookAndFeel()
{

}

juce::Label* MyLookAndFeel::createSliderTextBox(juce::Slider& myLAFSlider)
{
    juce::Label* myLAFLabelptr = LookAndFeel_V4::createSliderTextBox(myLAFSlider);
    myLAFLabelptr->setColour(juce::Label::textColourId, juce::Colours::palegreen);
    myLAFLabelptr->setColour(juce::Label::backgroundColourId, juce::Colours::black);
    myLAFLabelptr->setColour(juce::Label::outlineColourId, juce::Colours::seagreen);
    return myLAFLabelptr;

}
