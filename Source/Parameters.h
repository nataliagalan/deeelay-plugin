/*
  ==============================================================================

    Parameters.h
    Created: 23 Apr 2025 8:33:16pm
    Author:  Quiet Dimensions

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

const juce::ParameterID gainParamID { "gain", 1 };
const juce::ParameterID delayTimeParamID { "delayTime", 1 };

class Parameters
{
    public:
        // The constructor that is used to initialize a new Parameters object.
        // It has one argument, a reference or address to the APVTS.
        Parameters(juce::AudioProcessorValueTreeState &apvts);
        static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
        // this allows us to write Parameters::maxDelayTime instead of a hardcoded number
        // static means that it does not belong to any particular instance of Parameters but to the class itself
        static constexpr float minDelayTime = 5.0f;
        static constexpr float maxDelayTime = 5000.0f;
        
        void prepareToPlay(double sampleRate) noexcept;
        void update() noexcept;
        void reset() noexcept;
        void smoothen() noexcept;
    
        float gain = 0.0f;
        float delayTime = 0.0f;
    
    private:
    juce::AudioParameterFloat *gainParam;
    juce::LinearSmoothedValue<float> gainSmoother;
    
    juce::AudioParameterFloat *delayTimeParam;
    
//    float targetDelayTime = 0.0f;
//    float coeff = 0.0f; // one-pole smoothing, determines how fast the smoothing happens
};
