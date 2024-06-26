/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <math.h>

//==============================================================================
JucebasicfilterpluginAudioProcessor::JucebasicfilterpluginAudioProcessor()
    : myValueTreeState(*this, nullptr, juce::Identifier("myParamaters"),
        {
            std::make_unique<juce::AudioParameterInt>("cutoff", "CUTOFF", 20, 20000, 1000),
            std::make_unique<juce::AudioParameterFloat>("resonance", "RES", 0.707f, 10.0f, 2.66f),
            std::make_unique<juce::AudioParameterChoice>("type", "TYPE", juce::StringArray{"LowPass", "HighPass", "BandPass"}, 2),
            std::make_unique<juce::AudioParameterBool>("bypass", "BYPASS", 0),
            std::make_unique<juce::AudioParameterFloat>("lfoDepth", "LFO DEPTH", 0.0f, 1.0f, 0.0f)
        })
{
    myCutoffptr = dynamic_cast<juce::AudioParameterInt*>(myValueTreeState.getParameter("cutoff"));
    myResonanceptr = dynamic_cast<juce::AudioParameterFloat*>(myValueTreeState.getParameter("resonance"));
    myTypeptr = dynamic_cast<juce::AudioParameterChoice*>(myValueTreeState.getParameter("type"));
    myBypassptr = dynamic_cast<juce::AudioParameterBool*>(myValueTreeState.getParameter("bypass"));
    lfoDepthptr = dynamic_cast<juce::AudioParameterFloat*>(myValueTreeState.getParameter("lfoDepth"));
}

JucebasicfilterpluginAudioProcessor::~JucebasicfilterpluginAudioProcessor()
{
}

//==============================================================================
const juce::String JucebasicfilterpluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool JucebasicfilterpluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool JucebasicfilterpluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool JucebasicfilterpluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double JucebasicfilterpluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int JucebasicfilterpluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int JucebasicfilterpluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void JucebasicfilterpluginAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String JucebasicfilterpluginAudioProcessor::getProgramName (int index)
{
    return {};
}

void JucebasicfilterpluginAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void JucebasicfilterpluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    processSpec.sampleRate = sampleRate;
    processSpec.maximumBlockSize = samplesPerBlock;
    processSpec.numChannels = getTotalNumOutputChannels();

    myFirstJuceFilter.prepare(processSpec);
    myFirstJuceFilter.reset();
}

void JucebasicfilterpluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool JucebasicfilterpluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void JucebasicfilterpluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    myPlayheadptr = this->getPlayHead();
    myPlayheadptr->getCurrentPosition(myHostTiming);
    mySongPosition = myHostTiming.ppqPosition;      //pulses (as figures-fractions) per quarternotes

    myLfoPhase = std::fmod(mySongPosition, 1);

    myFilterFrequency = myCutoffptr->get() + (myCutoffptr->get() * (myLfoPhase * lfoDepthptr->get()));

    myFirstJuceFilter.setCutoffFrequency(myFilterFrequency);
    myFirstJuceFilter.setResonance(myResonanceptr->get());
    myFirstJuceFilter.setType(getFilterType());

    if (!myBypassptr->get()) 
    {
        auto myBlock = juce::dsp::AudioBlock<float>(buffer);
        auto myContext = juce::dsp::ProcessContextReplacing<float>(myBlock);

        myFirstJuceFilter.process(myContext);
    }
}

//==============================================================================
bool JucebasicfilterpluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* JucebasicfilterpluginAudioProcessor::createEditor()
{
    return new JucebasicfilterpluginAudioProcessorEditor(*this);
}

//==============================================================================
void JucebasicfilterpluginAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = myValueTreeState.copyState();
    std::unique_ptr<juce::XmlElement>xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void JucebasicfilterpluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement>xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(myValueTreeState.state.getType()))
            myValueTreeState.replaceState(juce::ValueTree::fromXml(*xmlState));
}

juce::dsp::StateVariableTPTFilterType JucebasicfilterpluginAudioProcessor:: getFilterType()
{
    switch(myTypeptr->getIndex())
    {
    case 0:
        return juce::dsp::StateVariableTPTFilterType::lowpass;
        break;
    case 1:
        return juce::dsp::StateVariableTPTFilterType::highpass;
        break;
    case 2:
        return juce::dsp::StateVariableTPTFilterType::bandpass;
        break;
    default:
        return juce::dsp::StateVariableTPTFilterType::bandpass;

    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new JucebasicfilterpluginAudioProcessor();
}
