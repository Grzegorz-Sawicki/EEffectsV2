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

class LabeledComboBox : public juce::Component {
public:
  LabeledComboBox(const juce::String &labelText) {
    label.setText(labelText, juce::dontSendNotification);

    addAndMakeVisible(comboBox);
    addAndMakeVisible(label);
  }

  void resized() override {
    auto area = getLocalBounds();

    comboBox.setSize(90, 21);
    label.setSize(area.getWidth(), 15);

    comboBox.setTopLeftPosition(area.getTopLeft());
    label.setTopLeftPosition(area.getX(), area.getBottom() - label.getHeight());
  }

  juce::ComboBox comboBox;
  juce::Label label;
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
  EffectRackItem(juce::String effectName) :
      name(std::move(effectName)),
      background(CustomLookAndFeel::getColor(CustomLookAndFeel::Colors::grey)),
      whiteBackground(CustomLookAndFeel::getColor(CustomLookAndFeel::Colors::whiteHighlight)) {
    addAndMakeVisible(whiteBackground);
    addAndMakeVisible(background);

    label.setText(name, juce::dontSendNotification);
    label.setInterceptsMouseClicks(false, false);
    addAndMakeVisible(label);

    button.getProperties().set("customFontSize", 9.0f);
    button.setClickingTogglesState(true);
    button.onClick = [this]() {
      button.setButtonText(button.getToggleState() ? "ON" : "OFF");
    };
    button.onClick();
    setMainColor(mainColor);
    addAndMakeVisible(button);

    addMouseListener(this, true);
    updateHoverVisuals();
  }

  void resized() override {
    auto bounds = getLocalBounds();

    whiteBackground.setBounds(bounds);
    background.setBounds(bounds);

    button.setBounds(88, 3, 22, 14);
    label.setBounds(3, 3, 82, 14);
  }

  void paintOverChildren(juce::Graphics& g) override {
    if (isSelected) {
      g.setColour(mainColor);

      g.drawRect(getLocalBounds(), 1);
    }
  }

  void setSelected(bool shouldBeSelected) {
    if (isSelected != shouldBeSelected) {
      isSelected = shouldBeSelected;
      repaint();
    }
  }

  void mouseUp(const juce::MouseEvent &event) override {
    if (event.eventComponent != &button) {
      if (onSelect)
        onSelect(name);
    }
  }

  void mouseEnter(const juce::MouseEvent &) override { updateHoverVisuals(); }
  void mouseExit(const juce::MouseEvent &) override { updateHoverVisuals(); }

  void setMainColor(juce::Colour color) {
    mainColor = color;
    label.setColour(juce::Label::textColourId, mainColor);
    button.setColour(custom_colors::highlight, mainColor);
  }

  std::function<void(juce::String)> onSelect;

  Background whiteBackground;
  Background background;
  juce::TextButton button;

  juce::Colour mainColor = CustomLookAndFeel::getColor(CustomLookAndFeel::Colors::redHighlight);

private:
  void updateHoverVisuals() {
    bool currentlyHovered = isMouseOver(true);

    if (isHovered != currentlyHovered) {
      isHovered = currentlyHovered;

      float childAlpha = isHovered ? 0.85f : 1.0f;
      background.setAlpha(childAlpha);
      label.setAlpha(childAlpha);
      button.setAlpha(childAlpha);

      repaint();
    }
  }

  bool isHovered = false;
  bool isSelected = false;

  juce::String name;
  juce::Label label;
};

class EffectRackView : public juce::Component {
public:
  EffectRackView() : background(CustomLookAndFeel::getColor(CustomLookAndFeel::Colors::effectBackground)) {
    tremoloItem.setMainColor(CustomLookAndFeel::getColor(CustomLookAndFeel::Colors::tremoloHighlight));
    flangerItem.setMainColor(CustomLookAndFeel::getColor(CustomLookAndFeel::Colors::flangerHighlight));
    filterItem.setMainColor(CustomLookAndFeel::getColor(CustomLookAndFeel::Colors::filterHighlight));

    addAndMakeVisible(background);
    addAndMakeVisible(tremoloItem);
    addAndMakeVisible(flangerItem);
    addAndMakeVisible(filterItem);

    auto itemClickedCallback = [this](const juce::String& selectedName) {
      setSelectedItem(selectedName);

      if (onEffectChanged)
        onEffectChanged(selectedName);
    };

    tremoloItem.onSelect = itemClickedCallback;
    flangerItem.onSelect = itemClickedCallback;
    filterItem.onSelect = itemClickedCallback;
  }

  void resized() override {
    auto bounds = getLocalBounds();
    background.setBounds(bounds);

    auto padding = 4;
    auto itemWidth = 113;
    auto itemHeight = 20;

    tremoloItem.setBounds(padding, padding, itemWidth, itemHeight);
    flangerItem.setBounds(padding, tremoloItem.getBottom() + padding, itemWidth, itemHeight);
    filterItem.setBounds(padding, flangerItem.getBottom() + padding, itemWidth, itemHeight);
  }

  void setSelectedItem(const juce::String& effectName) {
    tremoloItem.setSelected(effectName == "Tremolo");
    flangerItem.setSelected(effectName == "Flanger");
    filterItem.setSelected(effectName == "Filter");
  }

  std::function<void(juce::String)> onEffectChanged;

  Background background;
  EffectRackItem tremoloItem{"Tremolo"};
  EffectRackItem flangerItem{"Flanger"};
  EffectRackItem filterItem{"Filter"};
};

class TremoloEditor : public juce::Component {
public:
  TremoloEditor(PluginProcessor &p) :
      activeAttachment(p.getParameterRefs().tremoloActive, activeButton),
      mixAttachment(p.getParameterRefs().tremoloMix, mixLabeledSlider.slider),
      depthAttachment(p.getParameterRefs().tremoloDepth, depthLabeledSlider.slider),
      rateAttachment(p.getParameterRefs().tremoloRate, rateLabeledSlider.slider),
      waveformAttachment(p.getParameterRefs().tremoloWaveform, waveformLabeledComboBox.comboBox) {
    addAndMakeVisible(background);
    addAndMakeVisible(innerBackground);

    activeButton.setClickingTogglesState(true);
    activeButton.onClick = [this]() {
      activeButton.setButtonText(activeButton.getToggleState() ? "ON" : "OFF");
    };
    activeButton.onClick();
    activeButton.setColour(custom_colors::highlight, mainColor);
    addAndMakeVisible(activeButton);

    mixLabeledSlider.slider.setColour(custom_colors::highlight, mainColor);
    mixLabeledSlider.label.setColour(juce::Label::textColourId, mainColor);
    addAndMakeVisible(mixLabeledSlider);

    depthLabeledSlider.slider.setColour(custom_colors::highlight, mainColor);
    depthLabeledSlider.label.setColour(juce::Label::textColourId, mainColor);
    addAndMakeVisible(depthLabeledSlider);

    rateLabeledSlider.slider.setColour(custom_colors::highlight, mainColor);
    rateLabeledSlider.label.setColour(juce::Label::textColourId, mainColor);
    addAndMakeVisible(rateLabeledSlider);

    waveformLabeledComboBox.comboBox.setColour(custom_colors::highlight, mainColor);
    waveformLabeledComboBox.comboBox.addItemList(p.getParameterRefs().tremoloWaveform.choices, 1);
    waveformAttachment.sendInitialUpdate();
    waveformLabeledComboBox.label.setColour(juce::Label::textColourId, mainColor);
    addAndMakeVisible(waveformLabeledComboBox);
  }

  void resized() override {
    auto bounds = getLocalBounds();
    auto backgroundBounds = bounds;
    background.setBounds(backgroundBounds);

    auto innerBackgroundBounds = backgroundBounds.reduced(5);
    innerBackground.setBounds(innerBackgroundBounds);

    activeButton.setBounds(292, 50, 50, 55);
    mixLabeledSlider.setBounds(238, 38, 50, 55);
    depthLabeledSlider.setBounds(158, 38, 50, 55);
    rateLabeledSlider.setBounds(106, 38, 50, 55);

    waveformLabeledComboBox.setBounds(15, 51, 90, 35);
  }

private:
  juce::Colour mainColor = CustomLookAndFeel::getColor(CustomLookAndFeel::Colors::tremoloHighlight);

  Background background{mainColor};
  Background innerBackground{CustomLookAndFeel::getColor(CustomLookAndFeel::Colors::effectBackground)};

  juce::TextButton activeButton;
  juce::ButtonParameterAttachment activeAttachment;

  LabeledSlider mixLabeledSlider{"MIX"};
  juce::SliderParameterAttachment mixAttachment;

  LabeledSlider depthLabeledSlider{"DEPTH"};
  juce::SliderParameterAttachment depthAttachment;

  LabeledSlider rateLabeledSlider{"RATE"};
  juce::SliderParameterAttachment rateAttachment;

  LabeledComboBox waveformLabeledComboBox{"WAVEFORM"};
  juce::ComboBoxParameterAttachment waveformAttachment;
};

class FlangerEditor : public juce::Component {
public:
  FlangerEditor(PluginProcessor &processor) {

  }

  void paint(juce::Graphics &g) override {
    const auto bounds = getLocalBounds();

    g.setColour(juce::Colours::green);
    g.fillRect(bounds);
  }
};

class FilterEditor : public juce::Component {
public:
  FilterEditor(PluginProcessor &processor) {

  }

  void paint(juce::Graphics &g) override {
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
