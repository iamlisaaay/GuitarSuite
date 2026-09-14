/*
  ==============================================================================

    CompressorDSP.cpp
    Created: 14 Sep 2026 6:41:00pm
    Author:  Анастасия

  ==============================================================================
*/

#include "CompressorDSP.h"
#include <cmath>

float CompressorDSP::processSample(float inputSample)
{

    // Дізнаємося поточну гучність семплу (завжди плюсова)
    float absInput = std::abs(inputSample);

    // Якщо звук різко став гучнішим - використовуємо швидку атаку, якщо затихає - плавний реліз
    if (absInput > envelope) {
        envelope = attack * envelope + (1.0f - attack) * absInput;
    }
    else {
        envelope = release * envelope + (1.0f - release) * absInput;
    }

    // 2. Логіка ручки Amount (Compression)
    // Якщо Amount = 0, поріг високий (1.0), компресор спить.
    // Якщо Amount = 1, поріг низький (0.1), компресор чавить майже весь сигнал.
    float threshold = 1.0f - (amount * 0.9f);

    float gain = 1.0f; // За замовчуванням нічого не стискаємо

    // Якщо наша усереднена гучність пробила поріг
    if (envelope > threshold) {
        // Рахуємо, на скільки саме ми вилізли за ліміт
        float excess = envelope - threshold;

        // Чим більше викручена ручка Amount, тим жорсткіше ми сплющуємо цей надлишок (від 1:1 до 10:1)
        float ratio = 1.0f + (amount * 9.0f);
        float compressedExcess = excess / ratio;

        // Нова цільова гучність для цього семплу
        float targetLevel = threshold + compressedExcess;

        // Розраховуємо множник для стиснення
        gain = targetLevel / envelope;
    }

    // 3. Застосовуємо стиснення та множимо на ручку вихідної гучності (Makeup Gain)
    return inputSample * gain * output;
}
