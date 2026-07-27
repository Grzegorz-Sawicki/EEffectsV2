
#include "efx_plugin/include/EFX/CustomLookAndFeel.h"

namespace efx {

CustomLookAndFeel::CustomLookAndFeel() {
  setColour(juce::ResizableWindow::backgroundColourId, getColor(Colors::background));
  //setColour(juce::Label::textColourId, juce::Colours::orange);
}

juce::Colour CustomLookAndFeel::getColor(Colors colorName) {
  static const std::array colors {
    juce::Colour{0xFF2A3135}
  };

  return colors.at(juce::toUnderlyingType(colorName));
}

}  // namespace efx
