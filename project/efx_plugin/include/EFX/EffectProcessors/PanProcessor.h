#pragma once

namespace efx {
class PanProcessor : public EffectProcessorBase {
public:
  void prepare(const juce::dsp::ProcessSpec &spec) noexcept override {
    const double sampleRate = spec.sampleRate;

    panValue.reset(sampleRate, rampLength);
    panValue.setCurrentAndTargetValue(0.f);
  }

  void setPan(float newPan) {
    panValue.setTargetValue((juce::jlimit(-1.f, 1.f, newPan)));
  }

  void process(juce::dsp::ProcessContextReplacing<float> &context) noexcept override {
    const auto &block = context.getOutputBlock();

    const int numChannels = block.getNumChannels();
    if (numChannels < 2) {
      return;
    }

    for (const auto frameIndex: std::views::iota(static_cast<size_t>(0), block.getNumSamples())) {
      const auto leftIn = block.getSample(0, frameIndex);
      const auto rightIn = block.getSample(1, frameIndex);
      const auto pan = panValue.getNextValue();

      const auto mappedPan = (pan + 1.f) * juce::MathConstants<float>::pi * 0.25;
      const auto boost = std::sqrt(2.f);

      const auto leftGain = std::cos(mappedPan);
      const auto rightGain = std::sin(mappedPan);

      const auto leftOut = leftIn * leftGain * boost;
      const auto rightOut = rightIn * rightGain * boost;

      block.setSample(0, frameIndex, leftOut);
      block.setSample(1, frameIndex, rightOut);
    }
  }

  void reset() noexcept override {}

private:
  juce::SmoothedValue<float> panValue = 0.f;
};
}  // namespace efx
