/*
  ==============================================================================

    Echo.h
    Created: 5 Mar 2021 8:52:24pm
    Author:  gyxox

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>



class Echo
{	
public:
    Echo();


    float processSample(float x, int c);   
    void prepare(float newFs);
    void setDelayMS(float newDelayMS);

private:

    
	
    float Fs = 48000;
    float delayMS = 500.f;
    int delaySamples = round(Fs*delayMS/1000.f); // number of Sample delay

    float w[2][96000] = {0.f};
    int writeIndex[2] = { 47999 };
	int delayIndex[2] = {47999-delaySamples}; // array index 인덱스도 배열로 만들어야 함
};