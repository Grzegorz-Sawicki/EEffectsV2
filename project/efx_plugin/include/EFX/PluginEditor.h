#include <utility>

#pragma once

namespace efx {
class PluginEditor : public juce::AudioProcessorEditor {
public:
  explicit PluginEditor(PluginProcessor &);

  ~PluginEditor() override;

  void paint(juce::Graphics &g) override;

  void resized() override;

private:
  void setupSlider(juce::Slider &slider, juce::Label &label, const juce::String &labelText);

  void setupToggleButton(juce::ToggleButton &button, juce::Label &label, const juce::String &labelText);

  std::unique_ptr<juce::Drawable> logo;

  juce::Slider gainSlider;
  juce::Label gainLabel;
  juce::SliderParameterAttachment gainAttachment;

  juce::Slider panSlider;
  juce::Label panLabel;
  juce::SliderParameterAttachment panAttachment;

  juce::ToggleButton bypassButton;
  juce::Label bypassLabel;
  juce::ButtonParameterAttachment bypassAttachment;

  VUMeter vuMeter;

  EffectRackView effectRackView;
  EffectDetailView effectDetailView;

  CustomLookAndFeel lookAndFeel;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginEditor)
};
}  // namespace efx