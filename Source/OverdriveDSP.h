/*
  ==============================================================================

    OverdriveDSP.h
    Created: 14 Sep 2026 6:23:17pm
    Author:  Анастасия

  ==============================================================================
*/

#pragma once

class OverdriveDSP
{
public:
    OverdriveDSP() = default;
    ~OverdriveDSP() = default;

    void prepareSampleRate(float newSampleRate) { sampleRate = newSampleRate; }

    // Приймаємо налаштування з інтерфейсу
    void setDrive(float newDrive) { drive = newDrive; }
    void setVolume(float newVolume) { volume = newVolume; }

    float processSample(float inputSample);

private:
    float sampleRate = 44100.0f;

    // Значення за замовчуванням
    float drive = 1.0f;   // Множник вхідного сигналу 
    float volume = 1.0f;  // Гучність на виході 
};
