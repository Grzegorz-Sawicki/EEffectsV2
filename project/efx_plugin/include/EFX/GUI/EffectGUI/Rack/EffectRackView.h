#pragma once

namespace efx {
class EffectRackView : public juce::Component {
public:
  EffectRackView(PluginProcessor &p) :
      tremoloActiveAttachment(p.getParameterRefs().tremoloActive, tremoloItem.button),
      flangerActiveAttachment(p.getParameterRefs().flangerActive, flangerItem.button),
      lowpassActiveAttachment(p.getParameterRefs().lowpassActive, filterItem.button),
      bandpassActiveAttachment(p.getParameterRefs().bandpassActive, filterItem.button2),
      highpassActiveAttachment(p.getParameterRefs().highpassActive, filterItem.button3) {
    tremoloItem.setMainColor(CustomLookAndFeel::getColor(CustomLookAndFeel::Colors::tremoloHighlight));
    flangerItem.setMainColor(CustomLookAndFeel::getColor(CustomLookAndFeel::Colors::flangerHighlight));
    filterItem.setMainColor(CustomLookAndFeel::getColor(CustomLookAndFeel::Colors::filterHighlight));

    addAndMakeVisible(tremoloItem);
    addAndMakeVisible(flangerItem);
    addAndMakeVisible(filterItem);

    auto itemClickedCallback = [this](const juce::String &selectedName) {
      setSelectedItem(selectedName);

      if (onEffectChanged)
        onEffectChanged(selectedName);
    };

    tremoloItem.onSelect = itemClickedCallback;
    flangerItem.onSelect = itemClickedCallback;
    filterItem.onSelect = itemClickedCallback;
  }

  void paint(juce::Graphics &g) override {
    const auto bounds = getLocalBounds();

    g.setColour(CustomLookAndFeel::getColor(CustomLookAndFeel::Colors::effectBackground));
    g.fillRect(bounds);
  }

  void resized() override {
    constexpr int padding = 4;
    constexpr int itemWidth = 113;
    constexpr int itemHeight = 20;

    tremoloItem.setBounds(padding, padding, itemWidth, itemHeight);
    flangerItem.setBounds(padding, tremoloItem.getBottom() + padding, itemWidth, itemHeight);
    filterItem.setBounds(padding, flangerItem.getBottom() + padding, itemWidth, itemHeight);
  }

  void setSelectedItem(const juce::String &effectName) {
    tremoloItem.setSelected(effectName == "Tremolo");
    flangerItem.setSelected(effectName == "Flanger");
    filterItem.setSelected(effectName == "Filter");
  }

  std::function<void(juce::String)> onEffectChanged;

  EffectRackItem tremoloItem{"Tremolo"};
  juce::ButtonParameterAttachment tremoloActiveAttachment;

  EffectRackItem flangerItem{"Flanger"};
  juce::ButtonParameterAttachment flangerActiveAttachment;

  MultiButtonRackItem filterItem{"Filter"};
  juce::ButtonParameterAttachment lowpassActiveAttachment;
  juce::ButtonParameterAttachment bandpassActiveAttachment;
  juce::ButtonParameterAttachment highpassActiveAttachment;
};
} // namespace efx