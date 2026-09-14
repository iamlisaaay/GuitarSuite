/*
  ==============================================================================

    GateDSP.h
    Created: 14 Sep 2026 1:51:38pm
    Author:  Анастасия

  ==============================================================================
*/
#pragma once
class GateDSP
{
public:
	GateDSP() = default;
	~GateDSP() = default;
	//секвенсор приймає нову частоту відкритого проєкту
	void prepareSampleRate(float newSampleRate);
	// прийом нових параметрів
	void setThreshold(float newThreshold) { threshold = newThreshold; }
	//обробка аідуо
	float processSample(float inputSample);

   
private:
	float sampleRate = 44100.0f;
	float threshold = 0.1f; // Поріг гейту

	float currentGain = 1.0f;
	float attackRate = 0.0f;
	float releaseRate = 0.0f;
};
