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
    //apvts.state = juce::ValueTree("ParameterSaved");
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

    rmsLevelLeft.reset(sampleRate, 0.5);
    rmsLevelRight.reset(sampleRate, 0.5); // smooth le temps pour que le meter anime entre 2 valeurs

    rmsLevelLeft.setCurrentAndTargetValue(-100.0f); 
    rmsLevelRight.setCurrentAndTargetValue(-100.0f);
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

        // pour chaque sample on distortionne le signal audio
        for (int sample = 0; sample < buffer.getNumSamples(); sample++)

           
        {      // creation dune boucle for

            auto chainSettings = getChainSettings(apvts); // attache la class chainSetting de nos paramêtres

            
            // processing disto


            if (!chainSettings.toggleBypass) // on verifi si le circuit est bypass
            {
                 // le signal audio a l entre du circuit est muliplie par le input gain knob en decibel

                const auto input = *channelData * juce::Decibels::decibelsToGain(chainSettings.inputDB);

                auto const disto = piDivisor * std::atanf(input * juce::Decibels::decibelsToGain(chainSettings.driveDB)) *2; // la disto est egal a 2/ par pi multiplie par la atan du input * la valeur du drive

             
                auto dryWet = chainSettings.mix / 100; // converti une valeur sur 100 en float de 0 a 1

                auto melange = input * (1.0 - dryWet) + disto * dryWet; // le melange dry/wet du signal est egal au input multiplie par 1 - le mix(100%) + la disto multiplie le mix(100%) 

                melange *= juce::Decibels::decibelsToGain(chainSettings.volumeDB); // le volume multipli le signal qui sort su melange dry/wet

                *channelData = melange; //

                channelData++; // increment le point pour que ca pointe vers le prochain channel data



                // rms value calculation

                rmsLevelLeft.skip(buffer.getNumSamples());
                rmsLevelRight.skip(buffer.getNumSamples());

                {
                    const auto value = Decibels::gainToDecibels(buffer.getRMSLevel(0, 0, buffer.getNumSamples())); //rms L
                    if (value < rmsLevelLeft.getCurrentValue())
                        rmsLevelLeft.setTargetValue(value);
                    else
                        rmsLevelLeft.setCurrentAndTargetValue(value);
                }

                {
                    const auto value = Decibels::gainToDecibels(buffer.getRMSLevel(1, 0, buffer.getNumSamples())); //rms R
                    if (value < rmsLevelRight.getCurrentValue())
                        rmsLevelRight.setTargetValue(value);
                    else
                        rmsLevelRight.setCurrentAndTargetValue(value);
                }
            }

            else {

                channelData;

            }

            
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

    juce::MemoryOutputStream mos(destData, true); // save la state du plugin
    apvts.state.writeToStream(mos);

}

void DistoVSTAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.


    auto tree = juce::ValueTree::readFromData(data, sizeInBytes);

    if(tree.isValid()) 
    
    {
        apvts.replaceState(tree);

    }

}


float DistoVSTAudioProcessor::getRmsValue(const int channel) const
{
    jassert(channel == 0 || channel == 1);
    if (channel == 0)
        return rmsLevelLeft.getCurrentValue();

    if (channel == 1)
        return rmsLevelRight.getCurrentValue();
    return 0.0f;
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

  settings.inputDB = apvts.getRawParameterValue("INPUTDB")->load(); // vas chercher la valeur du parametre en temps reel
  settings.driveDB = apvts.getRawParameterValue("DRIVEDB")->load();
  settings.mix = apvts.getRawParameterValue("MIX")->load();
  settings.volumeDB = apvts.getRawParameterValue("VOLUMEDB")->load();
  
  settings.toggleBypass = apvts.getRawParameterValue("BYPASS")->load();
    return settings; //retourne la valeur
}

juce::AudioProcessorValueTreeState::ParameterLayout DistoVSTAudioProcessor::createParameters() {

    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params; // listes des parameters

    // param disto data audio process
    
    // float
    params.push_back(std::make_unique<juce::AudioParameterFloat>("INPUTDB", "InputDB", -48.0f, 48.0f, 0.0f)); 
   

    params.push_back(std::make_unique<juce::AudioParameterFloat>("DRIVEDB", "DriveDB", 0.0f, 24.0f, 0.0f));
    

    params.push_back(std::make_unique<juce::AudioParameterFloat>("MIX", "Mix", 0.0f, 100.0f, 100.0f)); // valeur  sur 100 float en pourcentage
   

    params.push_back(std::make_unique<juce::AudioParameterFloat>("VOLUMEDB", "VolumeDB", -48.0f, 48.0f, 0.0f));
   

    //bool
    params.push_back(std::make_unique<juce::AudioParameterBool>("BYPASS", "BypassDB", false )); // variable bool pour bypass le circuit (false par defaut)

    return { params.begin(), params.end() };

}