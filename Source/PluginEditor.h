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
class GuitarSuiteAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    GuitarSuiteAudioProcessorEditor (GuitarSuiteAudioProcessor&);
    ~GuitarSuiteAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    GuitarSuiteAudioProcessor& audioProcessor;

    // --- ВЕРХНІЙ РЯД (Загальні налаштування) ---
    juce::Slider inVolSlider, gateSlider, resSlider, driveSlider, outVolSlider;
    juce::Label inVolLabel, gateLabel, resLabel, driveLabel, outVolLabel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> inVolAtt, gateAtt, resAtt, driveAtt, outVolAtt;

    // --- КОМПРЕСОР ---
    juce::Slider compAmountSlider, compOutSlider;
    juce::Label compAmountLabel, compOutLabel;
    juce::ToggleButton compActiveBtn{ "Comp On/Off" }; // Кнопка з текстом

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> compAmountAtt, compOutAtt;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> compActiveAtt;

    // --- WAH-WAH ПЕДАЛЬ ---
    juce::Slider wahPosSlider; // Це буде наша вертикальна педаль
    juce::ToggleButton wahActiveBtn{ "Wah On/Off" };

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> wahPosAtt;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> wahActiveAtt;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GuitarSuiteAudioProcessorEditor)

};