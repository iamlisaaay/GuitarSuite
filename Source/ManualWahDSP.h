/*
  ==============================================================================

    ManualWahDSP.h
    Created: 13 Sep 2026 10:07:54pm
    Author:  Анастасия

  ==============================================================================
*/

#pragma once
class ManualWahDSP
{
public: 
	ManualWahDSP() = default;
	~ManualWahDSP() = default;

    //секвенсор приймає нову частоту відкритого проєкту
    void prepareSampleRate(float newSampleRate);
    // прийом нових параметрів
	void setQ(float newQ) { q = newQ; }
	void setPosition(float newPosition) { position = newPosition; }

    //обробка аідуо
	float processSample(float inputSample);
private:
    float sampleRate = 44100.0f;
    float lowPass = 0.0f;
    float bandPass = 0.0f;

    // Параметри
    float q = 0.05f;          // Резонанс
    float position = 0.0f;    // Положення педалі (від 0.0 до 1.0)
};