#pragma once

namespace efx {
class EffectRackItem : public juce::Component {
public:
  EffectRackItem(juce::String effectName) :
      name(std::move(effectName)),
      background(CustomLookAndFeel::getColor(CustomLookAndFeel::Colors::grey)),
      whiteBackground(CustomLookAndFeel::getColor(CustomLookAndFeel::Colors::whiteHighlight)) {
    addAndMakeVisible(whiteBackground);
    addAndMakeVisible(background);

    label.setText(name, juce::dontSendNotification);
    label.setInterceptsMouseClicks(false, false);
    addAndMakeVisible(label);

    button.getProperties().set("customFontSize", 9.0f);
    button.setClickingTogglesState(true);
    button.onClick = [this]() {
      button.setButtonText(button.getToggleState() ? "ON" : "OFF");
    };
    button.onClick();
    setMainColor(mainColor);
    addAndMakeVisible(button);

    addMouseListener(this, true);
    updateHoverVisuals();
  }

  void resized() override {
    auto bounds = getLocalBounds();

    whiteBackground.setBounds(bounds);
    background.setBounds(bounds);

    button.setBounds(88, 3, 22, 14);
    label.setBounds(3, 3, 82, 14);
  }

  void paintOverChildren(juce::Graphics &g) override {
    if (isSelected) {
      g.setColour(mainColor);

      g.drawRect(getLocalBounds(), 1);
    }
  }

  void setSelected(bool shouldBeSelected) {
    if (isSelected != shouldBeSelected) {
      isSelected = shouldBeSelected;
      repaint();
    }
  }

  void mouseUp(const juce::MouseEvent &event) override {
    if (event.eventComponent != &button) {
      if (onSelect)
        onSelect(name);
    }
  }

  void mouseEnter(const juce::MouseEvent &) override { updateHoverVisuals(); }

  void mouseExit(const juce::MouseEvent &) override { updateHoverVisuals(); }

  void setMainColor(juce::Colour color) {
    mainColor = color;
    label.setColour(juce::Label::textColourId, mainColor);
    button.setColour(custom_colors::highlight, mainColor);
  }

  std::function<void(juce::String)> onSelect;

  Background whiteBackground;
  Background background;
  juce::TextButton button;

  juce::Colour mainColor = CustomLookAndFeel::getColor(CustomLookAndFeel::Colors::redHighlight);

protected:
  void updateHoverVisuals() {
    bool currentlyHovered = isMouseOver(true);

    if (isHovered != currentlyHovered) {
      isHovered = currentlyHovered;

      float childAlpha = isHovered ? 0.85f : 1.0f;
      background.setAlpha(childAlpha);
      label.setAlpha(childAlpha);
      button.setAlpha(childAlpha);

      repaint();
    }
  }

  bool isHovered = false;
  bool isSelected = false;

  juce::String name;
  juce::Label label;
};
} // namespace efx