/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ManualWahDSP.h"
#include "GateDSP.h"
#include "OverdriveDSP.h"
#include "CompressorDSP.h"
//==============================================================================
/**
*/
class GuitarSuiteAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    GuitarSuiteAudioProcessor();
    ~GuitarSuiteAudioProcessor() override;

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
    juce::AudioProcessorValueTreeState apvts;

private:

    //ручна педаль 
    ManualWahDSP wahLeft;
    ManualWahDSP wahRight;
    GateDSP gateLeft;
    GateDSP gateRight;
    OverdriveDSP driveLeft;
    OverdriveDSP driveRight;
	CompressorDSP compLeft;
	CompressorDSP compRight;
    // Менеджер параметрів передає значення між математикою та самою педаллю
    

    // Функція, яка створить лейаут
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GuitarSuiteAudioProcessor)
};
