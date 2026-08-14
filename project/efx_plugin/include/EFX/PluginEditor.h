#include <utility>

#pragma once

namespace efx {

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

  void paintOverChildren(juce::Graphics &g) override {
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

protected:
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

class MultiButtonRackItem : public EffectRackItem {
public:
  MultiButtonRackItem(juce::String effectName) : EffectRackItem(std::move(effectName)) {
    setupExtraButton(button2);
    setupExtraButton(button3);
  }

  void resized() override {
    EffectRackItem::resized();

    button.setBounds(88, 3, 22, 14);
    label.setBounds(3, 3, 82, 14);

    button3.setBounds(88, 3, 22, 14);
    button2.setBounds(63, 3, 22, 14);
    button.setBounds(38, 3, 22, 14);

    label.setBounds(3, 3, 32, 14);
  }

  void setMainColor(juce::Colour color) {
    EffectRackItem::setMainColor(color);
    button2.setColour(custom_colors::highlight, color);
    button3.setColour(custom_colors::highlight, color);
  }

  void mouseUp(const juce::MouseEvent &event) override {
    if (event.eventComponent != &button && event.eventComponent != &button2 && event.eventComponent != &button3) {
      if (onSelect)
        onSelect(name);
    }
  }

  juce::TextButton button2;
  juce::TextButton button3;

private:
  void setupExtraButton(juce::TextButton& btn) {
    btn.getProperties().set("customFontSize", 9.0f);
    btn.setClickingTogglesState(true);
    btn.onClick = [&btn]() {
      btn.setButtonText(btn.getToggleState() ? "ON" : "OFF");
    };
    btn.onClick();
    addAndMakeVisible(btn);
  }
};

class EffectRackView : public juce::Component {
public:
  EffectRackView(PluginProcessor &p) :
      background(CustomLookAndFeel::getColor(CustomLookAndFeel::Colors::effectBackground)),
      tremoloActiveAttachment(p.getParameterRefs().tremoloActive, tremoloItem.button),
      flangerActiveAttachment(p.getParameterRefs().flangerActive, flangerItem.button),
      lowpassActiveAttachment(p.getParameterRefs().lowpassActive, filterItem.button),
      bandpassActiveAttachment(p.getParameterRefs().bandpassActive, filterItem.button2),
      highpassActiveAttachment(p.getParameterRefs().highpassActive, filterItem.button3) {
    tremoloItem.setMainColor(CustomLookAndFeel::getColor(CustomLookAndFeel::Colors::tremoloHighlight));
    flangerItem.setMainColor(CustomLookAndFeel::getColor(CustomLookAndFeel::Colors::flangerHighlight));
    filterItem.setMainColor(CustomLookAndFeel::getColor(CustomLookAndFeel::Colors::filterHighlight));

    addAndMakeVisible(background);
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

  void setSelectedItem(const juce::String &effectName) {
    tremoloItem.setSelected(effectName == "Tremolo");
    flangerItem.setSelected(effectName == "Flanger");
    filterItem.setSelected(effectName == "Filter");
  }

  std::function<void(juce::String)> onEffectChanged;

  Background background;
  EffectRackItem tremoloItem{"Tremolo"};
  juce::ButtonParameterAttachment tremoloActiveAttachment;

  EffectRackItem flangerItem{"Flanger"};
  juce::ButtonParameterAttachment flangerActiveAttachment;

  MultiButtonRackItem filterItem{"Filter"};
  juce::ButtonParameterAttachment lowpassActiveAttachment;
  juce::ButtonParameterAttachment bandpassActiveAttachment;
  juce::ButtonParameterAttachment highpassActiveAttachment;
};

class TremoloEditor : public juce::Component {
public:
  TremoloEditor(PluginProcessor &p) :
      activeAttachment(p.getParameterRefs().tremoloActive, activeButton),
      mixAttachment(p.getParameterRefs().tremoloMix, mixSlider),
      depthAttachment(p.getParameterRefs().tremoloDepth, depthSlider),
      rateAttachment(p.getParameterRefs().tremoloRate, rateSlider),
      waveformAttachment(p.getParameterRefs().tremoloWaveform, waveformComboBox) {
    addAndMakeVisible(background);
    addAndMakeVisible(innerBackground);

    activeButton.setClickingTogglesState(true);
    activeButton.onClick = [this]() {
      activeButton.setButtonText(activeButton.getToggleState() ? "ON" : "OFF");
    };
    activeButton.onClick();
    activeButton.setColour(custom_colors::highlight, mainColor);
    addAndMakeVisible(activeButton);

    setupSlider(mixSlider, mixLabel, "MIX");
    addAndMakeVisible(mixSlider);
    addAndMakeVisible(mixLabel);

    setupSlider(depthSlider, depthLabel, "DEPTH");
    addAndMakeVisible(depthSlider);
    addAndMakeVisible(depthLabel);

    setupSlider(rateSlider, rateLabel, "RATE");
    addAndMakeVisible(rateSlider);
    addAndMakeVisible(rateLabel);

    waveformComboBox.setColour(custom_colors::highlight, mainColor);
    waveformComboBox.addItemList(p.getParameterRefs().tremoloWaveform.choices, 1);
    waveformAttachment.sendInitialUpdate();
    waveformLabel.setText("WAVEFORM", juce::dontSendNotification);
    waveformLabel.setColour(juce::Label::textColourId, mainColor);
    waveformLabel.setJustificationType(juce::Justification::centred);
    waveformLabel.setInterceptsMouseClicks(false, false);
    addAndMakeVisible(waveformComboBox);
    addAndMakeVisible(waveformLabel);
  }

  void resized() override {
    auto bounds = getLocalBounds();
    auto backgroundBounds = bounds;
    background.setBounds(backgroundBounds);

    auto innerBackgroundBounds = backgroundBounds.reduced(2);
    innerBackground.setBounds(innerBackgroundBounds);

    activeButton.setBounds(292, 50, 50, 55);

    mixSlider.setBounds(243, 38, 40, 40);
    mixLabel.setBounds(238, 78, 50, 15);

    depthSlider.setBounds(163, 38, 40, 40);
    depthLabel.setBounds(158, 78, 50, 15);

    rateSlider.setBounds(111, 38, 40, 40);
    rateLabel.setBounds(106, 78, 50, 15);

    waveformComboBox.setBounds(15, 51, 90, 21);
    waveformLabel.setBounds(15, 71, 90, 15);
  }

private:
  void setupSlider(juce::Slider& slider, juce::Label& label, const juce::String& labelText) {
    slider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    slider.setPopupDisplayEnabled(true, true, nullptr);
    slider.setColour(custom_colors::highlight, mainColor);

    label.setText(labelText, juce::dontSendNotification);
    label.setInterceptsMouseClicks(false, false);
    label.setJustificationType(juce::Justification::centred);
    label.setColour(juce::Label::textColourId, mainColor);
  }

  juce::Colour mainColor = CustomLookAndFeel::getColor(CustomLookAndFeel::Colors::tremoloHighlight);

  Background background{mainColor};
  Background innerBackground{CustomLookAndFeel::getColor(CustomLookAndFeel::Colors::effectBackground)};

  juce::TextButton activeButton;
  juce::ButtonParameterAttachment activeAttachment;

  juce::Slider mixSlider;
  juce::Label mixLabel;
  juce::SliderParameterAttachment mixAttachment;

  juce::Slider depthSlider;
  juce::Label depthLabel;
  juce::SliderParameterAttachment depthAttachment;

  juce::Slider rateSlider;
  juce::Label rateLabel;
  juce::SliderParameterAttachment rateAttachment;

  juce::ComboBox waveformComboBox;
  juce::Label waveformLabel;
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
  void setupSlider(juce::Slider& slider, juce::Label& label, const juce::String& labelText);
  void setupToggleButton(juce::ToggleButton& button, juce::Label& label, const juce::String& labelText);

  std::unique_ptr<juce::Drawable> logo;
  Background background;

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