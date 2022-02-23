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


    //ImageComponent audioShinobiBrandComponent;

    void setSliderParams(viator_gui::Dial& dial);

   // *" dB" is the text in the dial's number textbox

    viator_gui::Dial driveKnob{ " dB", 0.0, 10.0, 0.1, 0.0 };
    viator_gui::Dial mixKnob{ " %", 0.0, 10.0, 1.0, 0.0 };
    



    void setFaderParams(viator_gui::Fader& fader);
   
    viator_gui::Fader inputFader{ " dB", 0.0, 10.0, 0.1, 0.0 };
    viator_gui::Fader volumeFader{ " dB", 0.0, 10.0, 0.1, 0.0 }; // output

    // toogle

    void setToggle(viator_gui::Toggle& toggle);

    viator_gui::Toggle toggleBypass;
   
    //menu
    viator_gui::Menu menu;

    void setMenu(viator_gui::Menu& menu);




     // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DistoVSTAudioProcessor& audioProcessor; // audioP ref

    public:
    
        
        
    //float
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> inputAttachment;  // input attachement
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> driveAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mixAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> volumeAttachment;

    //bool
    std::unique_ptr < juce::AudioProcessorValueTreeState::ButtonAttachment> bypassAttachment;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DistoVSTAudioProcessorEditor)
};
