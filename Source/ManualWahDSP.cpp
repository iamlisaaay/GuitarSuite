/*
  ==============================================================================

    ManualWahDSP.cpp
    Created: 13 Sep 2026 10:07:54pm
    Author:  Анастасия

  ==============================================================================
*/

#include "ManualWahDSP.h"
#include <cmath>
void ManualWahDSP::prepareSampleRate(float newSampleRate) {
    sampleRate = newSampleRate;
    lowPass = 0;
    bandPass = 0;
 }
float ManualWahDSP::processSample(float inputSample){
    // перетворюємо положення педалі (0.0 - 1.0(350-2400)) на реальну частоту (положення педалі 0 коли п'ятка в підлогу 1 коли носок)
    float freq = 250.0f + (position * 1550.0f);

    // Крок 2: Рахуємо коефіцієнт фільтра (наскільки швидко він реагує)
    float f = 2.0f * std::sin(3.14159f * freq / sampleRate);

    // Крок 3: Гойдалка (перераховуємо змінні)
    float highPass = inputSample - lowPass - (q * bandPass);
    bandPass = bandPass + (f * highPass);
    lowPass = lowPass + (f * bandPass);

    return bandPass;
}