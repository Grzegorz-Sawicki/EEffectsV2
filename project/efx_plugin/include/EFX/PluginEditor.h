#include <utility>

#pragma once

namespace efx {

class LabeledSlider : public juce::Component {
public:
  LabeledSlider(const juce::String &labelText) {
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
  LabeledButton(const juce::String &labelText) {
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

namespace {
class Background : public juce::Component {
public:
  explicit Background(juce::Colour color) : color(color) {};

  void paint(juce::Graphics &g) override {
    g.fillAll(color);
  }

private:
  juce::Colour color;
};

class EffectRackItem : public juce::Component {
public:
  EffectRackItem(juce::String effectName) : name(std::move(effectName)) {
    label.setText(name, juce::dontSendNotification);
    label.setInterceptsMouseClicks(false, false);
    addAndMakeVisible(label);

    bypassButton.setButtonText("On");
    bypassButton.setClickingTogglesState(true);
    addAndMakeVisible(bypassButton);
  }

  void resized() override {
    auto bounds = getLocalBounds().reduced(4);
    bypassButton.setBounds(bounds.removeFromRight(40));
    label.setBounds(bounds);
  }

  void mouseUp(const juce::MouseEvent &event) override {
    if (event.eventComponent != &bypassButton) {
      if (onSelect)
        onSelect(name);
    }
  }

  std::function<void(juce::String)> onSelect;

  juce::TextButton bypassButton;

private:
  juce::String name;
  juce::Label label;
};

class EffectRackView : public juce::Component {
public:
  EffectRackView() {
    addAndMakeVisible(tremoloItem);
    addAndMakeVisible(flangerItem);
    addAndMakeVisible(filterItem);
  }

  void resized() override {
    auto bounds = getLocalBounds();
    int itemHeight = 30;

    tremoloItem.setBounds(bounds.removeFromTop(itemHeight));
    flangerItem.setBounds(bounds.removeFromTop(itemHeight));
    filterItem.setBounds(bounds.removeFromTop(itemHeight));
  }

  EffectRackItem tremoloItem{"Tremolo"};
  EffectRackItem flangerItem{"Flanger"};
  EffectRackItem filterItem{"Filter"};
};

class TremoloEditor : public juce::Component {
public:
  TremoloEditor(PluginProcessor &p) :
      bypassAttachment(p.getParameterRefs().tremoloBypass, bypassLabeledButton.button),
      mixAttachment(p.getParameterRefs().tremoloMix, mixLabeledSlider.slider),
      depthAttachment(p.getParameterRefs().tremoloDepth, depthLabeledSlider.slider),
      rateAttachment(p.getParameterRefs().tremoloRate, rateLabeledSlider.slider) {
    addAndMakeVisible(background);
    addAndMakeVisible(innerBackground);

    bypassLabeledButton.label.setColour(juce::Label::textColourId, mainColor);
    addAndMakeVisible(bypassLabeledButton);

    mixLabeledSlider.slider.setColour(custom_colors::highlight, mainColor);
    mixLabeledSlider.label.setColour(juce::Label::textColourId, mainColor);
    addAndMakeVisible(mixLabeledSlider);

    depthLabeledSlider.slider.setColour(custom_colors::highlight, mainColor);
    depthLabeledSlider.label.setColour(juce::Label::textColourId, mainColor);
    addAndMakeVisible(depthLabeledSlider);

    rateLabeledSlider.slider.setColour(custom_colors::highlight, mainColor);
    rateLabeledSlider.label.setColour(juce::Label::textColourId, mainColor);
    addAndMakeVisible(rateLabeledSlider);
  }

  void resized() override {
    auto bounds = getLocalBounds();
    auto backgroundBounds = bounds;
    background.setBounds(backgroundBounds);

    auto innerBackgroundBounds = backgroundBounds.reduced(5);
    innerBackground.setBounds(innerBackgroundBounds);

    bypassLabeledButton.setBounds(292, 50, 50, 55);
    mixLabeledSlider.setBounds(238, 38, 50, 55);
    depthLabeledSlider.setBounds(158, 38, 50, 55);
    rateLabeledSlider.setBounds(106, 38, 50, 55);
  }

private:
  juce::Colour mainColor = CustomLookAndFeel::getColor(CustomLookAndFeel::Colors::tremoloHighlight);

  Background background{mainColor};
  Background innerBackground{CustomLookAndFeel::getColor(CustomLookAndFeel::Colors::effectBackground)};

  LabeledButton bypassLabeledButton{"BYPASS"};
  juce::ButtonParameterAttachment bypassAttachment;

  LabeledSlider mixLabeledSlider{"MIX"};
  juce::SliderParameterAttachment mixAttachment;

  LabeledSlider depthLabeledSlider{"DEPTH"};
  juce::SliderParameterAttachment depthAttachment;

  LabeledSlider rateLabeledSlider{"RATE"};
  juce::SliderParameterAttachment rateAttachment;

  //TODO WAVEFORM CHOICE
};

class FlangerEditor : public juce::Component {
public:
  FlangerEditor(PluginProcessor &processor) {

  }

  void paint(juce::Graphics& g) override {
    const auto bounds = getLocalBounds();

    g.setColour(juce::Colours::green);
    g.fillRect(bounds);
  }
};

class FilterEditor : public juce::Component {
public:
  FilterEditor(PluginProcessor &processor) {

  }

  void paint(juce::Graphics& g) override {
    const auto bounds = getLocalBounds();

    g.setColour(juce::Colours::blue);
    g.fillRect(bounds);
  }
};

class EffectDetailView : public juce::Component {
public:
  EffectDetailView(PluginProcessor &processor)
      : tremoloEditor(processor), flangerEditor(processor), filterEditor(processor) {
    addChildComponent(tremoloEditor);
    addChildComponent(flangerEditor);
    addChildComponent(filterEditor);
  }

  void resized() override {
    auto bounds = getLocalBounds();

    tremoloEditor.setBounds(bounds);
    flangerEditor.setBounds(bounds);
    filterEditor.setBounds(bounds);
  }

  void showEditor(const juce::String &effectName) {
    tremoloEditor.setVisible(false);
    flangerEditor.setVisible(false);
    filterEditor.setVisible(false);

    if (effectName == "Tremolo") tremoloEditor.setVisible(true);
    else if (effectName == "Flanger") flangerEditor.setVisible(true);
    else if (effectName == "Filter") filterEditor.setVisible(true);
  }

private:
  TremoloEditor tremoloEditor;
  FlangerEditor flangerEditor;
  FilterEditor filterEditor;
};
}

class PluginEditor : public juce::AudioProcessorEditor {
public:
  explicit PluginEditor(PluginProcessor &);

  ~PluginEditor() override;

  void resized() override;

private:
  juce::ImageComponent logo;
  Background background;

  LabeledSlider gainLabeledSlider{"GAIN"};
  juce::SliderParameterAttachment gainAttachment;

  LabeledSlider panLabeledSlider{"PAN"};
  juce::SliderParameterAttachment panAttachment;

  LabeledButton bypassLabeledButton{"BYPASS"};
  juce::ButtonParameterAttachment bypassAttachment;

  VUMeter vuMeter;

  EffectRackView effectRackView;
  EffectDetailView effectDetailView;

  CustomLookAndFeel lookAndFeel;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginEditor)
};
}  // namespace efx
