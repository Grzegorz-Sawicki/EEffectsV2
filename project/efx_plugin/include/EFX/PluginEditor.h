#pragma once

namespace efx {
namespace {
class Background : public juce::Component {
public:
  void paint(juce::Graphics& g) override {
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
  }
};
}

class LabeledSlider : public juce::Component {
public:
  LabeledSlider (const juce::String& labelText) {
    label.setText(labelText, juce::dontSendNotification);

    setSliderDefaults();
    slider.getProperties().set("isBipolar", true);
    slider.setColour(custom_colors::highlightDefault, CustomLookAndFeel::getColor(CustomLookAndFeel::Colors::whiteHighlight));

    setLabelDefaults();
    label.setColour(juce::Label::textColourId, CustomLookAndFeel::getColor(CustomLookAndFeel::Colors::whiteHighlight));
    addAndMakeVisible(slider);
    addAndMakeVisible(label);
  }

  void resized() override {
    auto area = getLocalBounds();

    slider.setTopLeftPosition(area.getTopLeft());
    label.setSize(area.getWidth(), 20);
    label.setTopLeftPosition(slider.getX(), slider.getBottom() - 5.0f);
  }

  juce::Slider slider;
  juce::Label label;

private:
  void setSliderDefaults() {
    slider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    slider.setPopupDisplayEnabled(true, true, nullptr);
    slider.setSize(40, 40);
  }

  void setLabelDefaults() {
    label.setInterceptsMouseClicks(false, false);
    label.setJustificationType(juce::Justification::centred);
  }

};

class PluginEditor : public juce::AudioProcessorEditor {
public:
  explicit PluginEditor(PluginProcessor&);
  ~PluginEditor() override;

  void resized() override;

private:
  juce::ImageComponent logo;
  juce::Label label{"test label", "TEST"};
  Background background;

  LabeledSlider gainLabeledSlider{"GAIN"};
  juce::SliderParameterAttachment gainAttachment;

  LabeledSlider panLabeledSlider{"PAN"};
  juce::SliderParameterAttachment panAttachment;

  juce::ToggleButton bypassButton;
  juce::Label bypassLabel{"bypass label", "BYPASS"};
  juce::ButtonParameterAttachment bypassAttachment;

  VUMeter vuMeter;

  CustomLookAndFeel lookAndFeel;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginEditor)
};
}  // namespace efx
