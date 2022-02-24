/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

struct ChainSettings
{
    //float
    float inputDB{ 0 };
    float driveDB{ 0 };
    float mix{ 0 };
    float volumeDB{ 0 };
    // bool
    bool toggleBypass{ false };

};

ChainSettings getChainSettings(juce::AudioProcessorValueTreeState& apvts);


//==============================================================================
/**
*/
class DistoVSTAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    DistoVSTAudioProcessor();
    ~DistoVSTAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //juce::AudioProcessorValueTreeState& getState();

    

    juce::AudioProcessorValueTreeState apvts; //creation de l objet tree state

    void updateParams(const float input, const float drive, const float mix, const float volume);


    float getRmsValue(const int channel) const; 

    

private:


    juce::AudioProcessorValueTreeState::ParameterLayout createParameters();


   const float piDivisor = 2.f / juce::MathConstants<float>::pi; // constante piDivisor pour simplifier la formule de distortion , 2 divise pi 

    
   LinearSmoothedValue<float> rmsLevelLeft, rmsLevelRight; // smooth la valeur des valeurs rms



   // Bypass Toggle
   
   //
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DistoVSTAudioProcessor)
};

