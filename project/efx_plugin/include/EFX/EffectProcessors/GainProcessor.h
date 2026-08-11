#pragma once

namespace efx {
class GainProcessor : public EffectProcessorBase {
public:
  void prepare(const juce::dsp::ProcessSpec &spec) noexcept override {
    const double sampleRate = spec.sampleRate;

    gainLinear.reset(sampleRate, rampLength);
    gainLinear.setCurrentAndTargetValue(1.0f);
  }

  void setGain(float gainDb, bool force = false) {
    const float linear = std::pow(10.0f, gainDb / 20.0f);
    if (force) {
      gainLinear.setCurrentAndTargetValue(linear);
    } else {
      gainLinear.setTargetValue(linear);
    }
  }

  void process(juce::dsp::ProcessContextReplacing<float> &context) noexcept override {
    const auto &inputBlock = context.getInputBlock();
    auto &outputBlock = context.getOutputBlock();

    const auto numChannels = outputBlock.getNumChannels();
    const auto numSamples = outputBlock.getNumSamples();

    for (size_t frameIndex = 0; frameIndex < numSamples; ++frameIndex) {
      const auto gainValue = gainLinear.getNextValue();

      for (size_t channelIndex = 0; channelIndex < numChannels; ++channelIndex) {
        const auto inputSample = inputBlock.getSample(channelIndex, frameIndex);

        const auto outputSample = inputSample * gainValue;

        outputBlock.setSample(channelIndex, frameIndex, outputSample);
      }
    }
  }

  void reset() noexcept override {}

private:
  juce::SmoothedValue<float> gainLinear{1.0f};
};
}  // namespace efx
