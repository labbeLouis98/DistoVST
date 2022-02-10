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
                       ), apvts (*this, nullptr, "Parameters", createParameters()) //relie les parametre au apvts et au audioprocessor
#endif

{
    
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

    auto chainSettings = getChainSettings(apvts);

    
}



void DistoVSTAudioProcessor::updateParams(const float input, const float drive, const float mix, const float volume)
{
    

    
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

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        float* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...     
        // 
        //                                   // ou on applique la distortion
        
        // pour chaque sample on distortionne le signal audio
        for (int sample = 0; sample < buffer.getNumSamples(); sample++)

           
        {      // creation dune boucle for

            auto chainSettings = getChainSettings(apvts); // attache la class chainSetting de nos paramÍtres
            
             //-----------------------------------------------------------------------nouvelle version disto-------------------------------------------------//
            
            
                const auto input = *channelData * juce::Decibels::decibelsToGain(chainSettings.inputDB);

                const auto disto = piDivisor * std::atanf(input * juce::Decibels::decibelsToGain(chainSettings.driveDB));

                auto melange = input * (1.0 - chainSettings.mix) + disto * chainSettings.mix;

                melange *= juce::Decibels::decibelsToGain(chainSettings.volumeDB);

                *channelData = melange;

                channelData++; // increment le point pour que ca pointe vers le prochain channel data

        }                                                                  
       
    }
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
    
    
}

void DistoVSTAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DistoVSTAudioProcessor();
}

ChainSettings getChainSettings(juce::AudioProcessorValueTreeState& apvts)
{
    ChainSettings settings;

  settings.inputDB = apvts.getRawParameterValue("INPUTDB")->load();
  settings.driveDB = apvts.getRawParameterValue("DRIVEDB")->load();
  settings.mix = apvts.getRawParameterValue("MIX")->load();
  settings.volumeDB = apvts.getRawParameterValue("VOLUMEDB")->load();

    return settings;
}

juce::AudioProcessorValueTreeState::ParameterLayout DistoVSTAudioProcessor::createParameters() {

    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params; // listes des parameters

    // param disto data audio process
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>("INPUTDB", "InputDB", -48.0f, 48.0f, 0.0f));
   

    params.push_back(std::make_unique<juce::AudioParameterFloat>("DRIVEDB", "DriveDB", 0.0f, 20.0f, 0.0f));
    

    params.push_back(std::make_unique<juce::AudioParameterFloat>("MIX", "Mix", 0.0f, 1.0f, 0.5f));
   

    params.push_back(std::make_unique<juce::AudioParameterFloat>("VOLUMEDB", "VolumeDB", -48.0f, 12.0f, 0.0f));
   

    return { params.begin(), params.end() };

}