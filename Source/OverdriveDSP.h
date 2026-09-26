/*
  ==============================================================================

    OverdriveDSP.h
    Created: 14 Sep 2026 6:23:17pm
    Author:  Анастасия

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
class OverdriveDSP
{
public:
    OverdriveDSP() = default;
    ~OverdriveDSP() = default;

    void prepareSampleRate(float newSampleRate) {
        sampleRate = newSampleRate;
        drive.reset(sampleRate, 0.02);
    }
    // Приймаємо налаштування з інтерфейсу
    void setDrive(float newDrive) {
        drive.setTargetValue(newDrive);
    }
    void setVolume(float newVolume) { volume = newVolume; }

    float processSample(float inputSample);

private:
    float sampleRate = 44100.0f;

    // Значення за замовчуванням
    juce::SmoothedValue<float> drive;   // Множник вхідного сигналу 
    float volume = 1.0f;  // Гучність на виході 
};
