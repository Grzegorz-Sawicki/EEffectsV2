#pragma once

namespace efx {
class TremoloEditor : public EffectEditorBase {
public:
  explicit TremoloEditor(PluginProcessor &p) :
      activeAttachment(p.getParameterRefs().tremoloActive, activeButton),
      mixAttachment(p.getParameterRefs().tremoloMix, mixSlider),
      depthAttachment(p.getParameterRefs().tremoloDepth, depthSlider),
      rateAttachment(p.getParameterRefs().tremoloRate, rateSlider),
      waveformAttachment(p.getParameterRefs().tremoloWaveform, waveformComboBox) {
    mainColor = CustomLookAndFeel::getColor(CustomLookAndFeel::Colors::tremoloHighlight);

    CustomLookAndFeel::setupLabel(logoLabel, logoFont, mainColor);
    addAndMakeVisible(logoLabel);

    CustomLookAndFeel::setupActiveButton(activeButton, mainColor);
    addAndMakeVisible(activeButton);

    CustomLookAndFeel::setupSlider(mixSlider, false, mainColor);
    CustomLookAndFeel::setupLabel(mixLabel, labelFont, mainColor);
    addAndMakeVisible(mixSlider);
    addAndMakeVisible(mixLabel);

    CustomLookAndFeel::setupSlider(depthSlider, false, mainColor);
    CustomLookAndFeel::setupLabel(depthLabel, labelFont, mainColor);
    addAndMakeVisible(depthSlider);
    addAndMakeVisible(depthLabel);

    CustomLookAndFeel::setupSlider(rateSlider, false, mainColor);
    CustomLookAndFeel::setupLabel(rateLabel, labelFont, mainColor);
    addAndMakeVisible(rateSlider);
    addAndMakeVisible(rateLabel);

    CustomLookAndFeel::setupComboBox(waveformComboBox, mainColor);
    CustomLookAndFeel::setupLabel(waveformLabel, labelFont, mainColor);
    waveformComboBox.addItemList(p.getParameterRefs().tremoloWaveform.choices, 1);
    waveformAttachment.sendInitialUpdate();

    addAndMakeVisible(waveformComboBox);
    addAndMakeVisible(waveformLabel);
  }

  void paint(juce::Graphics &g) override {
    auto bounds = getLocalBounds().toFloat();

    g.setColour(mainColor);
    g.fillRect(bounds);

    auto innerBounds = bounds.reduced(lineThickness);
    g.setColour(CustomLookAndFeel::getColor(CustomLookAndFeel::Colors::effectBackground));
    g.fillRect(innerBounds);

    g.setColour(mainColor);
    g.drawLine(12.0f, 129.0f, 240.0f, 129.0f, lineThickness);
    g.drawLine(239.0f, 129.0f, 239.0f, 13.0f, lineThickness);
    g.drawLine(238.0f, 13.0f, 343.0f, 13.0f, lineThickness);

    g.drawLine(12.0f, 13.0f, 126.0f, 13.0f, lineThickness);
    g.drawLine(12.0f, 37.0f, 126.0f, 37.0f, lineThickness);
  }

  void resized() override {
    constexpr int logoWidth = 114;
    constexpr int logoHeight = 29;

    logoLabel.setBounds(12, 10, logoWidth, logoHeight);

    activeButton.setBounds(305, 50, GUI::textButtonWidth, GUI::textButtonHeight);

    mixSlider.setBounds(259, 46, GUI::smallSliderSize, GUI::smallSliderSize);
    mixLabel.setBounds(259, 79, mixSlider.getWidth(), GUI::labelHeight);

    depthSlider.setBounds(185, 46, GUI::smallSliderSize, GUI::smallSliderSize);
    depthLabel.setBounds(182, 79, depthSlider.getWidth() + 6, GUI::labelHeight);

    rateSlider.setBounds(139, 46, GUI::smallSliderSize, GUI::smallSliderSize);
    rateLabel.setBounds(139, 79, rateSlider.getWidth(), GUI::labelHeight);

    waveformComboBox.setBounds(53, 52, GUI::comboBoxWidth, GUI::comboBoxHeight);
    waveformLabel.setBounds(53, 79, waveformComboBox.getWidth(), GUI::labelHeight);
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
} // namespace efx