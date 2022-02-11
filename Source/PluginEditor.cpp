/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DistoVSTAudioProcessorEditor::DistoVSTAudioProcessorEditor (DistoVSTAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{

    setSliderParams(inputKnob); // reference a la construction dun slider (void en bas)
    setSliderParams(driveKnob);
    setSliderParams(mixKnob);
    setSliderParams(volumeKnob);

    
    inputAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "INPUTDB", inputKnob); //reference le data du processor au knob 
    driveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "DRIVEDB", driveKnob); //alocate la memoire de notre slider attachement
    mixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "MIX", mixKnob);
    volumeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "VOLUMEDB", volumeKnob);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (500, 200);
}

DistoVSTAudioProcessorEditor::~DistoVSTAudioProcessorEditor()
{
}

//==============================================================================
void DistoVSTAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)

    g.fillAll (juce::Colours::black);

    g.setColour (juce::Colours::red);
    g.setFont (15.0f);

    g.drawText("InputDB", ((getWidth() / 5) * 1) - (100 / 2), (getHeight() / 2) + 5, 100, 100, juce::Justification::centred, false);  // ecrit un texte a lemplacement design
    g.drawText("DriveDB", ((getWidth() / 5) * 2) - (100 / 2), (getHeight() / 2) + 5, 100, 100, juce::Justification::centred, false);
    g.drawText("Mix", ((getWidth() / 5) * 3) - (100 / 2), (getHeight() / 2) + 5, 100, 100, juce::Justification::centred, false);
    g.drawText("VolumeDB", ((getWidth() / 5) * 4) - (100 / 2), (getHeight() / 2) + 5, 100, 100, juce::Justification::centred, false);
}

void DistoVSTAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    // on divise lespace pour que chaque knob soient centre
    inputKnob.setBounds(((getWidth() / 5) * 1) - (100 / 2), (getHeight() / 2) - (100 / 2), 100, 100);
    driveKnob.setBounds(((getWidth() / 5) * 2) - (100 / 2), (getHeight() / 2) - (100 / 2), 100, 100);  
     
    mixKnob.setBounds(((getWidth() / 5) * 3) - (100 / 2), (getHeight() / 2) - (100 / 2), 100, 100);
    volumeKnob.setBounds(((getWidth() / 5) * 4) - (100 / 2), (getHeight() / 2) - (100 / 2), 100, 100);

}

void DistoVSTAudioProcessorEditor::setSliderParams(juce::Slider& slider)   // fonction qui definit un slider 

                                                                           // evite de repetter le code pour crer plusieurs slider
{
    slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 50, 50);
    
    addAndMakeVisible(slider);

    
}