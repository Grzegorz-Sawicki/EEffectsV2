namespace efx {
PluginEditor::PluginEditor(PluginProcessor& p) :
    AudioProcessorEditor(&p),
    effectDetailView(p),
    effectRackView(p),
    vuMeter (p.leftPeak, p.rightPeak),
    gainAttachment(p.getParameterRefs().gain, gainSlider),
    panAttachment(p.getParameterRefs().pan, panSlider),
    bypassAttachment(p.getParameterRefs().bypass, bypassButton) {

  constexpr int pluginWidth = 540;
  constexpr int pluginHeight = 218;
  constexpr float scaleFactor = 2.0f;

  constexpr float logoX = -2.0f;
  constexpr float logoY = -2.0f;
  constexpr float logoWidth = 161.0f;
  constexpr float logoHeight = 78.0f;

  // Logo
  logo = juce::Drawable::createFromImageData (assets::efx_logo_svg, assets::efx_logo_svgSize);

  if (auto* composite = dynamic_cast<juce::DrawableComposite*> (logo.get()))
  {
    composite->setBoundingBox(juce::Rectangle<float> (logoX, logoY, logoWidth, logoHeight));
  }

  addAndMakeVisible(logo.get());

  // Gain Slider
  CustomLookAndFeel::setupSlider(gainSlider, true, mainColor);
  CustomLookAndFeel::setupLabel(gainLabel, labelFont, mainColor);
  addAndMakeVisible(gainSlider);
  addAndMakeVisible(gainLabel);

  // Pan Slider
  CustomLookAndFeel::setupSlider(panSlider, true, mainColor);
  CustomLookAndFeel::setupLabel(panLabel, labelFont, mainColor);
  addAndMakeVisible(panSlider);
  addAndMakeVisible(panLabel);

  // Bypass Button
  CustomLookAndFeel::setupLabel(bypassLabel, labelFont, mainColor);
  addAndMakeVisible(bypassButton);
  addAndMakeVisible(bypassLabel);

  // Effect Rack View
  effectRackView.setSelectedItem("Tremolo");

  effectRackView.onEffectChanged = [this](juce::String effectName) {
    effectDetailView.showEditor(effectName);
  };

  effectDetailView.showEditor("Tremolo");

  addAndMakeVisible(effectRackView);

  addAndMakeVisible(vuMeter);

  addAndMakeVisible(effectDetailView);

  setLookAndFeel(&lookAndFeel);

  juce::Desktop::getInstance().setGlobalScaleFactor (scaleFactor);
  setSize(pluginWidth, pluginHeight);
}

PluginEditor::~PluginEditor() {
  setLookAndFeel(nullptr);
}

void PluginEditor::paint(juce::Graphics &g) {
  g.setColour(CustomLookAndFeel::getColor(CustomLookAndFeel::Colors::background));
  g.fillAll();
}

void PluginEditor::resized() {
  vuMeter.setBounds(494, 6, 40, 206);

  gainSlider.setBounds(333, 12, 40, 40);
  gainLabel.setBounds(328, 50, 50, 15);

  panSlider.setBounds(385, 12, 40, 40);
  panLabel.setBounds(380, 50, 50, 15);

  bypassButton.setBounds(442, 17, 30, 30);
  bypassLabel.setBounds(432, 50, 50, 15);

  effectRackView.setBounds(6, 70, 122, 142);

  effectDetailView.setBounds(133, 70, 355, 142);
}
}  // namespace efx