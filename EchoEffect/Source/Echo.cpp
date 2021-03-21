/*
  ==============================================================================

    Echo.cpp
    Created: 5 Mar 2021 8:52:24pm
    Author:  gyxox

  ==============================================================================
*/

#include "Echo.h"
// Constructor
Echo::Echo(){}

float Echo::processSample(float x, int c)
{
	float y = x + (w[c][delayIndex[c]]) * 0.7f;

	// feedforward
	//w[c][index[c]] = x;

	// feedback
	w[c][writeIndex[c]] = y;
	
	delayIndex[c]++; // 인덱스만 변경
	if(delayIndex[c] >= 96000)
	{
		delayIndex[c] = 0;
	}
	
	writeIndex[c]++; // 인덱스만 변경
	if (writeIndex[c] >= 96000)
	{
		delayIndex[c] = 0;
	}
	
	
	return y;
	
}

void Echo::prepare(float newFs)
{
	Fs = newFs;
	delaySamples = round(Fs * delayMS / 1000.f);
	
	
}

void Echo::setDelayMS(float newDelayMS)
{
	if(newDelayMS <= 1000.f)
	{
		delayMS = newDelayMS;
		delaySamples = round(Fs * delayMS / 1000.f);
	}

}

