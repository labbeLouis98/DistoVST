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

    addAndMakeVisible(inputKnob = new juce::Slider("InputDB"));
    inputKnob->setSliderStyle(juce::Slider::Rotary);
    inputKnob->setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 100);  //input knob style

    addAndMakeVisible(driveKnob = new juce::Slider("DriveDB"));
    driveKnob->setSliderStyle(juce::Slider::Rotary);
    driveKnob->setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 100);

    addAndMakeVisible(mixKnob = new juce::Slider("Mix"));
    mixKnob->setSliderStyle(juce::Slider::Rotary);
    mixKnob->setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 100);

    addAndMakeVisible(volumeKnob = new juce::Slider("VolumeDB")); // on cree un knob 
    volumeKnob->setSliderStyle(juce::Slider::Rotary); // son style
    volumeKnob->setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 100); // la taille du text box

    inputAttachment = new juce::AudioProcessorValueTreeState::SliderAttachment(p.getState(), "inputDB", *inputKnob);
    driveAttachment = new juce::AudioProcessorValueTreeState::SliderAttachment(p.getState(), "driveDB", *driveKnob); // on attache le parametre audio du drive au knob du drive
    
    mixAttachment = new juce::AudioProcessorValueTreeState::SliderAttachment(p.getState(), "mix", *mixKnob);
    volumeAttachment = new juce::AudioProcessorValueTreeState::SliderAttachment(p.getState(), "volumeDB", *volumeKnob);

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
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
   // g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);

    g.drawText("InputDB", ((getWidth() / 5) * 1) - (100 / 2), (getHeight() / 2) + 5, 100, 100, juce::Justification::centred, false);  // ecrit un texte a lemplacement design
    g.drawText("DriveDB", ((getWidth() / 5) * 2) - (100 / 2), (getHeight() / 2) + 5, 100, 100, juce::Justification::centred, false);
    g.drawText("Mix", ((getWidth() / 5) * 3) - (100 / 2), (getHeight() / 2) + 5, 100, 100, juce::Justification::centred, false);
    g.drawText("VolumeDB", ((getWidth() / 5) * 4) - (100 / 2), (getHeight() / 2) + 5, 100, 100, juce::Justification::centred, false);
}

void DistoVSTAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    inputKnob->setBounds(((getWidth() / 5) * 1) - (100 / 2), (getHeight() / 2) - (100 / 2), 100, 100);
    driveKnob->setBounds(((getWidth() / 5) * 2) - (100 / 2), (getHeight() / 2) - (100 / 2), 100, 100);  // on divise lespace pour que chaque knob soient centre
     
    mixKnob->setBounds(((getWidth() / 5) * 3) - (100 / 2), (getHeight() / 2) - (100 / 2), 100, 100);
    volumeKnob->setBounds(((getWidth() / 5) * 4) - (100 / 2), (getHeight() / 2) - (100 / 2), 100, 100);

}
