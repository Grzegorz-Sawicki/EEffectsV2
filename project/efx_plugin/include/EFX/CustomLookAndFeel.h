#pragma once

namespace efx {
namespace custom_colors {
  const int highlight = 0x12345678;
}

class CustomLookAndFeel : public juce::LookAndFeel_V4 {
public:
  CustomLookAndFeel();

  enum class Colors : size_t { background, effectBackground, whiteHighlight, redHighlight, tremoloHighlight, flangerHighlight, filterHighlight };

  static juce::Colour getColor(Colors colorName);

  void drawToggleButton (juce::Graphics& g, juce::ToggleButton& button,
                         bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;

  void drawRotarySlider(juce::Graphics &, int x, int y, int width, int height, float sliderPosProportional,
                        float rotaryStartAngle, float rotaryEndAngle, juce::Slider &) override;

  juce::Font getLabelFont (juce::Label&) override {
    return interMedium().withPointHeight(12.0f);
  }

  static juce::FontOptions getInterMediumFont() {
    return interMedium().withPointHeight(12.0f);
  }

  static juce::FontOptions getOrbitronMediumFont() {
    return orbitronMedium().withPointHeight(20.0f);
  }

private:
  static juce::FontOptions interMedium();
  static juce::FontOptions orbitronMedium();
};
}  // namespace efx
