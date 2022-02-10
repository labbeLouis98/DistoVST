/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class DistoVSTAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    DistoVSTAudioProcessorEditor (DistoVSTAudioProcessor&);
    ~DistoVSTAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:

    void setSliderParams(juce::Slider& slider);

    juce::Slider inputKnob; //input pointer
    juce::Slider driveKnob;
    juce::Slider mixKnob;
    juce::Slider volumeKnob;

    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> inputAttachment;  // input attachement
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> driveAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mixAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> volumeAttachment;
    


    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DistoVSTAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DistoVSTAudioProcessorEditor)
};
