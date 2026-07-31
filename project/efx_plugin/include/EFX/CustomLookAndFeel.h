#pragma once

namespace efx {
class CustomLookAndFeel : public juce::LookAndFeel_V4 {
public:
  CustomLookAndFeel();

  enum class Colors : size_t { background, whiteHighlight, redHighlight };

  static juce::Colour getColor(Colors colorName);

  void drawToggleButton (juce::Graphics& g, juce::ToggleButton& button,
                         bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;

  void drawRotarySlider(juce::Graphics &, int x, int y, int width, int height, float sliderPosProportional,
                        float rotaryStartAngle, float rotaryEndAngle, juce::Slider &) override;

  static juce::FontOptions getInterMediumFont() {
    return interMedium().withPointHeight(12.0f);
  }

private:
  static juce::FontOptions interMedium();
};
}  // namespace efx
