#pragma once

namespace efx {
class MultiButtonRackItem : public EffectRackItem {
public:
  MultiButtonRackItem(juce::String effectName) : EffectRackItem(std::move(effectName)) {
    setupExtraButton(button2);
    setupExtraButton(button3);
  }

  void resized() override {
    EffectRackItem::resized();

    button.setBounds(88, 3, 22, 14);
    label.setBounds(3, 3, 82, 14);

    button3.setBounds(88, 3, 22, 14);
    button2.setBounds(63, 3, 22, 14);
    button.setBounds(38, 3, 22, 14);

    label.setBounds(3, 3, 32, 14);
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
    btn.getProperties().set("customFontSize", 9.0f);
    btn.setClickingTogglesState(true);
    btn.onClick = [&btn]() {
      btn.setButtonText(btn.getToggleState() ? "ON" : "OFF");
    };
    btn.onClick();
    addAndMakeVisible(btn);
  }
};
} // namespace efx