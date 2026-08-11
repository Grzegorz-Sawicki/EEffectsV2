namespace efx {
PluginProcessor::PluginProcessor()
    : gain(juce::dsp::get<gainIndex>(processorChain)),
      pan(juce::dsp::get<panIndex>(processorChain)),
      tremolo(juce::dsp::get<tremoloIndex>(processorChain)),
      flanger(juce::dsp::get<flangerIndex>(processorChain)),
      lowpass(juce::dsp::get<lowpassIndex>(processorChain)),
      highpass(juce::dsp::get<highpassIndex>(processorChain)),
      bandpass(juce::dsp::get<bandpassIndex>(processorChain)),
      AudioProcessor(
          BusesProperties()
              .withInput("Input", juce::AudioChannelSet::stereo(), true)
              .withOutput("Output", juce::AudioChannelSet::stereo(), true)) {}

const juce::String PluginProcessor::getName() const {
  return PLUGIN_NAME;
}

bool PluginProcessor::acceptsMidi() const {
  return false;
}

bool PluginProcessor::producesMidi() const {
  return false;
}

bool PluginProcessor::isMidiEffect() const {
  return false;
}

double PluginProcessor::getTailLengthSeconds() const {
  return 0.0;
}

int PluginProcessor::getNumPrograms() {
  return 1;
}

int PluginProcessor::getCurrentProgram() {
  return 0;
}

void PluginProcessor::setCurrentProgram(int index) {
  juce::ignoreUnused(index);
}

const juce::String PluginProcessor::getProgramName(int index) {
  juce::ignoreUnused(index);
  return "None";
}

void PluginProcessor::changeProgramName(int index,
                                        const juce::String &newName) {
  juce::ignoreUnused(index, newName);
}

void PluginProcessor::prepareToPlay(double sampleRate,
                                    int expectedMaxFramesPerBlock) {
  const auto numChannels = juce::jmax(getTotalNumInputChannels(), getTotalNumOutputChannels());

  auto spec = juce::dsp::ProcessSpec(
      sampleRate,
      expectedMaxFramesPerBlock,
      numChannels
  );

  processorChain.prepare(spec);
}

void PluginProcessor::releaseResources() {
  processorChain.reset();
}

bool PluginProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const {
  if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() &&
      layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo()) {
    return false;
  }

  if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet()) {
    return false;
  }

  return true;
}

void PluginProcessor::processBlock(juce::AudioBuffer<float> &buffer,
                                   juce::MidiBuffer &midiMessages) {
  juce::ignoreUnused(midiMessages);

  juce::ScopedNoDenormals noDenormals;
  const auto inChannels = getTotalNumInputChannels();
  const auto outChannels = getTotalNumOutputChannels();
  const auto numSamples = buffer.getNumSamples();

  for (const auto channelToClear:
      std::views::iota(inChannels, outChannels)) {
    buffer.clear(channelToClear, 0, buffer.getNumSamples());
  }

  updateParameters();
  if (parameters.bypass.get()) {
    return;
  }

  const auto numChannels = juce::jmax(inChannels, outChannels);
  auto inoutBlock = juce::dsp::AudioBlock<float>(buffer)
      .getSubsetChannelBlock(0, static_cast<size_t>(numChannels));

  auto context = juce::dsp::ProcessContextReplacing<float>(inoutBlock);
  processorChain.process(context);

  if (outChannels > 0) {
    const float lPeak = buffer.getMagnitude(0, 0, numSamples);
    leftPeak.store(lPeak);

    if (outChannels > 1) {
      const float rPeak = buffer.getMagnitude(1, 0, numSamples);
      rightPeak.store(rPeak);
    } else {
      rightPeak.store(lPeak);
    }
  }
}

bool PluginProcessor::hasEditor() const {
  return true;
}

juce::AudioProcessorEditor *PluginProcessor::createEditor() {
  return new PluginEditor(*this);
}

void PluginProcessor::getStateInformation(juce::MemoryBlock &destData) {
  juce::MemoryOutputStream outputStream{destData, true};
  JsonSerializer::serialize(parameters, outputStream);
}

void PluginProcessor::setStateInformation(const void *data, int sizeInBytes) {
  juce::MemoryInputStream inputStream{data, static_cast<size_t>(sizeInBytes), false};
  const auto result = JsonSerializer::deserialize(inputStream, parameters);

  if (result.failed()) {
    DBG(result.getErrorMessage());
  }

  gain.setGain(parameters.gain.get(), true);
  pan.setPan(parameters.pan.get(), true);

  tremolo.setModulationRate(parameters.tremoloRate.get());
  tremolo.setMix(parameters.tremoloMix.get(), true);
  tremolo.setDepth(parameters.tremoloDepth.get(), true);
  tremolo.setActive(parameters.tremoloActive.get());
  tremolo.setLfoWaveform(static_cast<Waveform>(parameters.tremoloWaveform.getIndex()));

  flanger.setMix(parameters.flangerMix.get(), true);
  flanger.setDepth(parameters.flangerDepth.get(), true);
  flanger.setFeedback(parameters.flangerFeedback.get(), true);
  flanger.setRate(parameters.flangerRate.get(), true);
  flanger.setActive(parameters.flangerActive.get());

  lowpass.setCutoff(parameters.lowpassFrequency.get(), true);
  lowpass.setResonance(parameters.lowpassResonance.get(), true);
  lowpass.setMix(parameters.lowpassMix.get(), true);
  lowpass.setActive(parameters.lowpassActive.get());

  highpass.setCutoff(parameters.highpassFrequency.get(), true);
  highpass.setResonance(parameters.highpassResonance.get(), true);
  highpass.setMix(parameters.highpassMix.get(), true);
  highpass.setActive(parameters.highpassActive.get());

  bandpass.setCutoff(parameters.bandpassFrequency.get(), true);
  bandpass.setResonance(parameters.bandpassResonance.get(), true);
  bandpass.setMix(parameters.bandpassMix.get(), true);
  bandpass.setActive(parameters.bandpassActive.get());
}

juce::AudioProcessorParameter *PluginProcessor::getBypassParameter() const {
  return &parameters.bypass;
}

void PluginProcessor::updateParameters() {
  gain.setGain(parameters.gain.get());
  pan.setPan(parameters.pan.get());

  tremolo.setModulationRate(parameters.tremoloRate.get());
  tremolo.setMix(parameters.tremoloMix.get());
  tremolo.setDepth(parameters.tremoloDepth.get()),
      tremolo.setLfoWaveform(static_cast<Waveform>(parameters.tremoloWaveform.getIndex()));
  tremolo.setActive(parameters.tremoloActive.get());

  flanger.setRate(parameters.flangerRate.get());
  flanger.setMix(parameters.flangerMix.get());
  flanger.setDepth(parameters.flangerDepth.get());
  flanger.setFeedback(parameters.flangerFeedback.get());
  flanger.setActive(parameters.flangerActive.get());

  lowpass.setCutoff(parameters.lowpassFrequency.get());
  lowpass.setResonance(parameters.lowpassResonance.get());
  lowpass.setMix(parameters.lowpassMix.get());
  lowpass.setActive(parameters.lowpassActive.get());

  highpass.setCutoff(parameters.highpassFrequency.get());
  highpass.setResonance(parameters.highpassResonance.get());
  highpass.setMix(parameters.highpassMix.get());
  highpass.setActive(parameters.highpassActive.get());

  bandpass.setCutoff(parameters.bandpassFrequency.get());
  bandpass.setResonance(parameters.bandpassResonance.get());
  bandpass.setMix(parameters.bandpassMix.get());
  bandpass.setActive(parameters.bandpassActive.get());
}
}  // namespace efx

juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
  return new efx::PluginProcessor();
}
