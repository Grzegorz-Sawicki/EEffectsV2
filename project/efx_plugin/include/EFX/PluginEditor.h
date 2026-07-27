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

  void resized() override;

private:
  void setSliderDefaults(juce::Slider& slider);

  juce::ImageComponent logo;
  juce::Label label{"test label", "TEST"};
  Background background;
  juce::Slider gainSlider;

  CustomLookAndFeel lookAndFeel;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginEditor)
};
}  // namespace efx
