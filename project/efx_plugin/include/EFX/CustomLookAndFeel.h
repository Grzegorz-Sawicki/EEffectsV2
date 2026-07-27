#pragma once

namespace efx {
class CustomLookAndFeel : public juce::LookAndFeel_V4 {
public:
  CustomLookAndFeel();

  enum class Colors : size_t { background };

  static juce::Colour getColor(Colors colorName);

private:
};
}  // namespace efx
