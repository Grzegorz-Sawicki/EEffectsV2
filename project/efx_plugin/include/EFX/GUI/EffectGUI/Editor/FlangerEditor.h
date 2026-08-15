#pragma once

namespace efx {
class FlangerEditor : public EffectEditorBase {
public:
  explicit FlangerEditor(PluginProcessor &p) :
      activeAttachment(p.getParameterRefs().flangerActive, activeButton),
      mixAttachment(p.getParameterRefs().flangerMix, mixSlider),
      depthAttachment(p.getParameterRefs().flangerDepth, depthSlider),
      rateAttachment(p.getParameterRefs().flangerRate, rateSlider),
      feedbackAttachment(p.getParameterRefs().flangerFeedback, feedbackSlider) {
    mainColor = CustomLookAndFeel::getColor(CustomLookAndFeel::Colors::flangerHighlight);

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

    CustomLookAndFeel::setupSlider(feedbackSlider, true, mainColor);
    CustomLookAndFeel::setupLabel(feedbackLabel, labelFont, mainColor);
    addAndMakeVisible(feedbackSlider);
    addAndMakeVisible(feedbackLabel);
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

    depthSlider.setBounds(185, 23, 34, 34);
    depthLabel.setBounds(183, 56, 40, 15);

    rateSlider.setBounds(139, 23, 34, 34);
    rateLabel.setBounds(139, 56, 34, 15);

    feedbackSlider.setBounds(162, 74, 34, 34);
    feedbackLabel.setBounds(142, 107, 74, 15);
  }

private:
  juce::Label logoLabel{"flangerLogoLabel", "FLANGER"};

  juce::TextButton activeButton;
  juce::ButtonParameterAttachment activeAttachment;

  juce::Slider mixSlider;
  juce::Label mixLabel{"flangerMixLabel", "MIX"};
  juce::SliderParameterAttachment mixAttachment;

  juce::Slider depthSlider;
  juce::Label depthLabel{"flangerDepthLabel", "DEPTH"};
  juce::SliderParameterAttachment depthAttachment;

  juce::Slider rateSlider;
  juce::Label rateLabel{"flangerRateLabel", "RATE"};
  juce::SliderParameterAttachment rateAttachment;

  juce::Slider feedbackSlider;
  juce::Label feedbackLabel{"flangerFeedbackLabel", "FEEDBACK"};
  juce::SliderParameterAttachment feedbackAttachment;
};
} // namespace efx