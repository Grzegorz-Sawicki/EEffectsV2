#pragma once

namespace efx {
class DelayLine {
public:
  DelayLine() = default;

  void prepare(double sampleRate, double maxDelayMs) {
    currentSampleRate = sampleRate;

    bufferSize = static_cast<int>(std::ceil(sampleRate * (maxDelayMs / 1000.0))) + 10;
    buffer.assign(static_cast<size_t>(bufferSize), 0.0f);
    writeIndex = 0;
  }

  void reset() {
    std::fill(buffer.begin(), buffer.end(), 0.0f);
    writeIndex = 0;
  }

  void pushSample(float sample) {
    buffer.at(writeIndex) = sample;
    writeIndex = (writeIndex + 1) % bufferSize;
  }

  float popSample(float delayMs) {
    const float delaySamples = (delayMs / 1000.0f) * static_cast<float>(currentSampleRate);
    float readPosition = static_cast<float>(writeIndex) - delaySamples;

    while (readPosition < 0.0f) {
      readPosition += static_cast<float>(bufferSize);
    }

    const int indexA = static_cast<int>(readPosition);
    const int indexB = (indexA + 1) % bufferSize;
    const float fraction = readPosition - static_cast<float>(indexA);

    const float sampleA = buffer.at(indexA);
    const float sampleB = buffer.at(indexB);

    return sampleA + fraction * (sampleB - sampleA);
  }

private:
  std::vector<float> buffer;
  int bufferSize{0};
  int writeIndex{0};
  double currentSampleRate{44100.0};
};

class FlangerProcessor : public EffectProcessorBase {
public:
  FlangerProcessor() {
    lfo.setWaveform(TremoloLFO::Waveform::sine);
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
      depthSmoothed.setCurrentAndTargetValue(std::clamp(depthNormalized, 0.0f, 1.0f));
    }
    depthSmoothed.setTargetValue(std::clamp(depthNormalized, 0.0f, 1.0f));
  }

  void setMix(float mixNormalized, bool force = false) {
    if (force) {
      mixSmoothed.setCurrentAndTargetValue(std::clamp(mixNormalized, 0.0f, 1.0f));
    }
    mixSmoothed.setTargetValue(std::clamp(mixNormalized, 0.0f, 1.0f));
  }

  void setFeedback(float feedbackNormalized, bool force = false) {
    if (force) {
      feedbackSmoothed.setCurrentAndTargetValue(std::clamp(feedbackNormalized, -0.95f, 0.95f));
    }
    feedbackSmoothed.setTargetValue(std::clamp(feedbackNormalized, -0.95f, 0.95f));
  }

private:
  std::vector<DelayLine> delayLines;

  TremoloLFO lfo;

  juce::SmoothedValue<float> mixSmoothed{0.5f};
  juce::SmoothedValue<float> rateSmoothed{0.5f};
  juce::SmoothedValue<float> depthSmoothed{0.5f};
  juce::SmoothedValue<float> feedbackSmoothed{0.3f};
};
} // namespace efx