/*
  ==============================================================================

    Parameters.cpp
    Created: 23 Apr 2025 8:33:16pm
    Author:  Quiet Dimensions

  ==============================================================================
*/

#include "Parameters.h"

template<typename T>
static void castParameter(juce::AudioProcessorValueTreeState &apvts, const juce::ParameterID &id, T &destination)
{
    destination = dynamic_cast<T>(apvts.getParameter(id.getParamID()));
    jassert(destination); // parameter does not exist or wrong type
}

static juce::String stringFromMilliseconds(float value, int)
{
    if (value < 10.0f)
    {
        return juce::String(value, 2) + " ms";
    }
    else if (value < 100.0f)
    {
        return juce::String(value, 1) + " ms";
    }
    else if (value < 1000.0f)
    {
        return juce::String(int(value)) + " ms";
    }
    else
    {
        return juce::String(value * 0.001f, 2) + " s";
    }
}

static juce::String stringFromDecibels(float value, int)
{
    return juce::String(value, 1) + " dB";
}

Parameters::Parameters(juce::AudioProcessorValueTreeState &apvts)
{
    castParameter(apvts, gainParamID, gainParam);
    castParameter(apvts, delayTimeParamID, delayTimeParam);
}

juce::AudioProcessorValueTreeState::ParameterLayout Parameters::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        gainParamID,
        "Output Gain",
        juce::NormalisableRange<float> { -12.0f, 12.0f },
        0.0f,
        juce::AudioParameterFloatAttributes().withStringFromValueFunction(stringFromDecibels)));
    
    //  step size, meaning that the slider will move in steps of 0.001 ms.
    // 0.25 is the skew factor. the smaller this number, the more space the low values get on the slider
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        delayTimeParamID,
        "Delay Time",
        juce::NormalisableRange<float> { minDelayTime, maxDelayTime, 0.001f, 0.25f },
        100.0f,
        juce::AudioParameterFloatAttributes().withStringFromValueFunction(stringFromMilliseconds)));
    return layout;
}

// this reads and stores the current value of our parameters so that the DeeelayAudioProcessor can use them
void Parameters::update() noexcept
{
    // since gainParam is a pointer, we must use -> to call the parameter object's get() function.
    gainSmoother.setTargetValue(juce::Decibels::decibelsToGain(gainParam->get()));
    
    delayTime = delayTimeParam->get();
//    targetDelayTime = delayTimeParam->get();
//    if (delayTime == 0.0f)
//    {
//        delayTime = targetDelayTime;
//    }
}

void Parameters::prepareToPlay(double sampleRate) noexcept
{
    // how long it takes to transition from the previous parameter value to the new one
    // 0.02 seconds equals 20 milliseconds
    double duration = 0.02;
    gainSmoother.reset(sampleRate, duration);
    
//    coeff = 1.0f - std::exp(-1.0f / (0.2f * float(sampleRate)));
}

void Parameters::reset() noexcept
{
    gain = 0.0f;
    
    gainSmoother.setCurrentAndTargetValue(juce::Decibels::decibelsToGain(gainParam->get()));
    
    delayTime = 0.0f;
}

void Parameters::smoothen() noexcept
{
    gain = gainSmoother.getNextValue();
//    delayTime += (targetDelayTime - delayTime) * coeff;
}
