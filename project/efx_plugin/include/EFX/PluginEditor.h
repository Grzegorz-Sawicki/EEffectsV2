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

class PluginEditor : public juce::AudioProcessorEditor {
public:
  explicit PluginEditor(PluginProcessor&);
  ~PluginEditor() override;

  void resized() override;

private:
  void setSliderDefaults(juce::Slider& slider);

  juce::ImageComponent logo;
  juce::Label label{"test label", "TEST"};
  Background background;

  juce::Slider gainSlider;
  juce::Label gainLabel{"gain label", "GAIN"};
  juce::SliderParameterAttachment gainAttachment;

  juce::Slider panSlider;
  juce::Label panLabel{"pan label", "PAN"};
  juce::SliderParameterAttachment panAttachment;

  juce::ToggleButton bypassButton;
  juce::Label bypassLabel{"bypass label", "BYPASS"};
  juce::ButtonParameterAttachment bypassAttachment;

  VUMeter vuMeter;

  CustomLookAndFeel lookAndFeel;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginEditor)
};
}  // namespace efx
