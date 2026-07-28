namespace efx {
PluginEditor::PluginEditor(PluginProcessor& p) : AudioProcessorEditor(&p), vuMeter (p.leftPeak, p.rightPeak) {
  logo.setImage(
      juce::ImageCache::getFromMemory(assets::efx_logo_png, assets::efx_logo_pngSize));

  addAndMakeVisible(background);
  addAndMakeVisible(logo);
  addAndMakeVisible(label);

  setSliderDefaults(gainSlider);
  addAndMakeVisible(gainSlider);

  addAndMakeVisible(vuMeter);

  setLookAndFeel(&lookAndFeel);

  // Make sure that before the constructor has finished, you've set the
  // editor's size to whatever you need it to be.
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
}

void PluginEditor::setSliderDefaults(juce::Slider &slider) {
  slider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
  slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
  slider.setPopupDisplayEnabled(true, true, this);
  slider.setSize(40, 40);
}

}  // namespace efx
