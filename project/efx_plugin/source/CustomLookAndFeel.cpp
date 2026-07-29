
#include "efx_plugin/include/EFX/CustomLookAndFeel.h"

namespace efx {

CustomLookAndFeel::CustomLookAndFeel() {
  setColour(juce::ResizableWindow::backgroundColourId, getColor(Colors::background));
}

juce::Colour CustomLookAndFeel::getColor(Colors colorName) {
  static const std::array colors {
    juce::Colour{0xFF2A3135},
    juce::Colour{0xFFFFFFFF}
  };

  return colors.at(juce::toUnderlyingType(colorName));
}

void
CustomLookAndFeel::drawRotarySlider(juce::Graphics &g, int x, int y, int width, int height, float sliderPosProportional,
                                    float rotaryStartAngle, float rotaryEndAngle, juce::Slider &slider) {
  const auto bounds = juce::Rectangle<float>{(float) x, (float) y, (float) width, (float) height};

  const float startAngle = -juce::MathConstants<float>::pi * 0.75f; // -135 deg
  const float endAngle   =  juce::MathConstants<float>::pi * 0.75f; // +135 deg
  const float currentAngle = startAngle + sliderPosProportional * (endAngle - startAngle);
  auto center = bounds.getCentre();

  juce::Path pizzaPath;

  pizzaPath.addPieSegment (bounds, startAngle, endAngle, 0.0f);

  g.setColour (juce::Colour(0xFF919191));
  g.fillPath (pizzaPath);

  // Path Indicator
  if (currentAngle > startAngle)
  {
    juce::Path indicatorPath;

    indicatorPath.addPieSegment (bounds, startAngle, currentAngle, 0.0f);

    g.setColour (getColor(Colors::whiteHighlight));
    g.fillPath (indicatorPath);
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

  juce::Point<float> endPoint (center.x + innerBoundsRadius * std::sin (currentAngle),
                               center.y - innerBoundsRadius * std::cos (currentAngle));

  juce::Path indicatorLine;
  indicatorLine.startNewSubPath(center);
  indicatorLine.lineTo(endPoint);

  g.setColour(getColor(Colors::whiteHighlight));
  g.strokePath(indicatorLine, juce::PathStrokeType(1.0f, juce::PathStrokeType::curved));
}

juce::FontOptions CustomLookAndFeel::interMedium() {
  static const auto result = juce::Typeface::createSystemTypefaceFor(assets::InterMedium_ttf,
                                                                     assets::InterMedium_ttfSize);
  return juce::FontOptions(result);
}

}  // namespace efx
