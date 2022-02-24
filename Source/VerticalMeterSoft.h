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
    class VerticalMeter : public Component

    {
    public:
        void paint(Graphics& g) override
        {
            auto bounds = getLocalBounds().toFloat();

            g.setColour(Colours::darkgrey.darker(0.5f).withAlpha(0.5f)); //text color);
            g.fillRoundedRectangle(bounds, 2.0f);

            
            g.setColour(Colours::blueviolet.brighter(0.4f));
            // map level from -60.f / +6f to 0 / width

            const auto scaledY = jmap(level, -60.0f, +6.0f, 0.0f, static_cast<float>(getHeight())); 
            
            //scaledY sur laxe des Y pour le vertical

            //g.fillRoundedRectangle(bounds.removeFromTop(scaledY), 2.0f); // from bottom pour avoir un meter vertical
            g.fillRoundedRectangle(bounds.removeFromBottom(scaledY), 2.0f);
            
        }

        void setLevel(const float value) { level = value; }
    private:
        float level = -60.f;
    };
}