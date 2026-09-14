/*
  ==============================================================================

    OverdriveDSP.cpp
    Created: 14 Sep 2026 6:23:17pm
    Author:  Анастасия

  ==============================================================================
*/

#include "OverdriveDSP.h"
#include <cmath>
float OverdriveDSP::processSample(float inputSample) {
	
	float processedSample = std::tanh(inputSample * drive);
	processedSample *= volume;
	return processedSample;
}