
#include "efx_plugin/include/EFX/GUI/CustomLookAndFeel.h"

namespace efx {
CustomLookAndFeel::CustomLookAndFeel() {
  setColour(juce::ResizableWindow::backgroundColourId, getColor(Colors::background));
  setColour(juce::PopupMenu::textColourId, getColor(Colors::tremoloHighlight));
  setColour(juce::PopupMenu::highlightedBackgroundColourId, getColor(Colors::tremoloHighlight));
  setColour(juce::PopupMenu::highlightedTextColourId, getColor(Colors::whiteHighlight));
  setColour(juce::PopupMenu::backgroundColourId, getColor(Colors::grayBackground));
  setColour(juce::ComboBox::textColourId, getColor(Colors::tremoloHighlight));
}

juce::Colour CustomLookAndFeel::getColor(Colors colorName) {
  static const std::array colors{
      juce::Colour{0xFF2A3135},
      juce::Colour{0xFF1E1E1E},
      juce::Colour{0xFFFFFFFF},
      juce::Colour{0xFFFF005E},
      juce::Colour{0xFFDEA11F},
      juce::Colour{0xFF3BB463},
      juce::Colour{0xFF5F78DB},
      juce::Colour{0xFF353535},
      juce::Colour{0xFFD9D9D9},
      juce::Colour{0xFF4A494E}
  };

  return colors.at(juce::toUnderlyingType(colorName));
}

void CustomLookAndFeel::drawToggleButton (juce::Graphics& g, juce::ToggleButton& button,
                       bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) {
  const auto bounds = button.getLocalBounds().toFloat();
  const auto innerBounds = bounds.reduced(2.0f);

  g.setColour(juce::Colour(getColor(Colors::dirtyWhite)));
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

void CustomLookAndFeel::drawButtonBackground(juce::Graphics &g, juce::Button &button, const juce::Colour &backgroundColour,
                                        bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) {
  auto bounds = button.getLocalBounds().toFloat();
  auto cornerRadius = 2.0f;
  auto backgroundColor = getColor(Colors::dirtyWhite);

  g.setColour(backgroundColor);
  g.fillRoundedRectangle(bounds, cornerRadius);

  auto innerBounds = bounds.reduced(1.0f);
  juce::Colour innerColor;

  if (button.getToggleState()) {
    innerColor = button.findColour(custom_colors::highlight, true);
  } else {
    innerColor = getColor(Colors::effectBackground);
  }

  if (shouldDrawButtonAsHighlighted) {
    innerColor = innerColor.withAlpha(0.7f);
  }

  g.setColour(innerColor);
  g.fillRoundedRectangle(innerBounds, cornerRadius);
}

void CustomLookAndFeel::drawButtonText(juce::Graphics &g, juce::TextButton &button, bool shouldDrawButtonAsHighlighted,
                                       bool shouldDrawButtonAsDown) {
  auto fontSize = button.getProperties().getWithDefault("customFontSize", 12.0f);

  auto font = getInterMediumFont().withPointHeight(fontSize);
  g.setFont(font);

  juce::Colour textColor;

  if(button.getToggleState()) {
    textColor = getColor(Colors::effectBackground);
  } else {
    textColor = getColor(Colors::dirtyWhite);
  }

  g.setColour(textColor);

  g.drawFittedText(button.getButtonText(), button.getLocalBounds(), juce::Justification::centred, 1);
}

void CustomLookAndFeel::drawRotarySlider(juce::Graphics &g, int x, int y, int width, int height, float sliderPosProportional,
                                    float rotaryStartAngle, float rotaryEndAngle, juce::Slider &slider) {
  const auto bounds = slider.getLocalBounds().toFloat();

  const float startAngle = -juce::MathConstants<float>::pi * 0.75f;
  const float endAngle = juce::MathConstants<float>::pi * 0.75f;
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
  const auto backgroundReduction = bounds.getWidth() * 0.075f;
  const auto backgroundBounds = bounds.reduced(backgroundReduction);
  g.setColour(juce::Colour(0xFF170A2D));
  g.fillEllipse(backgroundBounds);

  // Inner Border
  const auto innerBorderReduction = bounds.getWidth() * 0.1f;
  const auto innerBorderBounds = backgroundBounds.reduced(innerBorderReduction);
  g.setColour(juce::Colour(0xFF7E7E7E));
  g.fillEllipse(innerBorderBounds);

  // Inner
  const auto innerReduction = bounds.getWidth() * 0.25f;
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
  g.strokePath(indicatorLine, juce::PathStrokeType(0.025f * bounds.getWidth(), juce::PathStrokeType::curved));
}

void CustomLookAndFeel::drawComboBox(juce::Graphics &g,
                                     int,
                                     int,
                                     bool,
                                     int,
                                     int,
                                     int, int,
                                     juce::ComboBox &box) {
  auto bounds = box.getLocalBounds().toFloat();
  auto highlightColor = box.findColour(custom_colors::highlight, true);
  g.setColour(highlightColor);
  g.fillRoundedRectangle(bounds, 2);

  auto buttonBounds = bounds.reduced(2.f);
  g.setColour(getColor(Colors::effectBackground));
  g.fillRoundedRectangle(buttonBounds, 2);

  auto arrowWidth = 8.f;
  auto arrowHeight = 6.f;
  auto arrowX = buttonBounds.getWidth() - 8 - arrowWidth / 2.f;
  auto arrowY = buttonBounds.getCentreY();
  auto arrowBounds = juce::Rectangle<float>{0, 0, arrowWidth, arrowHeight};
  arrowBounds.setCentre(arrowX, arrowY);

  juce::Path arrow;
  arrow.startNewSubPath(arrowBounds.getTopLeft());
  arrow.lineTo(arrowBounds.getTopRight());
  arrow.lineTo(arrowBounds.getCentreX(), arrowBounds.getBottom());
  arrow.closeSubPath();

  g.setColour(highlightColor);
  g.fillPath(arrow);
}

void CustomLookAndFeel::positionComboBoxText(juce::ComboBox &box, juce::Label &label) {
  auto bounds = box.getLocalBounds();
  bounds.removeFromLeft(5);
  bounds.removeFromTop(4);
  bounds.removeFromBottom(4);
  bounds.removeFromRight(10);

  label.setBounds(bounds);
  label.setJustificationType(juce::Justification::left);
  label.setFont(getComboBoxFont(box));
}

juce::PopupMenu::Options CustomLookAndFeel::getOptionsForComboBoxPopupMenu(juce::ComboBox &box, juce::Label &label) {
  const auto menuBounds = box.getScreenBounds().reduced(2, 0);

  return LookAndFeel_V4::getOptionsForComboBoxPopupMenu(box, label)
      .withStandardItemHeight(18)
      .withItemThatMustBeVisible(0)
      .withTargetScreenArea(menuBounds)
      .withMinimumWidth(76);
}

juce::FontOptions CustomLookAndFeel::interMedium() {
  static const auto result = juce::Typeface::createSystemTypefaceFor(assets::InterMedium_ttf,
                                                                     assets::InterMedium_ttfSize);
  return juce::FontOptions(result);
}

juce::FontOptions CustomLookAndFeel::orbitronMedium() {
  static const auto result = juce::Typeface::createSystemTypefaceFor(assets::OrbitronMedium_ttf,
                                                                     assets::OrbitronMedium_ttfSize);
  return juce::FontOptions(result);
}

void CustomLookAndFeel::setupLabel(juce::Label &label, juce::Font &font, juce::Colour &mainColor) {
  label.setInterceptsMouseClicks(false, false);
  label.setJustificationType(juce::Justification::centred);
  label.setColour(juce::Label::textColourId, mainColor);
  label.setFont(font);
}

void CustomLookAndFeel::setupSlider(juce::Slider& slider, bool isBipolar, juce::Colour &mainColor) {
  slider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
  slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
  slider.setPopupDisplayEnabled(true, true, nullptr);
  slider.setColour(custom_colors::highlight, mainColor);
  slider.getProperties().set("isBipolar", isBipolar);
}

void CustomLookAndFeel::setupActiveButton(juce::TextButton &activeButton, juce::Colour &mainColor) {
  activeButton.setClickingTogglesState(true);
  activeButton.setColour(custom_colors::highlight, mainColor);

  activeButton.onClick = [&activeButton]() {
    activeButton.setButtonText(activeButton.getToggleState() ? "ON" : "OFF");
  };
  activeButton.onClick();
}

void CustomLookAndFeel::setupComboBox(juce::ComboBox &comboBox, juce::Colour &mainColor) {
  comboBox.setColour(custom_colors::highlight, mainColor);
}

}  // namespace efx
