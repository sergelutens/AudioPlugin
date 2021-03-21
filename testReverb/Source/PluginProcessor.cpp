/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AlgoReverbAudioProcessor::AlgoReverbAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", AudioChannelSet::stereo(), true)
#endif
    )
#endif
{
}

AlgoReverbAudioProcessor::~AlgoReverbAudioProcessor()
{
}

//==============================================================================
const String AlgoReverbAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AlgoReverbAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool AlgoReverbAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool AlgoReverbAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double AlgoReverbAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AlgoReverbAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AlgoReverbAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AlgoReverbAudioProcessor::setCurrentProgram(int index)
{
}

const String AlgoReverbAudioProcessor::getProgramName(int index)
{
    return {};
}

void AlgoReverbAudioProcessor::changeProgramName(int index, const String& newName)
{
}

//==============================================================================
void AlgoReverbAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{

    //플레이 전 샘플레이트 설정
    predelay.setFs(sampleRate);

    //딜레이 블록은 지연을 줄이거나 늘리는 것이 목적이기에 
    //초기값은 0.0f의 값으로 시작하는 것이 바람직함
    //정말 조심하고 싶다면 생성자에서 선언해도 됨
    predelay.setDelaySamples(0.0f);

	//아래는 슈뢰더 클래스에서 값을 주었으므로 주석처리
    //FDN Fs 설정
    //fdn.setFs(sampleRate);
    //APF Fs 설정
    //apf1.setFs(sampleRate);
    //apf2.setFs(sampleRate);

    tank.setFs(sampleRate);
    lowpassFilter.setFs(sampleRate);
    Fs = sampleRate; //정적 변수
}

void AlgoReverbAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AlgoReverbAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}
#endif

void AlgoReverbAudioProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();


    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    predelay.setDepth(0.0f);
    predelay.setSpeed(0.0f);

    // 밀리초를 초로 변경 (Editor에서 선언한 값이 밀리초이기 때문에, 샘플 연산을 위해 변경)
    // 어떤 단위를 지울 것인가를 분모로 둠. n sec / 1000ms 
    float predelaySec = predelayMS * 0.001;

    // 초에서 샘플로 변경
    // Fs samples / 1 sec
    float predelaySamples = predelaySec * Fs;

    // 딜레이 샘플 지정
    predelay.setDelaySamples(predelaySamples);

    tank.setFeedbackGain(timeValue);
    tank.setDiffusionGain(diffusionValue);
    tank.setDepth(modValue);
    lowpassFilter.setFrequency(freqValue);
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        for (int n = 0; n < buffer.getNumSamples(); ++n)
        {
            //auto* channelData = buffer.getWritePointer(channel);
             //기본은 위와 같지만 전체 버퍼를 얻고자 하는 것이 아니므로 아래와 같이 변경

             //n 이라는 샘플을 하나 input으로 받음
            float x = buffer.getWritePointer(channel)[n];

            //위 x값을 FractionalDelay.cpp에서 딜레이 블록을 통과한 딜레이 된 신호 w값으로 가져옴
           //딜레이 신호의 채널을 항상 추적하는 것이 좋다.
            float verb = predelay.processSample(x, channel);
            verb = lowpassFilter.processSample(x, channel);
        	/*
        	슈뢰더 클래스에 포함되므로 주석처리
            verb = fdn.processSample(verb, channel);//fdn이 적용된 verb

            // diffusion 설정
            verb = apf1.processSample(verb, channel);//apf1이 적용된 verb
            verb = apf2.processSample(verb, channel);//apf2가 적용된 verb
            */

        	// 슈뢰더로 통합 이후 슬라이더가 잘 설정되었는지 확인해보자.
            verb = tank.processSample(verb, channel);
            
            //아웃풋 y는 원래 신호 x에 1-wet만큼 곱해진 값과, verb에 wet을 곱한 값을 더하여 생성
            float y = (1.f - wet) * x + wet * verb;
            
        	
            //buffer.getWritePointer(channel)[n] = y로 버퍼에 값을 반환
            buffer.getWritePointer(channel)[n] = y;


        }


        // ..do something to the data...
    }
}

//==============================================================================
bool AlgoReverbAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* AlgoReverbAudioProcessor::createEditor()
{
    return new AlgoReverbAudioProcessorEditor(*this);
}

//==============================================================================
void AlgoReverbAudioProcessor::getStateInformation(MemoryBlock& destData)
{
  
  
}

void AlgoReverbAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AlgoReverbAudioProcessor();
}