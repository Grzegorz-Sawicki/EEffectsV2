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

  EffectRackItem tremoloItem{"Tremolo"};
  juce::ButtonParameterAttachment tremoloActiveAttachment;

  EffectRackItem flangerItem{"Flanger"};
  juce::ButtonParameterAttachment flangerActiveAttachment;

  MultiButtonRackItem filterItem{"Filter"};
  juce::ButtonParameterAttachment lowpassActiveAttachment;
  juce::ButtonParameterAttachment bandpassActiveAttachment;
  juce::ButtonParameterAttachment highpassActiveAttachment;
};

class EffectEditorBase : public juce::Component {
protected:
  void setupLabel(juce::Label& label, juce::Font& font) {
    label.setInterceptsMouseClicks(false, false);
    label.setJustificationType(juce::Justification::centred);
    label.setColour(juce::Label::textColourId, mainColor);
    label.setFont(font);
  }

  void setupActiveButton(juce::TextButton& activeButton) {
    activeButton.setClickingTogglesState(true);
    activeButton.setColour(custom_colors::highlight, mainColor);

    activeButton.onClick = [&activeButton]() {
      activeButton.setButtonText(activeButton.getToggleState() ? "ON" : "OFF");
    };
    activeButton.onClick();
  }

  void setupSlider(juce::Slider& slider) {
    slider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    slider.setPopupDisplayEnabled(true, true, nullptr);
    slider.setColour(custom_colors::highlight, mainColor);
  }

  void setupComboBox(juce::ComboBox& comboBox) {
    comboBox.setColour(custom_colors::highlight, mainColor);
  }

  juce::Colour mainColor;
  juce::Font labelFont = CustomLookAndFeel::getInterMediumFont().withPointHeight(10.0f);
  juce::Font logoFont = CustomLookAndFeel::getOrbitronMediumFont().withPointHeight(20.0f);
};

class TremoloEditor : public EffectEditorBase {
public:
  TremoloEditor(PluginProcessor &p) :
      activeAttachment(p.getParameterRefs().tremoloActive, activeButton),
      mixAttachment(p.getParameterRefs().tremoloMix, mixSlider),
      depthAttachment(p.getParameterRefs().tremoloDepth, depthSlider),
      rateAttachment(p.getParameterRefs().tremoloRate, rateSlider),
      waveformAttachment(p.getParameterRefs().tremoloWaveform, waveformComboBox) {
    mainColor = CustomLookAndFeel::getColor(CustomLookAndFeel::Colors::tremoloHighlight);

    setupLabel(logoLabel, logoFont);
    addAndMakeVisible(logoLabel);

    setupActiveButton(activeButton);
    addAndMakeVisible(activeButton);

    setupSlider(mixSlider);
    setupLabel(mixLabel, labelFont);
    addAndMakeVisible(mixSlider);
    addAndMakeVisible(mixLabel);

    setupSlider(depthSlider);
    setupLabel(depthLabel, labelFont);
    addAndMakeVisible(depthSlider);
    addAndMakeVisible(depthLabel);

    setupSlider(rateSlider);
    setupLabel(rateLabel, labelFont);
    addAndMakeVisible(rateSlider);
    addAndMakeVisible(rateLabel);

    setupComboBox(waveformComboBox);
    setupLabel(waveformLabel, labelFont);
    waveformComboBox.addItemList(p.getParameterRefs().tremoloWaveform.choices, 1);
    waveformAttachment.sendInitialUpdate();

    addAndMakeVisible(waveformComboBox);
    addAndMakeVisible(waveformLabel);
  }

  void paint(juce::Graphics &g) override {
    auto bounds = getLocalBounds().toFloat();

    g.setColour(mainColor);
    g.fillRect(bounds);

    auto innerBounds = bounds.reduced(2.0f);
    g.setColour(CustomLookAndFeel::getColor(CustomLookAndFeel::Colors::effectBackground));
    g.fillRect(innerBounds);

    g.setColour(mainColor);
    g.drawLine(12.0f, 129.0f, 240.0f, 129.0f, 2.0f);
    g.drawLine(239.0f, 129.0f, 239.0f, 13.0f, 2.0f);
    g.drawLine(238.0f, 13.0f, 343.0f, 13.0f, 2.0f);

    g.drawLine(12.0f, 13.0f, 126.0f, 13.0f, 2.0f);
    g.drawLine(12.0f, 37.0f, 126.0f, 37.0f, 2.0f);
  }

  void resized() override {
    logoLabel.setBounds(12, 10, 114, 29);

    activeButton.setBounds(305, 50, 38, 26);

    mixSlider.setBounds(259, 46, 34, 34);
    mixLabel.setBounds(259, 79, 34, 15);

    depthSlider.setBounds(185, 46, 34, 34);
    depthLabel.setBounds(183, 79, 40, 15);

    rateSlider.setBounds(139, 46, 34, 34);
    rateLabel.setBounds(139, 79, 34, 15);

    waveformComboBox.setBounds(53, 52, 74, 21);
    waveformLabel.setBounds(53, 79, 74, 15);
  }

private:
  juce::Label logoLabel{"tremoloLogoLabel", "TREMOLO"};

  juce::TextButton activeButton;
  juce::ButtonParameterAttachment activeAttachment;

  juce::Slider mixSlider;
  juce::Label mixLabel{"tremoloMixLabel", "MIX"};
  juce::SliderParameterAttachment mixAttachment;

  juce::Slider depthSlider;
  juce::Label depthLabel{"tremoloDepthLabel", "DEPTH"};
  juce::SliderParameterAttachment depthAttachment;

  juce::Slider rateSlider;
  juce::Label rateLabel{"tremoloRateLabel", "RATE"};
  juce::SliderParameterAttachment rateAttachment;

  juce::ComboBox waveformComboBox;
  juce::Label waveformLabel{"tremoloWaveformLabel", "WAVEFORM"};
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

  void paint(juce::Graphics &g) override;

  void resized() override;

private:
  void setupSlider(juce::Slider& slider, juce::Label& label, const juce::String& labelText);
  void setupToggleButton(juce::ToggleButton& button, juce::Label& label, const juce::String& labelText);

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