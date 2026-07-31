namespace efx {
PluginEditor::PluginEditor(PluginProcessor& p) :
  AudioProcessorEditor(&p),
  vuMeter (p.leftPeak, p.rightPeak),
  gainAttachment(p.getParameterRefs().gain, gainLabeledSlider.slider),
  panAttachment(p.getParameterRefs().pan, panLabeledSlider.slider),
  bypassAttachment(p.getParameterRefs().bypass, bypassButton) {
  logo.setImage(
      juce::ImageCache::getFromMemory(assets::efx_logo_png, assets::efx_logo_pngSize));

  addAndMakeVisible(background);
  addAndMakeVisible(logo);
  addAndMakeVisible(label);

  addAndMakeVisible(gainLabeledSlider);

  addAndMakeVisible(panLabeledSlider);

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

  gainLabeledSlider.setBounds(333, 12, 40, 60);

  panLabeledSlider.setBounds(385, 12, 40, 60);

  bypassButton.setBounds(443, 18, 28, 28);
  bypassLabel.setSize(60, 15);
  bypassLabel.setCentrePosition(bypassButton.getX() + bypassButton.getWidth() * 0.5f, bypassButton.getBottom() + 10);
}
}  // namespace efx
