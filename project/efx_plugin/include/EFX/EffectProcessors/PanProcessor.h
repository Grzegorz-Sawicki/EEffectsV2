#pragma once

namespace efx {
class PanProcessor : public EffectProcessorBase {
public:
  void prepare(const juce::dsp::ProcessSpec &spec) noexcept override {
    const double sampleRate = spec.sampleRate;

    panValue.reset(sampleRate, rampLength);
    panValue.setCurrentAndTargetValue(0.0f);
  }

  void setPan(float newPan, bool force = false) {
    if (force) {
      panValue.setCurrentAndTargetValue(newPan);
    } else {
      panValue.setTargetValue(newPan);
    }
  }

  void process(juce::dsp::ProcessContextReplacing<float> &context) noexcept override {
    const auto &inputBlock = context.getInputBlock();
    auto &outputBlock = context.getOutputBlock();

    const auto numChannels = outputBlock.getNumChannels();
    const auto numSamples = outputBlock.getNumSamples();

    if (numChannels < 2) {
      return;
    }

    for (size_t frameIndex = 0; frameIndex < numSamples; ++frameIndex) {
      const auto leftIn = inputBlock.getSample(0, frameIndex);
      const auto rightIn = inputBlock.getSample(1, frameIndex);
      const auto pan = panValue.getNextValue();

      const auto mappedPan = (pan + 1.0f) * juce::MathConstants<float>::pi * 0.25f;
      const auto boost = std::sqrt(2.0f);

      const auto leftGain = std::cos(mappedPan);
      const auto rightGain = std::sin(mappedPan);

      const auto leftOut = leftIn * leftGain * boost;
      const auto rightOut = rightIn * rightGain * boost;

      outputBlock.setSample(0, frameIndex, leftOut);
      outputBlock.setSample(1, frameIndex, rightOut);
    }
  }

  void reset() noexcept override {}

private:
  juce::SmoothedValue<float> panValue = 0.0f;
};
}  // namespace efx
