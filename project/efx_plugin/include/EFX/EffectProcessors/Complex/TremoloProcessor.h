#pragma once

namespace efx {
class TremoloProcessor : public ComplexEffectProcessorBase {
public:
  TremoloProcessor() = default;

  void prepare(const juce::dsp::ProcessSpec& spec) noexcept override {
    const auto sampleRate = spec.sampleRate;

    bypass = false;

    mixSmoothed.reset(sampleRate, rampLength);
    mixSmoothed.setCurrentAndTargetValue(1.0f);

    depthSmoothed.reset(sampleRate, rampLength);
    depthSmoothed.setCurrentAndTargetValue(0.4f);

    lfo.prepare(sampleRate);
  }

  void setLfoWaveform(Waveform waveform) {
    jassert(waveform == Waveform::sine || waveform == Waveform::triangle);
    lfo.setWaveform(waveform);
  }

  void setModulationRate(float rateHz) {
    lfo.setFrequency(rateHz);
  }

  void setDepth(float depth, bool force = false) {
    if(force) {
      depthSmoothed.setCurrentAndTargetValue(depth);
    } else {
      depthSmoothed.setTargetValue(depth);
    }
  }

  void process(juce::dsp::ProcessContextReplacing<float>& context) noexcept override {
    if(bypass) {
      return;
    }

    const auto &inputBlock = context.getInputBlock();
    auto &outputBlock = context.getOutputBlock();

    const auto numChannels = outputBlock.getNumChannels();
    const auto numSamples = outputBlock.getNumSamples();

    for (size_t frameIndex = 0; frameIndex < numSamples; ++frameIndex) {
      const auto lfoValue = lfo.getNextSample();

      const auto modulationValue = (depthSmoothed.getNextValue() * lfoValue) + 1;
      const auto mixValue = mixSmoothed.getNextValue();

      for (size_t channelIndex = 0; channelIndex < numChannels; ++channelIndex) {
        const auto inputSample = inputBlock.getSample(channelIndex, frameIndex);

        const auto dry = inputSample * (1 - mixValue);
        const auto unmixedOutput = inputSample * modulationValue;
        const auto wet = unmixedOutput * mixValue;

        const auto mixedOutput = dry + wet;

        outputBlock.setSample(channelIndex, frameIndex, mixedOutput);
      }
    }
  }

  void reset() noexcept override {
    lfo.reset();
  }
private:
  juce::SmoothedValue<float> depthSmoothed{0.4f};

  LFO lfo;
};
}