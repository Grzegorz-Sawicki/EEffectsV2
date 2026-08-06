#pragma once

namespace efx {
class PluginProcessor : public juce::AudioProcessor {
public:
  PluginProcessor();

  void prepareToPlay(double sampleRate, int expectedMaxFramesPerBlock) override;

  void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
  using AudioProcessor::processBlock;

  void releaseResources() override;

  juce::AudioProcessorEditor* createEditor() override;
  bool hasEditor() const override;

  const juce::String getName() const override;

  bool isBusesLayoutSupported(const BusesLayout& layouts) const override;

  bool acceptsMidi() const override;
  bool producesMidi() const override;
  bool isMidiEffect() const override;
  double getTailLengthSeconds() const override;

  int getNumPrograms() override;
  int getCurrentProgram() override;
  void setCurrentProgram(int index) override;
  const juce::String getProgramName(int index) override;
  void changeProgramName(int index, const juce::String& newName) override;

  void getStateInformation(juce::MemoryBlock& destData) override;
  void setStateInformation(const void* data, int sizeInBytes) override;

  juce::AudioProcessorParameter* getBypassParameter() const override;

  Parameters& getParameterRefs() noexcept {
    return parameters;
  }

  std::atomic<float> leftPeak { 0.0f };
  std::atomic<float> rightPeak { 0.0f };

private:
  Parameters parameters{*this};

  juce::dsp::ProcessorChain<
    GainProcessor,
    PanProcessor,
    TremoloProcessor,
    FlangerProcessor,
    FilterProcessor<FilterType::LowPass>,
    FilterProcessor<FilterType::HighPass>,
    FilterProcessor<FilterType::BandPass>> processorChain;

  enum ProcessorIndices
  {
    gainIndex,
    panIndex,
    tremoloIndex,
    flangerIndex,
    lowpassIndex,
    highpassIndex,
    bandpassIndex
  };

  GainProcessor& gain;
  PanProcessor& pan;
  TremoloProcessor& tremolo;
  FlangerProcessor& flanger;
  FilterProcessor<FilterType::LowPass>& lowpass;
  FilterProcessor<FilterType::HighPass>& highpass;
  FilterProcessor<FilterType::BandPass>& bandpass;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginProcessor)
};
}  // namespace efx
