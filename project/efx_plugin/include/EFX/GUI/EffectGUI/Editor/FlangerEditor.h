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

    activeButton.setBounds(305, 50, gui::textButtonWidth, gui::textButtonHeight);

    mixSlider.setBounds(259, 46, gui::smallSliderSize, gui::smallSliderSize);
    mixLabel.setBounds(259, 79, mixSlider.getWidth(), gui::labelHeight);

    depthSlider.setBounds(185, 23, gui::smallSliderSize, gui::smallSliderSize);
    depthLabel.setBounds(182, 56, depthSlider.getWidth() + 6, gui::labelHeight);

    rateSlider.setBounds(139, 23, gui::smallSliderSize, gui::smallSliderSize);
    rateLabel.setBounds(139, 56, rateSlider.getWidth(), gui::labelHeight);

    feedbackSlider.setBounds(162, 74, gui::smallSliderSize, gui::smallSliderSize);
    feedbackLabel.setBounds(142, 107, feedbackSlider.getWidth() + 40, gui::labelHeight);
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