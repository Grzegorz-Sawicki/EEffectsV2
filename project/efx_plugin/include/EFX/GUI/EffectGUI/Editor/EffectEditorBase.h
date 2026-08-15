#pragma once

namespace efx {
class EffectEditorBase : public juce::Component {
protected:
  void setupLabel(juce::Label &label, juce::Font &font) {
    label.setInterceptsMouseClicks(false, false);
    label.setJustificationType(juce::Justification::centred);
    label.setColour(juce::Label::textColourId, mainColor);
    label.setFont(font);
  }

  void setupActiveButton(juce::TextButton &activeButton) {
    activeButton.setClickingTogglesState(true);
    activeButton.setColour(custom_colors::highlight, mainColor);

    activeButton.onClick = [&activeButton]() {
      activeButton.setButtonText(activeButton.getToggleState() ? "ON" : "OFF");
    };
    activeButton.onClick();
  }

  void setupSlider(juce::Slider &slider) {
    slider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    slider.setPopupDisplayEnabled(true, true, nullptr);
    slider.setColour(custom_colors::highlight, mainColor);
  }

  void setupComboBox(juce::ComboBox &comboBox) {
    comboBox.setColour(custom_colors::highlight, mainColor);
  }

  juce::Colour mainColor;
  juce::Font labelFont = CustomLookAndFeel::getInterMediumFont().withPointHeight(10.0f);
  juce::Font logoFont = CustomLookAndFeel::getOrbitronMediumFont().withPointHeight(20.0f);
};
} //namespace efx