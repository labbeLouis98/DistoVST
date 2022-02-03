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

    juce::ScopedPointer<juce::Slider> driveKnob;
    juce::ScopedPointer<juce::Slider> rangeKnob;
    juce::ScopedPointer<juce::Slider> mixKnob;
    juce::ScopedPointer<juce::Slider> volumeKnob;

    juce::ScopedPointer<juce::AudioProcessorValueTreeState::SliderAttachment> driveAttachment;
    juce::ScopedPointer<juce::AudioProcessorValueTreeState::SliderAttachment> rangeAttachment;
    juce::ScopedPointer<juce::AudioProcessorValueTreeState::SliderAttachment> mixAttachment;
    juce::ScopedPointer<juce::AudioProcessorValueTreeState::SliderAttachment> volumeAttachment;



    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DistoVSTAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DistoVSTAudioProcessorEditor)
};
