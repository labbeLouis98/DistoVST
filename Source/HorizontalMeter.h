/*
  ==============================================================================

    HorizontalMeter.h
    Created: 24 Feb 2022 11:48:11am
    Author:  Shino

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

namespace Gui
{
    class HorizontalMeter : public Component 
    
    {
    public: 
        void paint(Graphics& g) override
        {
            auto bounds = getLocalBounds().toFloat();

            g.setColour(Colours::blueviolet.brighter(0.4f));
            g.fillRoundedRectangle(bounds, 5.f);

            g.setColour(Colours::black);
            // map level from -60.f / +6f to 0 / width
            const auto scaledY = jmap(level, -60.0f, +6.0f, 0.0f, static_cast<float>(getWidth()));
            
            g.fillRoundedRectangle(bounds.removeFromBottom(scaledY), 5.0f);
        }

        void setLevel(const float value) { level = value; }
    private:
        float level = -60.f;
    };
}