#pragma once

namespace efx {
class MultiButtonRackItem : public EffectRackItem {
public:
  MultiButtonRackItem(juce::String effectName) : EffectRackItem(std::move(effectName)) {
    setupExtraButton(button2);
    setupExtraButton(button3);
  }

  void resized() override {
    constexpr int labelWidth = 32;
    constexpr int labelHeight = 14;

    EffectRackItem::resized();

    button3.setBounds(88, 3, GUI::smallTextButtonWidth, GUI::smallTextButtonHeight);
    button2.setBounds(63, 3, GUI::smallTextButtonWidth, GUI::smallTextButtonHeight);
    button.setBounds(38, 3, GUI::smallTextButtonWidth, GUI::smallTextButtonHeight);

    label.setBounds(3, 3, labelWidth, labelHeight);
  }

  void setMainColor(juce::Colour color) {
    EffectRackItem::setMainColor(color);
    button2.setColour(custom_colors::highlight, color);
    button3.setColour(custom_colors::highlight, color);
  }

  void mouseUp(const juce::MouseEvent &event) override {
    if (event.eventComponent != &button && event.eventComponent != &button2 && event.eventComponent != &button3) {
      if (onSelect)
        onSelect(name);
    }
  }

  juce::TextButton button2;
  juce::TextButton button3;

private:
  void setupExtraButton(juce::TextButton &btn) {
    btn.getProperties().set("customFontSize", fontSize);
    btn.setClickingTogglesState(true);
    btn.onClick = [&btn]() {
      btn.setButtonText(btn.getToggleState() ? "ON" : "OFF");
    };
    btn.onClick();
    addAndMakeVisible(btn);
  }
};
} // namespace efx