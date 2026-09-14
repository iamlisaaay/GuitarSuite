/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GuitarSuiteAudioProcessor::GuitarSuiteAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), 
	apvts(*this, nullptr, "Parameters", createParameterLayout()) //ініціалізація менеджера параметрів

#endif
{
}

GuitarSuiteAudioProcessor::~GuitarSuiteAudioProcessor()
{
}

//==============================================================================
const juce::String GuitarSuiteAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool GuitarSuiteAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool GuitarSuiteAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool GuitarSuiteAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double GuitarSuiteAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int GuitarSuiteAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int GuitarSuiteAudioProcessor::getCurrentProgram()
{
    return 0;
}

void GuitarSuiteAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String GuitarSuiteAudioProcessor::getProgramName (int index)
{
    return {};
}

void GuitarSuiteAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void GuitarSuiteAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    wahLeft.prepareSampleRate(sampleRate);
    wahRight.prepareSampleRate(sampleRate);

    gateLeft.prepareSampleRate(sampleRate);
    gateRight.prepareSampleRate(sampleRate);

    driveLeft.prepareSampleRate(sampleRate);
    driveRight.prepareSampleRate(sampleRate);

	compLeft.prepareSampleRate(sampleRate);
	compRight.prepareSampleRate(sampleRate);
}

void GuitarSuiteAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool GuitarSuiteAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void GuitarSuiteAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    // 1. Витягуємо позицію ручок з інтерфейсу
    float inVol = apvts.getRawParameterValue("in_volume")->load();
    float outVol = apvts.getRawParameterValue("out_volume")->load();
    bool isWahActive = apvts.getRawParameterValue("wah_active")->load() > 0.5f;
    bool isCompActive = apvts.getRawParameterValue("comp_active")->load() > 0.5f;
    float currentPos = apvts.getRawParameterValue("position")->load();
    float currentRes = apvts.getRawParameterValue("resonance")->load();
    float currentThreshold = apvts.getRawParameterValue("threshold")->load();
    float currentDrive = apvts.getRawParameterValue("drive")->load();
	float currentCompAmount = apvts.getRawParameterValue("comp_amount")->load();
	float currentCompOutput = apvts.getRawParameterValue("comp_output")->load();

    // 2. Передаємо налаштування у DSP
    wahLeft.setPosition(currentPos);
    wahLeft.setQ(currentRes);
    wahRight.setPosition(currentPos);
    wahRight.setQ(currentRes);

    gateLeft.setThreshold(currentThreshold);
    gateRight.setThreshold(currentThreshold);

    driveLeft.setDrive(currentDrive);
    driveLeft.setVolume(0.5f);
    driveRight.setDrive(currentDrive);
    driveRight.setVolume(0.5f);

	compLeft.setAmount(currentCompAmount);
	compLeft.setOutput(currentCompOutput);
	compRight.setAmount(currentCompAmount);
	compRight.setOutput(currentCompOutput);


    // 3. Беремо вказівники на лівий і правий канали
    auto* leftChannel = buffer.getWritePointer(0);
    auto* rightChannel = buffer.getWritePointer(1);

    // 4. Пропускаємо кожен семпл через фільтри
    for (int i = 0; i < buffer.getNumSamples(); ++i)
    {
        leftChannel[i] *= inVol;
        rightChannel[i] *= inVol;

      
        leftChannel[i] = gateLeft.processSample(leftChannel[i]);
        rightChannel[i] = gateRight.processSample(rightChannel[i]);

        if (isCompActive) {
            leftChannel[i] = compLeft.processSample(leftChannel[i]);
            rightChannel[i] = compRight.processSample(rightChannel[i]);
        }
        leftChannel[i] = driveLeft.processSample(leftChannel[i]);
        rightChannel[i] = driveRight.processSample(rightChannel[i]);

        if (isWahActive) {
            leftChannel[i] = wahLeft.processSample(leftChannel[i]);
            rightChannel[i] = wahRight.processSample(rightChannel[i]);
        }
        leftChannel[i] *= outVol;
        rightChannel[i] *= outVol;
        
    }
}

//==============================================================================
bool GuitarSuiteAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* GuitarSuiteAudioProcessor::createEditor()
{
    return new GuitarSuiteAudioProcessorEditor (*this);
}

//==============================================================================
void GuitarSuiteAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void GuitarSuiteAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GuitarSuiteAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout GuitarSuiteAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    layout.add(std::make_unique<juce::AudioParameterFloat>("in_volume", "Input", juce::NormalisableRange<float>(0.0f, 2.0f, 0.01f), 1.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("out_volume", "Output", juce::NormalisableRange<float>(0.0f, 2.0f, 0.01f), 1.0f));

    layout.add(std::make_unique<juce::AudioParameterBool>("wah_active", "Wah Active", true));
    layout.add(std::make_unique<juce::AudioParameterBool>("comp_active", "Comp Active", false));
    //Ручка Position  
    // Від 0.0 до 1.0. Крок зміни: 0.01. Стартує з 0.0 
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "position", "Position",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.0f));

    // Ручка Resonance 
    // Від 0.01 до 0.5. Стартує з 0.05.
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "resonance", "Resonance",
        juce::NormalisableRange<float>(0.01f, 0.5f, 0.01f), 0.05f));

	// Ручка Threshold для гейту
	// Від 0.0 до 0.2. Крок зміни: 0.001. Стартує з 0.05
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "threshold", "Threshold",
        juce::NormalisableRange<float>(0.0f, 0.2f, 0.001f), 0.05f));

	// Ручка Drive для овердрайву
	// Від 1.0 до 50.0. Крок зміни: 0.1. Стартує з 10.0
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "drive", "Drive",
        juce::NormalisableRange<float>(1.0f, 50.0f, 0.1f), 10.0f));

	// Ручка Amount для компресора
	// Від 0.0 до 1.0. Крок зміни: 0.01. Стартує з 0.5
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "comp_amount", "Compression",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f));

	// Ручка Output для компресора
	// Від 0.0 до 1.0. Крок зміни: 0.01. Стартує з 0.5
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "comp_output", "Makeup Gain",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f));



    return layout;
}

