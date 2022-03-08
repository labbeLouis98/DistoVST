/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "VerticalMeterSoft.h"
//#include "Component/HorizontalMeter.h"


//==============================================================================
/**
*/
class DistoVSTAudioProcessorEditor  : public juce::AudioProcessorEditor, public Timer
{
public:
    DistoVSTAudioProcessorEditor (DistoVSTAudioProcessor&);
    ~DistoVSTAudioProcessorEditor() override;


    void timerCallback() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    
   
    Label inputLabel{ "Input" }; //input text name
    Label driveLabel{ "Drive" };
    Label mixLabel{ "Mix" };
    Label volumeLabel{ "Volume" };

   //sliders
    void setSliderParams(viator_gui::Dial& dial);

   // *" dB" is the text in the dial's number textbox

    viator_gui::Dial driveKnob{ " dB", 0.0, 10.0, 0.1, 0.0 };
    viator_gui::Dial mixKnob{ " %", 0.0, 10.0, 1.0, 0.0 };
    


    //faders
    void setFaderParams(viator_gui::Fader& fader);
   
    viator_gui::Fader inputFader{ " dB", 0.0, 10.0, 0.1, 0.0 };
    viator_gui::Fader volumeFader{ " dB", 0.0, 10.0, 0.1, 0.0 }; // output




    // toogle

    void setToggle(viator_gui::Toggle& toggle);

    viator_gui::Toggle toggleBypass;
   



    //menu
    viator_gui::Menu menu;

    void setMenu(viator_gui::Menu& menu);

  


    DistoVSTAudioProcessor& audioProcessor; // audioP ref


    Gui::VerticalMeter verticalMeterL, verticalMeterR; // verticals



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
