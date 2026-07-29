namespace efx {
PluginProcessor::PluginProcessor()
    : gain (juce::dsp::get<gainIndex>(processorChain)),
      pan (juce::dsp::get<panIndex>(processorChain)),
      tremolo (juce::dsp::get<tremoloIndex>(processorChain)),
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
  // Some hosts don't cope very well if you tell them there are 0 programs, so
  // this should be at least 1, even if you're not really implementing programs.
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
                                        const juce::String& newName) {
  juce::ignoreUnused(index, newName);
}

void PluginProcessor::prepareToPlay(double sampleRate,
                                    int expectedMaxFramesPerBlock) {
  const auto numChannels = juce::jmax (getTotalNumInputChannels(), getTotalNumOutputChannels());

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

bool PluginProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const {
  // This is the place where you check if the layout is supported.
  // In this template code we only support mono or stereo.
  // Some plugin hosts, such as certain GarageBand versions, will only
  // load plugins that support stereo bus layouts.
  if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() &&
      layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo()) {
    return false;
  }

  // This checks if the input layout matches the output layout
  if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet()) {
    return false;
  }

  return true;
}

void PluginProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                   juce::MidiBuffer& midiMessages) {
  juce::ignoreUnused(midiMessages);

  juce::ScopedNoDenormals noDenormals;
  const auto inChannels = getTotalNumInputChannels();
  const auto outChannels = getTotalNumOutputChannels();
  const auto numSamples = buffer.getNumSamples();

  // In case we have more outputs than inputs, this code clears any output
  // channels that didn't contain input data, (because these aren't
  // guaranteed to be empty - they may contain garbage).
  // This is here to avoid people getting screaming feedback
  // when they first compile a plugin, but obviously you don't need to keep
  // this code if your algorithm always overwrites all the output channels.
  for (const auto channelToClear :
       std::views::iota(inChannels, outChannels)) {
    buffer.clear(channelToClear, 0, buffer.getNumSamples());
  }

  gain.setGain(parameters.gain.get());
  pan.setPan(parameters.pan.get());

  tremolo.setModulationRate(parameters.tremoloRate.get());
  tremolo.setMix(parameters.tremoloMix.get());
  tremolo.setDepth(parameters.tremoloDepth.get()),
  tremolo.setLfoWaveform(static_cast<TremoloProcessor::LfoWaveform>(parameters.tremoloWaveform.getIndex()));
  tremolo.setBypass(parameters.tremoloBypass.get());

  if(parameters.bypass.get()) {
    return;
  }

  const auto numChannels = juce::jmax (inChannels, outChannels);
  auto inoutBlock = juce::dsp::AudioBlock<float> (buffer)
      .getSubsetChannelBlock(0, static_cast<size_t>(numChannels));

  auto context = juce::dsp::ProcessContextReplacing<float> (inoutBlock);
  processorChain.process(context);

  if (outChannels > 0)
  {
    const float lPeak = buffer.getMagnitude (0, 0, numSamples);
    leftPeak.store (lPeak);

    if (outChannels > 1)
    {
      const float rPeak = buffer.getMagnitude (1, 0, numSamples);
      rightPeak.store (rPeak);
    }
    else
    {
      rightPeak.store (lPeak);
    }
  }
}

bool PluginProcessor::hasEditor() const {
  return false;
}

// This function will be called to create an instance of the editor
juce::AudioProcessorEditor* PluginProcessor::createEditor() {
  //return nullptr;
  return new PluginEditor(*this);
}

void PluginProcessor::getStateInformation(juce::MemoryBlock& destData) {
  juce::MemoryOutputStream outputStream{destData, true};
  JsonSerializer::serialize(parameters, outputStream);
}

void PluginProcessor::setStateInformation(const void* data, int sizeInBytes) {
  juce::MemoryInputStream inputStream{data, static_cast<size_t>(sizeInBytes), false};
  const auto result = JsonSerializer::deserialize(inputStream, parameters);

  if(result.failed()) {
    DBG(result.getErrorMessage());
  }

  gain.setGain(parameters.gain.get()); //TODO add force
  pan.setPan(parameters.pan.get()); //TODO add force

  tremolo.setModulationRate(parameters.tremoloRate.get());
  tremolo.setMix(parameters.tremoloMix.get(), true);
  tremolo.setDepth(parameters.tremoloDepth.get(), true);
  tremolo.setBypass(parameters.tremoloBypass.get());
  tremolo.setLfoWaveform(static_cast<TremoloProcessor::LfoWaveform>(parameters.tremoloWaveform.getIndex()), true);
}

juce::AudioProcessorParameter* PluginProcessor::getBypassParameter() const {
  return &parameters.bypass;
}

}  // namespace efx

// This creates new instances of the plugin.
// This function definition must be in the global namespace.
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
  return new efx::PluginProcessor();
}
