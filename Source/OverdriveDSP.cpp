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
	float currentDrive = drive.getNextValue();
	float processedSample = std::tanh(inputSample * currentDrive);
	processedSample *= volume;
	return processedSample;
}