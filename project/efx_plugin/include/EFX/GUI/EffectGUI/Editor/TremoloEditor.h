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
} // namespace efx