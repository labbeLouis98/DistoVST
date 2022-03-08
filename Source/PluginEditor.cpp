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
   
    
    setFaderParams(inputFader); // reference a la construction dun slider (void en bas)
    setSliderParams(driveKnob);
    setSliderParams(mixKnob);
    setFaderParams(volumeFader);

    //button
    setToggle(toggleBypass);

    setMenu(menu);

    // float
    inputAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "INPUTDB", inputFader); //reference le data du processor au knob 
    driveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "DRIVEDB", driveKnob); //alocate la memoire de notre slider attachement
    mixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "MIX", mixKnob);
    volumeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "VOLUMEDB", volumeFader);

    // bool 

    bypassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "BYPASS", toggleBypass);

    

    setSize (1100, 740); //taille du plugin

    addAndMakeVisible(verticalMeterL); // rendre les meter visible
    addAndMakeVisible(verticalMeterR);

    //labels pour mes params
    //style pour les text
    //attachement au slider
    
    //input
    inputLabel.setText("Input", juce::dontSendNotification);
    inputLabel.attachToComponent(&inputFader,false);
    inputLabel.setFont(18.0f);
    inputLabel.setColour(juce::Label::ColourIds::textColourId,juce::Colours::darkgrey.darker(0.5f));
    inputLabel.setJustificationType(Justification::centred);

    //Drive
    driveLabel.setText("Drive", juce::dontSendNotification);
    driveLabel.attachToComponent(&driveKnob, false);
    driveLabel.setFont(18.0f);
    driveLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::darkgrey.darker(0.5f));
    driveLabel.setJustificationType(Justification::centred);

    // Mix
    mixLabel.setText("Mix", juce::dontSendNotification);
    mixLabel.attachToComponent(&mixKnob, false);
    mixLabel.setFont(18.0f);
    mixLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::darkgrey.darker(0.5f));
    mixLabel.setJustificationType(Justification::centred);

    //Output Volume
    volumeLabel.setText("Output", juce::dontSendNotification);
    volumeLabel.attachToComponent(&volumeFader, false);
    volumeLabel.setFont(18.0f);
    volumeLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::darkgrey.darker(0.5f));
    volumeLabel.setJustificationType(Justification::centred);

    // startTimer de lanimation
    startTimerHz(30);

    

    
}

DistoVSTAudioProcessorEditor::~DistoVSTAudioProcessorEditor()
{
}



void DistoVSTAudioProcessorEditor::timerCallback()
{
    verticalMeterL.setLevel(audioProcessor.getRmsValue(0)); //fetch rms value sur chaques channels L et R
    verticalMeterR.setLevel(audioProcessor.getRmsValue(1));

    verticalMeterL.repaint();
    verticalMeterR.repaint();
}

//==============================================================================
void DistoVSTAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)

    g.setColour (juce::Colours::black);
    g.setFont (15.0f);

   
    
    auto backgroundImage = juce::ImageCache::getFromMemory(BinaryData::vstBackground2_png , BinaryData::vstBackground2_pngSize); //vas chercher les data de l'image
    g.drawImageAt(backgroundImage, 0, 0);
    

}

void DistoVSTAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    // on divise lespace pour que chaque knob soient centre

    //flex rules

    /*------------------------------------------------*/
    // header
    
    Rectangle<int> bounds = getLocalBounds();

    //row header
    FlexBox flexboxHead; //item
    flexboxHead.flexDirection = FlexBox::Direction::row;
    flexboxHead.flexWrap = FlexBox::Wrap::noWrap;
    flexboxHead.alignContent = FlexBox::AlignContent::center;
    flexboxHead.justifyContent = juce::FlexBox::JustifyContent::spaceBetween;

    // ajoute des items dans le tableau du conteneur
    Array<FlexItem> itemArrayHead;
    itemArrayHead.add(FlexItem(80,80,toggleBypass).withMargin(20));
    //itemArrayHead.add(FlexItem(80, 80, menu).withMargin(20));

    flexboxHead.items = itemArrayHead;
    flexboxHead.performLayout(bounds.removeFromTop((bounds.getHeight() / 3) - 90));
   
    /*------------------------------------------------*/
    
    FlexBox flexbox; //item
    flexbox.flexDirection = FlexBox::Direction::row;
    flexbox.flexWrap = FlexBox::Wrap::wrap;
    flexbox.alignContent = FlexBox::AlignContent::center;
    flexbox.justifyContent = juce::FlexBox::JustifyContent::spaceAround;
    
    //flexbox.alignItems = FlexBox::AlignItems::center;
    
    // array ou on met les items
    Array<FlexItem> itemArray;
    itemArray.add(FlexItem(90, 240, inputFader));
    itemArray.add(FlexItem(50, 50, verticalMeterL)); // position des meter et leurs tailles
    itemArray.add(FlexItem(50, 50, verticalMeterR));
    itemArray.add(FlexItem(90, 240, volumeFader));
 

    flexbox.items = itemArray;
    flexbox.performLayout(bounds.removeFromTop((bounds.getHeight() / 2) -10));


    /*------------------------------------------------*/
    //flexbox 2
    
    FlexBox flexbox2; //item
    flexbox2.flexDirection = FlexBox::Direction::row;
    flexbox2.flexWrap = FlexBox::Wrap::wrap;
    flexbox2.alignContent = FlexBox::AlignContent::center;
    flexbox2.justifyContent = juce::FlexBox::JustifyContent::center;
    //flexbox2.alignItems = FlexBox::AlignItems::center;

    // array ou on met les items
    Array<FlexItem> itemArray2;

    itemArray2.add(FlexItem(200, 200, driveKnob));
    itemArray2.add(FlexItem(200, 200, mixKnob));

    flexbox2.items = itemArray2;
    flexbox2.performLayout(bounds.removeFromTop(bounds.getHeight()));
    //layout
    /*------------------------------------------------*/
    
}


// classses pour mes components dial, fader, bouton etc...

void DistoVSTAudioProcessorEditor::setSliderParams(viator_gui::Dial& dial)   // fonction qui definit un slider 

                                                                           // evite de repetter le code pour crer plusieurs slider
{
   
    
    addAndMakeVisible(dial);

    //color override la class predefinies des knobs
   

    dial.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, findColour(Slider::ColourIds::thumbColourId).brighter(0.2f)); 
    dial.setColour(juce::Slider::ColourIds::rotarySliderOutlineColourId, juce::Colours::black.withAlpha(0.1f)); // affect the fill & outline 
    dial.setColour(juce::Slider::ColourIds::trackColourId, Colours::blueviolet.withAlpha(0.5f)); // tick and dot
    dial.setColour(juce::Slider::ColourIds::textBoxTextColourId, juce::Colours::darkgrey.darker(0.5f)); //text color
    dial.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colour::fromFloatRGBA(0.101f, 0.200f, 0.208f, 0.8f).darker(0.8f)); // thumb knob color

    dial.setDialStyle(viator_gui::Dial::DialStyle::kFullDial);
    dial.forceShadow();
    
 

    // theme rgb colors 
     //purple : 226,205,254
    // blue : 101, 200, 208
    // yellow : 247, 240 , 133

}

void DistoVSTAudioProcessorEditor::setFaderParams(viator_gui::Fader& fader)   // fonction qui definit un fader 

                                                                           // evite de repetter le code pour crer plusieurs slider
{
    addAndMakeVisible(fader);
    fader.setColour(juce::Slider::ColourIds::textBoxTextColourId, juce::Colours::darkgrey.darker(0.5f)); //text color
    fader.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::blueviolet.brighter(0.5f));

    
    
   
    fader.forceShadow();
    
}
//toggle

void DistoVSTAudioProcessorEditor::setToggle(viator_gui::Toggle& toggle){

    addAndMakeVisible(toggle);
    

    toggle.setColour(juce::ToggleButton::tickColourId, juce::Colours::blueviolet.brighter(0.5f));

    
    toggle.setToggleStyle(viator_gui::Toggle::ToggleStyle::kPower);

   
}


//menu


void DistoVSTAudioProcessorEditor::setMenu(viator_gui::Menu& menu) 
{
    addAndMakeVisible(menu);
}
