/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class JucebasicfilterpluginAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:

    juce::AudioProcessorValueTreeState myValueTreeState;

    juce::AudioParameterInt* myCutoffptr;
    juce::AudioParameterFloat* myResonanceptr;
    juce::AudioParameterChoice* myTypeptr;
    juce::AudioParameterBool* myBypassptr;
    juce::AudioParameterFloat* lfoDepthptr;

    //==============================================================================
    JucebasicfilterpluginAudioProcessor();
    ~JucebasicfilterpluginAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    //==============================================================================
    juce::dsp::StateVariableTPTFilter<float> myFirstJuceFilter;
    juce::dsp::ProcessSpec processSpec;


    juce::AudioPlayHead* myPlayheadptr;
    juce::AudioPlayHead::CurrentPositionInfo myHostTiming;
    std::atomic<float> mySongPosition;
    std::atomic<float> myLfoPhase;
    std::atomic<float> myFilterFrequency;


    //==============================================================================
    juce::dsp::StateVariableTPTFilterType getFilterType();


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JucebasicfilterpluginAudioProcessor)
};
