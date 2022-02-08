/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DistoVSTAudioProcessor::DistoVSTAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    //

    state = new juce::AudioProcessorValueTreeState(*this, nullptr);

    state->createAndAddParameter("inputDB", "Input", "Input", juce::NormalisableRange<float>(0.5f, 1.f, 0.0001), 1.0, nullptr, nullptr);
    state->createAndAddParameter("driveDB", "Drive", "Drive", juce::NormalisableRange<float>(0.f, 5000.f, 0.0001), 1.0, nullptr, nullptr);  //definit la valeur depart (min) , la valeur fin (max) , la valeur intervalle (de cmb on augmente) 
   
    state->createAndAddParameter("mix", "Mix", "Mix", juce::NormalisableRange<float>(0.f, 1.f, 0.0001), 1.0, nullptr, nullptr);
    state->createAndAddParameter("volumeDB", "Volume", "Volume", juce::NormalisableRange<float>(0.01f, 1.f, 0.0001), 1.0, nullptr, nullptr); 


    state->state = juce::ValueTree("inputDB");
    state->state = juce::ValueTree("driveDB"); // le state des data du parametre = valeur du parametre
    
    state->state = juce::ValueTree("mix");
    state->state = juce::ValueTree("volumeDB");
}

DistoVSTAudioProcessor::~DistoVSTAudioProcessor()
{
}

//==============================================================================
const juce::String DistoVSTAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DistoVSTAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DistoVSTAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DistoVSTAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DistoVSTAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DistoVSTAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DistoVSTAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DistoVSTAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String DistoVSTAudioProcessor::getProgramName (int index)
{
    return {};
}

void DistoVSTAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void DistoVSTAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void DistoVSTAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DistoVSTAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void DistoVSTAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

   
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());


    // parametre pour tout les knobs
    
    float inputDB = *state->getRawParameterValue("inputDB");
    float driveDB = *state->getRawParameterValue("driveDB");   //definit la valeur des parametres en lisant c le state des parametres en temps reels
       //prend la valeur du parametre definit
    float mix = *state->getRawParameterValue("mix"); 
    float volumeDB = *state->getRawParameterValue("volumeDB");

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        float* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...                                       // ou on applique la distortion
                                                                               // pour chaque sample on distortionne le signal audio
        for (int sample = 0; sample < buffer.getNumSamples(); sample++) 
        
        {      // creation dune boucle for

            
            
             //-----------------------------------------------------------------------nouvelle version disto-------------------------------------------------//
           
            

                const auto input = *channelData * juce::Decibels::decibelsToGain(inputDB);

                const auto disto = piDivisor * std::atanf(input * juce::Decibels::decibelsToGain(driveDB));

                auto melange = input * (1.0 - (mix/100)) + disto * (mix/100);

                melange *= juce::Decibels::decibelsToGain(volumeDB);

                *channelData = melange;
           
                channelData++; // increment le point pour que ca pointe vers le prochain channel data
            

            //----------------------------------------------------------------------------------premiere version disto----------------------------//


           /* float cleanSig = *channelData; // clean signal avant la distorsion

            *channelData *= driveDB * inputDB;

             *channelData *= (((((piDivisor) * atan(*channelData)) * mix) + (cleanSig * (1.f - mix))) /2.f) * volumeDB; //fonction de disto le signal est multiplier par (2/pi) * atan(x)
             // clean sign avec la disto
            //multipli le mix avec le volume (output)

            const auto disto = ((piDivisor)*atan(*channelData));

             auto leMix = *channelData * (1.0 - mix) + disto * mix; 

             leMix *= *channelData;

           channelData++;    // increment le point pour que ca pointe vers le prochain channel data
            
            */
        }                                                                  
       
    }
}

juce::AudioProcessorValueTreeState& DistoVSTAudioProcessor::getState() {

    return *state;
}

//==============================================================================
bool DistoVSTAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DistoVSTAudioProcessor::createEditor()
{
    return new DistoVSTAudioProcessorEditor (*this);
}

//==============================================================================
void DistoVSTAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.

    juce::MemoryOutputStream stream(destData, false); //ecrit le data dans le buffer intern 
    state->state.writeToStream(stream);               //recupere le state du data pour l ecrire
                                                      //quand le data est ecrit on peut le lire
}

void DistoVSTAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

    juce::ValueTree tree = juce::ValueTree::readFromData(data, sizeInBytes);

    if (tree.isValid()) {                                  //verifi si le data est valide
                                                           //si oui le data peut etre lu
        state->state = tree;   

    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DistoVSTAudioProcessor();
}
