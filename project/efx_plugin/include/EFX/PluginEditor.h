#include <utility>

#pragma once

namespace efx {
namespace {
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
  TremoloEditor(PluginProcessor &processor) {

  }

  void paint(juce::Graphics& g) override {
    const auto bounds = getLocalBounds();

    g.setColour(juce::Colours::red);
    g.fillRect(bounds);
  }
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

class Background : public juce::Component {
public:
  void paint(juce::Graphics &g) override {
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
  }
};
}

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
