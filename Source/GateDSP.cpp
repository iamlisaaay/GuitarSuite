/*
  ==============================================================================

    GateDSP.cpp
    Created: 14 Sep 2026 1:51:38pm
    Author:  Анастасия

  ==============================================================================
*/

#include "GateDSP.h"
#include <cmath>
float GateDSP::processSample(float inputSample)
{
    // 1. Визначаємо ціль: 0.0 (тиша), якщо сигнал слабкий, або 1.0 (гучно), якщо сильний
    float targetGain = (std::abs(inputSample) < threshold) ? 0.0f : 1.0f;

    // 2. Плавно крутимо нашу віртуальну ручку до цілі
    if (currentGain < targetGain) {
        currentGain += attackRate; // Відкриваємо звук
        if (currentGain > 1.0f) currentGain = 1.0f; // Максимум 100% гучності
    }
    else if (currentGain > targetGain) {
        currentGain -= releaseRate; // Плавно закриваємо звук
        if (currentGain < 0.0f) currentGain = 0.0f; // Мінімум 0% (повна тиша)
    }

    // 3. Застосовуємо розраховану гучність до нашого гітарного сигналу
    return inputSample * currentGain;
}

void GateDSP::prepareSampleRate(float newSampleRate) {
    sampleRate = newSampleRate;

    // Рахуємо, на скільки кроків крутити ручку гучності за один семпл
    // 1 мс для відкриття гейту (швидко, щоб не з'їсти атаку медіатора)
    attackRate = 1.0f / (0.001f * sampleRate);
    // 50 мс для закриття гейту (досить швидко для стакато, але плавно)
    releaseRate = 1.0f / (0.050f * sampleRate);
}