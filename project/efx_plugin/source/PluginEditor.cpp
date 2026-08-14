namespace efx {
PluginEditor::PluginEditor(PluginProcessor& p) :
  AudioProcessorEditor(&p),
  background(CustomLookAndFeel::getColor(CustomLookAndFeel::Colors::background)),
  effectDetailView(p),
  effectRackView(p),
  vuMeter (p.leftPeak, p.rightPeak),
  gainAttachment(p.getParameterRefs().gain, gainLabeledSlider.slider),
  panAttachment(p.getParameterRefs().pan, panLabeledSlider.slider),
  bypassAttachment(p.getParameterRefs().bypass, bypassLabeledButton.button) {
  addAndMakeVisible(background);

  logo = juce::Drawable::createFromImageData (assets::efx_logo_svg, assets::efx_logo_svgSize);

  if (auto* composite = dynamic_cast<juce::DrawableComposite*> (logo.get()))
  {
    composite->setBoundingBox(juce::Rectangle<float> (-2, -2, 161, 78));
  }

  addAndMakeVisible(logo.get());

  gainLabeledSlider.slider.setColour(custom_colors::highlight, CustomLookAndFeel::getColor(CustomLookAndFeel::Colors::whiteHighlight));
  gainLabeledSlider.slider.getProperties().set("isBipolar", true);
  gainLabeledSlider.label.setColour(juce::Label::textColourId, CustomLookAndFeel::getColor(CustomLookAndFeel::Colors::whiteHighlight));
  addAndMakeVisible(gainLabeledSlider);

  panLabeledSlider.slider.setColour(custom_colors::highlight, CustomLookAndFeel::getColor(CustomLookAndFeel::Colors::whiteHighlight));
  panLabeledSlider.slider.getProperties().set("isBipolar", true);
  panLabeledSlider.label.setColour(juce::Label::textColourId, CustomLookAndFeel::getColor(CustomLookAndFeel::Colors::whiteHighlight));
  addAndMakeVisible(panLabeledSlider);

  bypassLabeledButton.label.setColour(juce::Label::textColourId, CustomLookAndFeel::getColor(CustomLookAndFeel::Colors::whiteHighlight));
  addAndMakeVisible(bypassLabeledButton);

  addAndMakeVisible(vuMeter);

  addAndMakeVisible(effectRackView);
  addAndMakeVisible(effectDetailView);

  effectRackView.setSelectedItem("Tremolo");

  effectRackView.onEffectChanged = [this](juce::String effectName) {
    effectDetailView.showEditor(effectName);
  };

  effectDetailView.showEditor("Tremolo");

  setLookAndFeel(&lookAndFeel);

  const int width = 540;
  const int height = 218;

  juce::Desktop::getInstance().setGlobalScaleFactor (2.0f);
  setSize(width, height);
}

PluginEditor::~PluginEditor() {
  setLookAndFeel(nullptr);
}

void PluginEditor::resized() {
  const auto bounds = getLocalBounds();

  background.setBounds(bounds);

  vuMeter.setBounds({494, 6, 40, 206});

  gainLabeledSlider.setBounds(330, 12, 50, 55);

  panLabeledSlider.setBounds(380, 12, 50, 55);

  bypassLabeledButton.setBounds(430, 12, 50, 55);

  effectRackView.setBounds(6, 70, 122, 142);

  effectDetailView.setBounds(130, 70, 352, 142);
}
}  // namespace efx
