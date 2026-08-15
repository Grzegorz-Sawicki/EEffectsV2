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
    CustomLookAndFeel::setupLabel(lowLogoLabel, logoFont, mainColor);
    addAndMakeVisible(lowLogoLabel);

    CustomLookAndFeel::setupActiveButton(lowActiveButton, mainColor);
    addAndMakeVisible(lowActiveButton);

    CustomLookAndFeel::setupSlider(lowMixSlider, false, mainColor);
    CustomLookAndFeel::setupLabel(lowMixLabel, labelFont, mainColor);
    addAndMakeVisible(lowMixSlider);
    addAndMakeVisible(lowMixLabel);

    CustomLookAndFeel::setupSlider(lowResonanceSlider, false, mainColor);
    CustomLookAndFeel::setupLabel(lowResonanceLabel, labelFont, mainColor);
    addAndMakeVisible(lowResonanceSlider);
    addAndMakeVisible(lowResonanceLabel);

    CustomLookAndFeel::setupSlider(lowFreqSlider, false, mainColor);
    CustomLookAndFeel::setupLabel(lowFreqLabel, labelFont, mainColor);
    addAndMakeVisible(lowFreqSlider);
    addAndMakeVisible(lowFreqLabel);

    // ==========================================
    // Bandpass Setup
    // ==========================================
    CustomLookAndFeel::setupLabel(bandLogoLabel, logoFont, mainColor);
    addAndMakeVisible(bandLogoLabel);

    CustomLookAndFeel::setupActiveButton(bandActiveButton, mainColor);
    addAndMakeVisible(bandActiveButton);

    CustomLookAndFeel::setupSlider(bandMixSlider, false, mainColor);
    CustomLookAndFeel::setupLabel(bandMixLabel, labelFont, mainColor);
    addAndMakeVisible(bandMixSlider);
    addAndMakeVisible(bandMixLabel);

    CustomLookAndFeel::setupSlider(bandResonanceSlider, false, mainColor);
    CustomLookAndFeel::setupLabel(bandResonanceLabel, labelFont, mainColor);
    addAndMakeVisible(bandResonanceSlider);
    addAndMakeVisible(bandResonanceLabel);

    CustomLookAndFeel::setupSlider(bandFreqSlider, false, mainColor);
    CustomLookAndFeel::setupLabel(bandFreqLabel, labelFont, mainColor);
    addAndMakeVisible(bandFreqSlider);
    addAndMakeVisible(bandFreqLabel);

    // ==========================================
    // Highpass Setup
    // ==========================================
    CustomLookAndFeel::setupLabel(highLogoLabel, logoFont, mainColor);
    addAndMakeVisible(highLogoLabel);

    CustomLookAndFeel::setupActiveButton(highActiveButton, mainColor);
    addAndMakeVisible(highActiveButton);

    CustomLookAndFeel::setupSlider(highMixSlider, false, mainColor);
    CustomLookAndFeel::setupLabel(highMixLabel, labelFont, mainColor);
    addAndMakeVisible(highMixSlider);
    addAndMakeVisible(highMixLabel);

    CustomLookAndFeel::setupSlider(highResonanceSlider, false, mainColor);
    CustomLookAndFeel::setupLabel(highResonanceLabel, labelFont, mainColor);
    addAndMakeVisible(highResonanceSlider);
    addAndMakeVisible(highResonanceLabel);

    CustomLookAndFeel::setupSlider(highFreqSlider, false, mainColor);
    CustomLookAndFeel::setupLabel(highFreqLabel, labelFont, mainColor);
    addAndMakeVisible(highFreqSlider);
    addAndMakeVisible(highFreqLabel);
  }

  void paint(juce::Graphics &g) override {
    auto bounds = getLocalBounds().toFloat();

    g.setColour(mainColor);
    g.fillRect(bounds);

    auto innerBounds = bounds.reduced(lineThickness);
    g.setColour(CustomLookAndFeel::getColor(CustomLookAndFeel::Colors::effectBackground));
    g.fillRect(innerBounds);

    g.setColour(mainColor);

    // Lowpass Lines
    g.drawLine(12.0f, 12.0f, 12.0f, 130.0f, lineThickness);
    g.drawLine(44.0f, 62.0f, 96.0f, 62.0f, lineThickness);
    g.drawLine(44.0f, 81.0f, 96.0f, 81.0f, lineThickness);

    // Bandpass Lines
    g.drawLine(126.0f, 12.0f, 126.0f, 130.0f, lineThickness);
    g.drawLine(158.0f, 62.0f, 210.0f, 62.0f, lineThickness);
    g.drawLine(158.0f, 81.0f, 210.0f, 81.0f, lineThickness);

    // Highpass Lines
    g.drawLine(240.0f, 12.0f, 240.0f, 130.0f, lineThickness);
    g.drawLine(272.0f, 62.0f, 324.0f, 62.0f, lineThickness);
    g.drawLine(272.0f, 81.0f, 324.0f, 81.0f, lineThickness);
  }

  void resized() override {
    constexpr int logoWidth = 52;
    constexpr int logoHeight = 20;

    // ==========================================
    // Lowpass Bounds
    // ==========================================
    lowLogoLabel.setBounds(44, 61, logoWidth, logoHeight);

    lowFreqSlider.setBounds(29, 10, GUI::smallSliderSize, GUI::smallSliderSize);
    lowFreqLabel.setBounds(29, 43, lowFreqSlider.getWidth(), GUI::labelHeight);

    lowResonanceSlider.setBounds(77, 10, GUI::smallSliderSize, GUI::smallSliderSize);
    lowResonanceLabel.setBounds(77, 43, lowResonanceSlider.getWidth(), GUI::labelHeight);

    lowMixSlider.setBounds(29, 87, GUI::smallSliderSize, GUI::smallSliderSize);
    lowMixLabel.setBounds(29, 120, lowMixSlider.getWidth(), GUI::labelHeight);

    lowActiveButton.setBounds(75, 91, GUI::textButtonWidth, GUI::textButtonHeight);

    // ==========================================
    // Bandpass Bounds
    // ==========================================
    bandLogoLabel.setBounds(158, 61, logoWidth, logoHeight);

    bandFreqSlider.setBounds(143, 10, GUI::smallSliderSize, GUI::smallSliderSize);
    bandFreqLabel.setBounds(143, 43, bandFreqSlider.getWidth(), GUI::labelHeight);

    bandResonanceSlider.setBounds(191, 10, GUI::smallSliderSize, GUI::smallSliderSize);
    bandResonanceLabel.setBounds(191, 43, bandResonanceSlider.getWidth(), GUI::labelHeight);

    bandMixSlider.setBounds(143, 87, GUI::smallSliderSize, GUI::smallSliderSize);
    bandMixLabel.setBounds(143, 120, bandMixSlider.getWidth(), GUI::labelHeight);

    bandActiveButton.setBounds(189, 91, GUI::textButtonWidth, GUI::textButtonHeight);

    // ==========================================
    // Highpass Bounds
    // ==========================================
    highLogoLabel.setBounds(272, 61, logoWidth, logoHeight);

    highFreqSlider.setBounds(257, 10, GUI::smallSliderSize, GUI::smallSliderSize);
    highFreqLabel.setBounds(257, 43, highFreqSlider.getWidth(), GUI::labelHeight);

    highResonanceSlider.setBounds(305, 10, GUI::smallSliderSize, GUI::smallSliderSize);
    highResonanceLabel.setBounds(305, 43, highResonanceSlider.getWidth(), GUI::labelHeight);

    highMixSlider.setBounds(257, 87, GUI::smallSliderSize, GUI::smallSliderSize);
    highMixLabel.setBounds(257, 120, highMixSlider.getWidth(), GUI::labelHeight);

    highActiveButton.setBounds(303, 91, GUI::textButtonWidth, GUI::textButtonHeight);
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