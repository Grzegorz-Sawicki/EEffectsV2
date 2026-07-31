namespace efx {
PluginEditor::PluginEditor(PluginProcessor& p) :
  AudioProcessorEditor(&p),
  vuMeter (p.leftPeak, p.rightPeak),
  gainAttachment(p.getParameterRefs().gain, gainLabeledSlider.slider),
  panAttachment(p.getParameterRefs().pan, panLabeledSlider.slider),
  bypassAttachment(p.getParameterRefs().bypass, bypassLabeledButton.button) {
  logo.setImage(
      juce::ImageCache::getFromMemory(assets::efx_logo_png, assets::efx_logo_pngSize));

  addAndMakeVisible(background);
  addAndMakeVisible(logo);
  addAndMakeVisible(label);

  gainLabeledSlider.slider.setColour(custom_colors::highlight, CustomLookAndFeel::getColor(CustomLookAndFeel::Colors::whiteHighlight));
  gainLabeledSlider.slider.getProperties().set("isBipolar", true);
  gainLabeledSlider.label.setColour(juce::Label::textColourId, CustomLookAndFeel::getColor(CustomLookAndFeel::Colors::whiteHighlight));
  addAndMakeVisible(gainLabeledSlider);

  panLabeledSlider.slider.setColour(custom_colors::highlight, CustomLookAndFeel::getColor(CustomLookAndFeel::Colors::whiteHighlight));
  panLabeledSlider.slider.getProperties().set("isBipolar", true);
  panLabeledSlider.label.setColour(juce::Label::textColourId, CustomLookAndFeel::getColor(CustomLookAndFeel::Colors::whiteHighlight));
  addAndMakeVisible(panLabeledSlider);

  bypassLabeledButton.setColour(juce::Label::textColourId, CustomLookAndFeel::getColor(CustomLookAndFeel::Colors::whiteHighlight));
  addAndMakeVisible(bypassLabeledButton);

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

  gainLabeledSlider.setBounds(330, 12, 50, 55);

  panLabeledSlider.setBounds(380, 12, 50, 55);

  bypassLabeledButton.setBounds(430, 12, 50, 55);
}
}  // namespace efx
