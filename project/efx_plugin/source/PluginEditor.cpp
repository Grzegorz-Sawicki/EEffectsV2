namespace efx {
PluginEditor::PluginEditor(PluginProcessor& p) :
  AudioProcessorEditor(&p),
  vuMeter (p.leftPeak, p.rightPeak),
  gainAttachment(p.getParameterRefs().gain, gainSlider),
  panAttachment(p.getParameterRefs().pan, panSlider),
  bypassAttachment(p.getParameterRefs().bypass, bypassButton) {
  logo.setImage(
      juce::ImageCache::getFromMemory(assets::efx_logo_png, assets::efx_logo_pngSize));

  addAndMakeVisible(background);
  addAndMakeVisible(logo);
  addAndMakeVisible(label);

  setSliderDefaults(gainSlider);
  addAndMakeVisible(gainSlider);

  gainLabel.setInterceptsMouseClicks(false, false);
  gainLabel.setJustificationType(juce::Justification::centred);
  gainLabel.setFont(lookAndFeel.getInterMediumFont());
  gainLabel.setColour(juce::Label::textColourId, CustomLookAndFeel::getColor(CustomLookAndFeel::Colors::whiteHighlight));
  addAndMakeVisible(gainLabel);

  setSliderDefaults(panSlider);
  panSlider.getProperties().set("isBipolar", true);
  addAndMakeVisible(panSlider);

  panLabel.setInterceptsMouseClicks(false, false);
  panLabel.setJustificationType(juce::Justification::centred);
  panLabel.setFont(lookAndFeel.getInterMediumFont());
  panLabel.setColour(juce::Label::textColourId, CustomLookAndFeel::getColor(CustomLookAndFeel::Colors::whiteHighlight));
  addAndMakeVisible(panLabel);

  addAndMakeVisible(bypassButton);

  bypassLabel.setInterceptsMouseClicks(false, false);
  bypassLabel.setJustificationType(juce::Justification::centred);
  bypassLabel.setFont(lookAndFeel.getInterMediumFont());
  bypassLabel.setColour(juce::Label::textColourId, CustomLookAndFeel::getColor(CustomLookAndFeel::Colors::whiteHighlight));
  addAndMakeVisible(bypassLabel);

  addAndMakeVisible(vuMeter);

  setLookAndFeel(&lookAndFeel);

  setSize(540, 218);
}

PluginEditor::~PluginEditor() {
  setLookAndFeel(nullptr);
}

void PluginEditor::resized() {
  const auto bounds = getLocalBounds();

  background.setBounds(bounds);
  logo.setBounds({-2, -3, 160, 77});
  label.setBounds({100, 100, 50, 50});

  vuMeter.setBounds({494, 6, 40, 206});

  gainSlider.setTopLeftPosition(333, 12);
  gainLabel.setBounds(gainSlider.getX(), gainSlider.getY() + 37, gainSlider.getWidth(), 15);

  panSlider.setTopLeftPosition(385, 12);
  panLabel.setBounds(panSlider.getX(), panSlider.getY() + 37, panSlider.getWidth(), 15);

  bypassButton.setBounds(443, 18, 28, 28);
  bypassLabel.setSize(60, 15);
  bypassLabel.setCentrePosition(bypassButton.getX() + bypassButton.getWidth() * 0.5f, bypassButton.getBottom() + 10);
}

void PluginEditor::setSliderDefaults(juce::Slider &slider) {
  slider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
  slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
  slider.setPopupDisplayEnabled(true, true, this);
  slider.setSize(40, 40);
}

}  // namespace efx
