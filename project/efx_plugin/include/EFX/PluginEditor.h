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
  juce::Colour mainColor = CustomLookAndFeel::getColor(CustomLookAndFeel::Colors::whiteHighlight);
  juce::Font labelFont = CustomLookAndFeel::getInterMediumFont();

  std::unique_ptr<juce::Drawable> logo;

  juce::Slider gainSlider;
  juce::Label gainLabel{"gainLabel", "GAIN"};
  juce::SliderParameterAttachment gainAttachment;

  juce::Slider panSlider;
  juce::Label panLabel{"panLabel", "PAN"};
  juce::SliderParameterAttachment panAttachment;

  juce::ToggleButton bypassButton;
  juce::Label bypassLabel{"bypassLabel", "BYPASS"};
  juce::ButtonParameterAttachment bypassAttachment;

  VUMeter vuMeter;

  EffectRackView effectRackView;
  EffectDetailView effectDetailView;

  CustomLookAndFeel lookAndFeel;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginEditor)
};
}  // namespace efx