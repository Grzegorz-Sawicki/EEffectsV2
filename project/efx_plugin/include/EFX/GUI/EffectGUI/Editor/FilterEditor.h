#pragma once

namespace efx {
class FilterEditor : public EffectEditorBase {
public:
  explicit FilterEditor(PluginProcessor &p) :
  // Lowpass Attachments
      lowActiveAttachment(p.getParameterRefs().lowpassActive, lowActiveButton),
      lowMixAttachment(p.getParameterRefs().lowpassMix, lowMixSlider),
      lowResonanceAttachment(p.getParameterRefs().lowpassResonance, lowResonanceSlider),
      lowFreqAttachment(p.getParameterRefs().lowpassFrequency, lowFreqSlider),

      // Bandpass Attachments
      bandActiveAttachment(p.getParameterRefs().bandpassActive, bandActiveButton),
      bandMixAttachment(p.getParameterRefs().bandpassMix, bandMixSlider),
      bandResonanceAttachment(p.getParameterRefs().bandpassResonance, bandResonanceSlider),
      bandFreqAttachment(p.getParameterRefs().bandpassFrequency, bandFreqSlider),

      // Highpass Attachments
      highActiveAttachment(p.getParameterRefs().highpassActive, highActiveButton),
      highMixAttachment(p.getParameterRefs().highpassMix, highMixSlider),
      highResonanceAttachment(p.getParameterRefs().highpassResonance, highResonanceSlider),
      highFreqAttachment(p.getParameterRefs().highpassFrequency, highFreqSlider) {

    mainColor = CustomLookAndFeel::getColor(CustomLookAndFeel::Colors::filterHighlight);
    logoFont = CustomLookAndFeel::getOrbitronMediumFont().withPointHeight(16.0f);

    // ==========================================
    // Lowpass Setup
    // ==========================================
    setupLabel(lowLogoLabel, logoFont);
    addAndMakeVisible(lowLogoLabel);

    setupActiveButton(lowActiveButton);
    addAndMakeVisible(lowActiveButton);

    setupSlider(lowMixSlider);
    setupLabel(lowMixLabel, labelFont);
    addAndMakeVisible(lowMixSlider);
    addAndMakeVisible(lowMixLabel);

    setupSlider(lowResonanceSlider);
    setupLabel(lowResonanceLabel, labelFont);
    addAndMakeVisible(lowResonanceSlider);
    addAndMakeVisible(lowResonanceLabel);

    setupSlider(lowFreqSlider);
    setupLabel(lowFreqLabel, labelFont);
    addAndMakeVisible(lowFreqSlider);
    addAndMakeVisible(lowFreqLabel);

    // ==========================================
    // Bandpass Setup
    // ==========================================
    setupLabel(bandLogoLabel, logoFont);
    addAndMakeVisible(bandLogoLabel);

    setupActiveButton(bandActiveButton);
    addAndMakeVisible(bandActiveButton);

    setupSlider(bandMixSlider);
    setupLabel(bandMixLabel, labelFont);
    addAndMakeVisible(bandMixSlider);
    addAndMakeVisible(bandMixLabel);

    setupSlider(bandResonanceSlider);
    setupLabel(bandResonanceLabel, labelFont);
    addAndMakeVisible(bandResonanceSlider);
    addAndMakeVisible(bandResonanceLabel);

    setupSlider(bandFreqSlider);
    setupLabel(bandFreqLabel, labelFont);
    addAndMakeVisible(bandFreqSlider);
    addAndMakeVisible(bandFreqLabel);

    // ==========================================
    // Highpass Setup
    // ==========================================
    setupLabel(highLogoLabel, logoFont);
    addAndMakeVisible(highLogoLabel);

    setupActiveButton(highActiveButton);
    addAndMakeVisible(highActiveButton);

    setupSlider(highMixSlider);
    setupLabel(highMixLabel, labelFont);
    addAndMakeVisible(highMixSlider);
    addAndMakeVisible(highMixLabel);

    setupSlider(highResonanceSlider);
    setupLabel(highResonanceLabel, labelFont);
    addAndMakeVisible(highResonanceSlider);
    addAndMakeVisible(highResonanceLabel);

    setupSlider(highFreqSlider);
    setupLabel(highFreqLabel, labelFont);
    addAndMakeVisible(highFreqSlider);
    addAndMakeVisible(highFreqLabel);
  }

  void paint(juce::Graphics &g) override {
    auto bounds = getLocalBounds().toFloat();

    g.setColour(mainColor);
    g.fillRect(bounds);

    auto innerBounds = bounds.reduced(2.0f);
    g.setColour(CustomLookAndFeel::getColor(CustomLookAndFeel::Colors::effectBackground));
    g.fillRect(innerBounds);

    g.setColour(mainColor);

    // Lowpass Lines
    g.drawLine(12.0f, 12.0f, 12.0f, 130.0f, 2.0f);
    g.drawLine(44.0f, 62.0f, 96.0f, 62.0f, 2.0f);
    g.drawLine(44.0f, 81.0f, 96.0f, 81.0f, 2.0f);

    // Bandpass Lines
    g.drawLine(126.0f, 12.0f, 126.0f, 130.0f, 2.0f);
    g.drawLine(158.0f, 62.0f, 210.0f, 62.0f, 2.0f);
    g.drawLine(158.0f, 81.0f, 210.0f, 81.0f, 2.0f);

    // Highpass Lines
    g.drawLine(240.0f, 12.0f, 240.0f, 130.0f, 2.0f);
    g.drawLine(272.0f, 62.0f, 324.0f, 62.0f, 2.0f);
    g.drawLine(272.0f, 81.0f, 324.0f, 81.0f, 2.0f);
  }

  void resized() override {
    // ==========================================
    // Lowpass Bounds
    // ==========================================
    lowLogoLabel.setBounds(44, 61, 52, 20);

    lowFreqSlider.setBounds(29, 10, 34, 34);
    lowFreqLabel.setBounds(29, 43, 34, 15);

    lowResonanceSlider.setBounds(77, 10, 34, 34);
    lowResonanceLabel.setBounds(77, 43, 34, 15);

    lowMixSlider.setBounds(29, 87, 34, 34);
    lowMixLabel.setBounds(29, 120, 34, 15);

    lowActiveButton.setBounds(75, 91, 38, 26);

    // ==========================================
    // Bandpass Bounds
    // ==========================================
    bandLogoLabel.setBounds(158, 61, 52, 20);

    bandFreqSlider.setBounds(143, 10, 34, 34);
    bandFreqLabel.setBounds(143, 43, 34, 15);

    bandResonanceSlider.setBounds(191, 10, 34, 34);
    bandResonanceLabel.setBounds(191, 43, 34, 15);

    bandMixSlider.setBounds(143, 87, 34, 34);
    bandMixLabel.setBounds(143, 120, 34, 15);

    bandActiveButton.setBounds(189, 91, 38, 26);

    // ==========================================
    // Highpass Bounds
    // ==========================================
    highLogoLabel.setBounds(272, 61, 52, 20);

    highFreqSlider.setBounds(257, 10, 34, 34);
    highFreqLabel.setBounds(257, 43, 34, 15);

    highResonanceSlider.setBounds(305, 10, 34, 34);
    highResonanceLabel.setBounds(305, 43, 34, 15);

    highMixSlider.setBounds(257, 87, 34, 34);
    highMixLabel.setBounds(257, 120, 34, 15);

    highActiveButton.setBounds(303, 91, 38, 26);
  }

private:
  // Lowpass Components
  juce::Label lowLogoLabel{"lowpassLogoLabel", "LOW"};

  juce::TextButton lowActiveButton;
  juce::ButtonParameterAttachment lowActiveAttachment;

  juce::Slider lowMixSlider;
  juce::Label lowMixLabel{"lowpassMixLabel", "MIX"};
  juce::SliderParameterAttachment lowMixAttachment;

  juce::Slider lowResonanceSlider;
  juce::Label lowResonanceLabel{"lowpassResonanceLabel", "Q"};
  juce::SliderParameterAttachment lowResonanceAttachment;

  juce::Slider lowFreqSlider;
  juce::Label lowFreqLabel{"lowpassFreqLabel", "FREQ"};
  juce::SliderParameterAttachment lowFreqAttachment;

  // Bandpass Components
  juce::Label bandLogoLabel{"bandpassLogoLabel", "BAND"};

  juce::TextButton bandActiveButton;
  juce::ButtonParameterAttachment bandActiveAttachment;

  juce::Slider bandMixSlider;
  juce::Label bandMixLabel{"bandpassMixLabel", "MIX"};
  juce::SliderParameterAttachment bandMixAttachment;

  juce::Slider bandResonanceSlider;
  juce::Label bandResonanceLabel{"bandpassResonanceLabel", "Q"};
  juce::SliderParameterAttachment bandResonanceAttachment;

  juce::Slider bandFreqSlider;
  juce::Label bandFreqLabel{"bandpassFreqLabel", "FREQ"};
  juce::SliderParameterAttachment bandFreqAttachment;

  // Highpass Components
  juce::Label highLogoLabel{"highpassLogoLabel", "HIGH"};

  juce::TextButton highActiveButton;
  juce::ButtonParameterAttachment highActiveAttachment;

  juce::Slider highMixSlider;
  juce::Label highMixLabel{"highpassMixLabel", "MIX"};
  juce::SliderParameterAttachment highMixAttachment;

  juce::Slider highResonanceSlider;
  juce::Label highResonanceLabel{"highpassResonanceLabel", "Q"};
  juce::SliderParameterAttachment highResonanceAttachment;

  juce::Slider highFreqSlider;
  juce::Label highFreqLabel{"highpassFreqLabel", "FREQ"};
  juce::SliderParameterAttachment highFreqAttachment;
};
} // namespace efx