/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GuitarSuiteAudioProcessorEditor::GuitarSuiteAudioProcessorEditor(GuitarSuiteAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    // 1. НАЛАШТУВАННЯ СТИЛІВ
    // Робимо всі звичайні ручки круглими і ховаємо цифри
    auto setupRotary = [this](juce::Slider& s, juce::Label& l, const juce::String& name) {
        s.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        s.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
        addAndMakeVisible(s);

        l.setText(name, juce::dontSendNotification);
        l.setJustificationType(juce::Justification::centred);
        l.attachToComponent(&s, false); // Підпис буде автоматично зверху
        };

    setupRotary(inVolSlider, inVolLabel, "INPUT");
    setupRotary(gateSlider, gateLabel, "GATE THRESHOLD");
    setupRotary(resSlider, resLabel, "WAH RESONANCE");
    setupRotary(driveSlider, driveLabel, "OVERDRIVE");
    setupRotary(outVolSlider, outVolLabel, "OUTPUT");
    setupRotary(compAmountSlider, compAmountLabel, "AMOUNT");
    setupRotary(compOutSlider, compOutLabel, "MAKEUP");

    // Педаль Wah-Wah (Вертикальна!)
    wahPosSlider.setSliderStyle(juce::Slider::LinearVertical);
    wahPosSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible(wahPosSlider);

    // Кнопки
    addAndMakeVisible(compActiveBtn);
    addAndMakeVisible(wahActiveBtn);

    // 2. БУДУЄМО МОСТИ (Підключаємо до ID з PluginProcessor)
    inVolAtt = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "in_volume", inVolSlider);
    gateAtt = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "threshold", gateSlider);
    resAtt = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "resonance", resSlider);
    driveAtt = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "drive", driveSlider);
    outVolAtt = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "out_volume", outVolSlider);

    compAmountAtt = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "comp_amount", compAmountSlider);
    compOutAtt = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "comp_output", compOutSlider);
    compActiveAtt = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "comp_active", compActiveBtn);

    wahPosAtt = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "position", wahPosSlider);
    wahActiveAtt = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "wah_active", wahActiveBtn);

    // 3. ЗБІЛЬШУЄМО ВІКНО ПЛАГІНА
    setSize(900, 600);
}
GuitarSuiteAudioProcessorEditor::~GuitarSuiteAudioProcessorEditor()
{
}

//==============================================================================
void GuitarSuiteAudioProcessorEditor::paint(juce::Graphics& g)
{
    // Темний студійний фон
    g.fillAll(juce::Colour(35, 35, 40));

    // Малюємо бокс для Компресора (сріблясто-сірий)
    g.setColour(juce::Colour(90, 95, 100));
    g.fillRoundedRectangle(150, 220, 250, 300, 10.0f);
    g.setColour(juce::Colours::white);
    g.setFont(20.0f);
    g.drawText("COMPRESSOR", 150, 230, 250, 30, juce::Justification::centred, false);

    // Малюємо бокс для Wah-Wah педалі (темніший)
    g.setColour(juce::Colour(45, 45, 50));
    g.fillRoundedRectangle(500, 180, 250, 380, 15.0f);
    g.setColour(juce::Colours::grey);
    g.drawText("WAH-WAH PEDAL", 500, 190, 250, 30, juce::Justification::centred, false);
}

void GuitarSuiteAudioProcessorEditor::resized()
{
    // --- ВЕРХНІЙ РЯД (Y = 60) ---
    // Тепер тут 4 ручки, ми розставили їх рівномірніше
    inVolSlider.setBounds(130, 60, 100, 100);
    gateSlider.setBounds(310, 60, 100, 100);
    driveSlider.setBounds(490, 60, 100, 100);
    outVolSlider.setBounds(670, 60, 100, 100);

    // --- КОМПРЕСОР БОКС ---
    compAmountSlider.setBounds(165, 290, 100, 100);
    compOutSlider.setBounds(285, 290, 100, 100);
    compActiveBtn.setBounds(230, 450, 120, 30);

    // --- WAH-WAH БОКС ---
    // Вертикальна педаль (трохи посунута вліво)
    wahPosSlider.setBounds(530, 230, 80, 250);

    // Ручка Resonance (стоїть праворуч від педалі)
    resSlider.setBounds(630, 300, 100, 100);

    // Кнопка увімкнення (по центру під педаллю та ручкою)
    wahActiveBtn.setBounds(565, 510, 120, 30);
}