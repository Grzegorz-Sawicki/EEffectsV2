#pragma once

namespace efx {
class EffectEditorBase : public juce::Component {
protected:
  juce::Colour mainColor;
  juce::Font labelFont = CustomLookAndFeel::getInterMediumFont().withPointHeight(10.0f);
  juce::Font logoFont = CustomLookAndFeel::getOrbitronMediumFont().withPointHeight(20.0f);

  const float lineThickness = 2.0f;
};
} //namespace efx