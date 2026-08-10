#pragma once

namespace efx {
class FlangerProcessor : public ComplexEffectProcessorBase {
public:
  FlangerProcessor() {
    lfo.setWaveform(Waveform::sine);
  }

  void prepare(const juce::dsp::ProcessSpec &spec) noexcept override {
    const auto sampleRate = spec.sampleRate;

    mixSmoothed.reset(sampleRate, rampLength);
    mixSmoothed.setCurrentAndTargetValue(0.5f);

    rateSmoothed.reset(sampleRate, rampLength);
    rateSmoothed.setCurrentAndTargetValue(0.5f);

    depthSmoothed.reset(sampleRate, rampLength);
    depthSmoothed.setCurrentAndTargetValue(0.5f);

    feedbackSmoothed.reset(sampleRate, rampLength);
    feedbackSmoothed.setCurrentAndTargetValue(0.3f);

    lfo.prepare(sampleRate);

    delayLines.resize(spec.numChannels);
    for (auto &delayLine: delayLines) {
      delayLine.prepare(sampleRate, 10.0);
    }
  }

  void process(juce::dsp::ProcessContextReplacing<float> &context) noexcept override {
    if(bypass) {
      return;
    }

    const auto &inputBlock = context.getInputBlock();
    auto &outputBlock = context.getOutputBlock();

    const auto numChannels = outputBlock.getNumChannels();
    const auto numSamples = outputBlock.getNumSamples();

    for (size_t sampleIndex = 0; sampleIndex < numSamples; ++sampleIndex) {
      lfo.setFrequency(rateSmoothed.getNextValue());
      const float lfoValue = lfo.getNextSample();

      const float currentMix = mixSmoothed.getNextValue();
      const float currentDepth = depthSmoothed.getNextValue();
      const float currentFeedback = feedbackSmoothed.getNextValue();

      const float maxSweepMs = 2.4f * currentDepth;
      const float centerDelayMs = 2.5f;
      const float currentDelayMs = centerDelayMs + (maxSweepMs * lfoValue);

      for (size_t channel = 0; channel < numChannels; ++channel) {
        const float inputSample = inputBlock.getSample(channel, sampleIndex);

        const float delayedSample = delayLines[channel].popSample(currentDelayMs);

        const float outputSample = (inputSample * (1.0f - currentMix)) + (delayedSample * currentMix);
        outputBlock.setSample(channel, sampleIndex, outputSample);

        float feedbackSample = inputSample + (delayedSample * currentFeedback);
        feedbackSample = std::clamp(feedbackSample, -1.0f, 1.0f);

        delayLines[channel].pushSample(feedbackSample);
      }
    }
  }

  void reset() noexcept override {
    lfo.reset();
    for (auto &delayLine: delayLines) {
      delayLine.reset();
    }
  }

  void setRate(float rateHz, bool force = false) {
    if (force) {
      rateSmoothed.setCurrentAndTargetValue(rateHz);
    }
    rateSmoothed.setTargetValue(rateHz);
  }

  void setDepth(float depthNormalized, bool force = false) {
    if (force) {
      depthSmoothed.setCurrentAndTargetValue(depthNormalized);
    }
    depthSmoothed.setTargetValue(depthNormalized);
  }

  void setFeedback(float feedbackNormalized, bool force = false) {
    if (force) {
      feedbackSmoothed.setCurrentAndTargetValue(feedbackNormalized);
    }
    feedbackSmoothed.setTargetValue(feedbackNormalized);
  }

private:
  std::vector<DelayLine> delayLines;

  LFO lfo;

  juce::SmoothedValue<float> rateSmoothed{0.5f};
  juce::SmoothedValue<float> depthSmoothed{0.5f};
  juce::SmoothedValue<float> feedbackSmoothed{0.3f};
};
} // namespace efx