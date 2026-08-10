#pragma once

namespace efx {
class GainProcessor : public EffectProcessorBase {
public:
  void prepare(const juce::dsp::ProcessSpec& spec) noexcept override {
    const double sampleRate = spec.sampleRate;

    gainLinear.reset(sampleRate, rampLength);
    gainLinear.setCurrentAndTargetValue(1.f);
  }

  void setGain(float gainDb) {
    const float linear = std::pow(10.f, gainDb / 20.f);
    gainLinear.setTargetValue(linear);
  }

  void process(juce::dsp::ProcessContextReplacing<float>& context) noexcept override {
    const auto& block = context.getOutputBlock();

    for (const auto frameIndex : std::views::iota(static_cast<size_t>(0), block.getNumSamples())) {
      for (const auto channelIndex :
           std::views::iota(static_cast<size_t>(0), block.getNumChannels())) {
        const auto inputSample = block.getSample(channelIndex, frameIndex);

        const auto outputSample = inputSample * gainLinear.getNextValue();

        block.setSample(channelIndex, frameIndex, outputSample);
      }
    }
  }

  void reset() noexcept override {}

private:
  juce::SmoothedValue<float> gainLinear{1.f};
};
}  // namespace efx
