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
    setLabelDefaults();

    addAndMakeVisible(slider);
    addAndMakeVisible(label);
  }

  void resized() override {
    auto area = getLocalBounds();
    label.setSize(area.getWidth(), 15);

    slider.setCentrePosition(area.getWidth() * 0.5f, slider.getHeight() * 0.5f);
    label.setTopLeftPosition(area.getX(), area.getBottom() - label.getHeight());
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

class LabeledButton : public juce::Component {
public:
  LabeledButton (const juce::String& labelText) {
    label.setText(labelText, juce::dontSendNotification);

    setButtonDefaults();
    setLabelDefaults();

    addAndMakeVisible(button);
    addAndMakeVisible(label);
  }

  void resized() override {
    auto area = getLocalBounds();

    button.setSize(30, 30);
    label.setSize(area.getWidth(), 15);

    button.setCentrePosition(area.getWidth() * 0.5f, (area.getHeight() - label.getHeight()) * 0.5f);
    label.setTopLeftPosition(area.getX(), area.getBottom() - label.getHeight());
  }

  juce::ToggleButton button;
  juce::Label label;

private:
  void setButtonDefaults() {}

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

  LabeledButton bypassLabeledButton{"BYPASS"};
  juce::ButtonParameterAttachment bypassAttachment;

  VUMeter vuMeter;

  CustomLookAndFeel lookAndFeel;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginEditor)
};
}  // namespace efx
