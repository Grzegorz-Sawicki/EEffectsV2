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

  logo = juce::Drawable::createFromImageData (assets::efx_logo_svg, assets::efx_logo_svgSize);

  if (auto* composite = dynamic_cast<juce::DrawableComposite*> (logo.get()))
  {
    composite->setBoundingBox(juce::Rectangle<float> (-2, -2, 161, 78));
  }

  addAndMakeVisible(logo.get());

  setupSlider(gainSlider, gainLabel, "GAIN");
  gainSlider.getProperties().set("isBipolar", true);
  addAndMakeVisible(gainSlider);
  addAndMakeVisible(gainLabel);

  setupSlider(panSlider, panLabel, "PAN");
  panSlider.getProperties().set("isBipolar", true);
  addAndMakeVisible(panSlider);
  addAndMakeVisible(panLabel);

  setupToggleButton(bypassButton, bypassLabel, "BYPASS");
  addAndMakeVisible(bypassButton);
  addAndMakeVisible(bypassLabel);

  addAndMakeVisible(vuMeter);

  addAndMakeVisible(effectRackView);
  addAndMakeVisible(effectDetailView);

  effectRackView.setSelectedItem("Tremolo");

  effectRackView.onEffectChanged = [this](juce::String effectName) {
    effectDetailView.showEditor(effectName);
  };

  effectDetailView.showEditor("Tremolo");

  setLookAndFeel(&lookAndFeel);

  juce::Desktop::getInstance().setGlobalScaleFactor (2.0f);
  setSize(pluginWidth, pluginHeight);
}

PluginEditor::~PluginEditor() {
  setLookAndFeel(nullptr);
}

void PluginEditor::setupSlider(juce::Slider& slider, juce::Label& label, const juce::String& labelText) {
  slider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
  slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
  slider.setPopupDisplayEnabled(true, true, nullptr);
  slider.setColour(custom_colors::highlight, CustomLookAndFeel::getColor(CustomLookAndFeel::Colors::whiteHighlight));

  label.setText(labelText, juce::dontSendNotification);
  label.setInterceptsMouseClicks(false, false);
  label.setJustificationType(juce::Justification::centred);
  label.setColour(juce::Label::textColourId, CustomLookAndFeel::getColor(CustomLookAndFeel::Colors::whiteHighlight));
  label.setFont(CustomLookAndFeel::getInterMediumFont());
}

void PluginEditor::setupToggleButton(juce::ToggleButton& button, juce::Label& label, const juce::String& labelText) {
  label.setText(labelText, juce::dontSendNotification);
  label.setInterceptsMouseClicks(false, false);
  label.setJustificationType(juce::Justification::centred);
  label.setColour(juce::Label::textColourId, CustomLookAndFeel::getColor(CustomLookAndFeel::Colors::whiteHighlight));
  label.setFont(CustomLookAndFeel::getInterMediumFont());
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