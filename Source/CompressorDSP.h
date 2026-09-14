/*
  ==============================================================================

    CompressorDSP.h
    Created: 14 Sep 2026 6:41:00pm
    Author:  Анастасия

  ==============================================================================
*/


#pragma once
#include <cmath>

class CompressorDSP
{
public:
    CompressorDSP() = default;
    ~CompressorDSP() = default;

    void prepareSampleRate(float newSampleRate) {
        sampleRate = newSampleRate;

        // Швидкість реакції нашого "звукорежисера"
        attack = std::exp(-1.0f / (0.010f * sampleRate));
        release = std::exp(-1.0f / (0.100f * sampleRate));
    }

    void setAmount(float newAmount) { amount = newAmount; }
    void setOutput(float newOutput) { output = newOutput; }

    float processSample(float inputSample);

private:
    float sampleRate = 44100.0f;
    float amount = 0.5f;   // Ручка Compression з педалі (від 0.0 до 1.0)
    float output = 1.0f;   // Ручка Output з педалі (від 0.0 до 2.0)

    float envelope = 0.0f; // Поточна усереднена гучність
    float attack = 0.0f;
    float release = 0.0f;
};