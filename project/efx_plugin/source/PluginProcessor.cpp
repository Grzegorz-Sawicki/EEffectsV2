namespace efx {
PluginProcessor::PluginProcessor()
    : gain (juce::dsp::get<gainIndex>(processorChain)),
      pan (juce::dsp::get<panIndex>(processorChain)),
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
  tremolo.prepare(sampleRate, expectedMaxFramesPerBlock);

  const auto numChannels = juce::jmax (getTotalNumInputChannels(), getTotalNumOutputChannels());

  auto spec = juce::dsp::ProcessSpec(
      sampleRate,
      expectedMaxFramesPerBlock,
      numChannels
  );

  processorChain.prepare(spec);

  //gain.prepare(spec);
}

void PluginProcessor::releaseResources() {
  // When playback stops, you can use this as an opportunity to free up any
  // spare memory, etc.

  tremolo.reset();

  processorChain.reset();
  //gain.reset();
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
  const auto totalNumInputChannels = getTotalNumInputChannels();
  const auto totalNumOutputChannels = getTotalNumOutputChannels();

  // In case we have more outputs than inputs, this code clears any output
  // channels that didn't contain input data, (because these aren't
  // guaranteed to be empty - they may contain garbage).
  // This is here to avoid people getting screaming feedback
  // when they first compile a plugin, but obviously you don't need to keep
  // this code if your algorithm always overwrites all the output channels.
  for (const auto channelToClear :
       std::views::iota(totalNumInputChannels, totalNumOutputChannels)) {
    buffer.clear(channelToClear, 0, buffer.getNumSamples());
  }

  gain.setGain(parameters.gain.get());
  pan.setPan(parameters.pan.get());

  if(parameters.bypass.get()) {
    return;
  }

  //tremolo.process(buffer);

  const auto numChannels = juce::jmax (totalNumInputChannels, totalNumOutputChannels);
  auto inoutBlock = juce::dsp::AudioBlock<float> (buffer)
      .getSubsetChannelBlock(0, static_cast<size_t>(numChannels));

  auto context = juce::dsp::ProcessContextReplacing<float> (inoutBlock);
  processorChain.process(context);
}

bool PluginProcessor::hasEditor() const {
  return true;
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

  gain.setGain(parameters.gain.get());
  pan.setPan(parameters.pan.get());
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
