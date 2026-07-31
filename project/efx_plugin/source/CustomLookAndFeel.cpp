
#include "efx_plugin/include/EFX/CustomLookAndFeel.h"

namespace efx {
CustomLookAndFeel::CustomLookAndFeel() {
  setColour(juce::ResizableWindow::backgroundColourId, getColor(Colors::background));
}

juce::Colour CustomLookAndFeel::getColor(Colors colorName) {
  static const std::array colors{
      juce::Colour{0xFF2A3135},
      juce::Colour{0xFFFFFFFF},
      juce::Colour{0xFFFF005E}
  };

  return colors.at(juce::toUnderlyingType(colorName));
}

void CustomLookAndFeel::drawToggleButton (juce::Graphics& g, juce::ToggleButton& button,
                       bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) {
  const auto bounds = button.getLocalBounds().toFloat();
  const auto innerBounds = bounds.reduced(2.0f);

  g.setColour(juce::Colour(0xFFD9D9D9));
  g.fillRect(bounds);

  juce::Colour fillColor;

  if (button.getToggleState()) {
    fillColor = getColor(Colors::redHighlight);
  } else {
    fillColor = getColor(Colors::background);
  }

  if (shouldDrawButtonAsHighlighted) {
    fillColor = fillColor.withAlpha(0.7f);
  }

  g.setColour(fillColor);
  g.fillRect(innerBounds);
}

void
CustomLookAndFeel::drawRotarySlider(juce::Graphics &g, int x, int y, int width, int height, float sliderPosProportional,
                                    float rotaryStartAngle, float rotaryEndAngle, juce::Slider &slider) {
  const auto bounds = slider.getLocalBounds().toFloat();

  const float startAngle = -juce::MathConstants<float>::pi * 0.75f; // -135 deg
  const float endAngle = juce::MathConstants<float>::pi * 0.75f; // +135 deg
  const float currentAngle = startAngle + sliderPosProportional * (endAngle - startAngle);
  auto center = bounds.getCentre();

  // Outer
  juce::Path outerPath;
  outerPath.addPieSegment(bounds, startAngle, endAngle, 0.0f);
  g.setColour(juce::Colour(0xFF919191));
  g.fillPath(outerPath);

  // Path Indicator
  bool isBipolar = slider.getProperties().getWithDefault("isBipolar", false);
  juce::Colour ringColor = slider.findColour(custom_colors::highlight, true);

  if (isBipolar) {
    const float centerAngle = 0.0f;

    if (!juce::approximatelyEqual(currentAngle, centerAngle)) {
      juce::Path indicatorPath;

      float arcStart = std::min(centerAngle, currentAngle);
      float arcEnd = std::max(centerAngle, currentAngle);

      indicatorPath.addPieSegment(bounds, arcStart, arcEnd, 0.0f);
      g.setColour(ringColor);
      g.fillPath(indicatorPath);
    }
  } else {
    if (currentAngle > startAngle) {
      juce::Path indicatorPath;
      indicatorPath.addPieSegment(bounds, startAngle, currentAngle, 0.0f);
      g.setColour(ringColor);
      g.fillPath(indicatorPath);
    }
  }

  // Background
  const auto backgroundBounds = bounds.reduced(3.0f);
  g.setColour(juce::Colour(0xFF170A2D));
  g.fillEllipse(backgroundBounds);

  // Inner Border
  const auto innerBorderBounds = backgroundBounds.reduced(4.0f);
  g.setColour(juce::Colour(0xFF7E7E7E));
  g.fillEllipse(innerBorderBounds);

  // Inner
  const auto innerBounds = innerBorderBounds.reduced(1.0f);
  g.setColour(juce::Colour(0xFF4A494E));
  g.fillEllipse(innerBounds);

  // Line Indicator
  const auto innerBoundsRadius = innerBounds.getWidth() * 0.5f;
  juce::Point<float> endPoint(center.x + innerBoundsRadius * std::sin(currentAngle),
                              center.y - innerBoundsRadius * std::cos(currentAngle));
  juce::Path indicatorLine;
  indicatorLine.startNewSubPath(center);
  indicatorLine.lineTo(endPoint);
  g.setColour(ringColor);
  g.strokePath(indicatorLine, juce::PathStrokeType(1.0f, juce::PathStrokeType::curved));
}

juce::FontOptions CustomLookAndFeel::interMedium() {
  static const auto result = juce::Typeface::createSystemTypefaceFor(assets::InterMedium_ttf,
                                                                     assets::InterMedium_ttfSize);
  return juce::FontOptions(result);
}

}  // namespace efx
