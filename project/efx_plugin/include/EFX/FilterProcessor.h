#pragma once

namespace efx {
enum class FilterType {
  LowPass,
  HighPass,
  BandPass
};

template <FilterType Type = FilterType::LowPass>
class FilterProcessor : public EffectProcessorBase {
public:
  explicit FilterProcessor(FilterType type = FilterType::LowPass)
      : filterType(type) {}

  void prepare(const juce::dsp::ProcessSpec &spec) noexcept override {
    sampleRate = spec.sampleRate;
    bypass = false;

    mixSmoothed.reset(sampleRate, rampLength);
    mixSmoothed.setCurrentAndTargetValue(1.0f);

    cutoffSmoothed.reset(sampleRate, rampLength);
    cutoffSmoothed.setCurrentAndTargetValue(1000.0f);

    resonanceSmoothed.reset(sampleRate, rampLength);
    resonanceSmoothed.setCurrentAndTargetValue(0.707f);

    states.resize(spec.numChannels, {0.f, 0.f, 0.f, 0.f});

    updateCoefficients(cutoffSmoothed.getCurrentValue(), resonanceSmoothed.getCurrentValue());
  }

  void setMix(float mix, bool force = false) {
    if (force) {
      mixSmoothed.setCurrentAndTargetValue(mix);
    } else {
      mixSmoothed.setTargetValue(mix);
    }
  }

  void setCutoff(float cutoffHz, bool force = false) {
    if (force) {
      cutoffSmoothed.setCurrentAndTargetValue(cutoffHz);
    } else {
      cutoffSmoothed.setTargetValue(cutoffHz);
    }
  }

  void setResonance(float q, bool force = false) {
    if (force) {
      resonanceSmoothed.setCurrentAndTargetValue(q);
    } else {
      resonanceSmoothed.setTargetValue(q);
    }
  }

  void setBypass(bool newBypass) {
    bypass = newBypass;
  }

  void process(juce::dsp::ProcessContextReplacing<float> &context) noexcept override {
    if (bypass) return;

    const auto &inputBlock = context.getInputBlock();
    auto &outputBlock = context.getOutputBlock();

    const auto numChannels = outputBlock.getNumChannels();
    const auto numSamples = outputBlock.getNumSamples();

    for (size_t frameIndex = 0; frameIndex < numSamples; ++frameIndex) {
      const float currentCutoff = cutoffSmoothed.getNextValue();
      const float currentQ = resonanceSmoothed.getNextValue();

      if (cutoffSmoothed.isSmoothing() || resonanceSmoothed.isSmoothing()) {
        updateCoefficients(currentCutoff, currentQ);
      }

      const auto mixValue = mixSmoothed.getNextValue();

      for (size_t channelIndex = 0; channelIndex < numChannels; ++channelIndex) {
        const auto inputSample = inputBlock.getSample(channelIndex, frameIndex);
        auto &state = states[channelIndex];

        const float filteredOutput = (b0 * inputSample) + (b1 * state.x1) + (b2 * state.x2)
                                     - (a1 * state.y1) - (a2 * state.y2);

        state.x2 = state.x1;
        state.x1 = inputSample;
        state.y2 = state.y1;
        state.y1 = filteredOutput;

        const auto dry = inputSample * (1.0f - mixValue);
        const auto wet = filteredOutput * mixValue;

        outputBlock.setSample(channelIndex, frameIndex, dry + wet);
      }
    }
  }

  void reset() noexcept override {
    for (auto &state: states) {
      state = {0.f, 0.f, 0.f, 0.f};
    }
  }

private:
  struct BiquadState {
    float x1{0.f}, x2{0.f}, y1{0.f}, y2{0.f};
  };

  void updateCoefficients(float cutoff, float q) {
    if (sampleRate <= 0.0) return;

    cutoff = std::clamp(cutoff, 20.0f, static_cast<float>(sampleRate * 0.49));
    q = std::max(0.1f, q);

    const float w0 = juce::MathConstants<float>::twoPi * cutoff / static_cast<float>(sampleRate);
    const float cosW0 = std::cos(w0);
    const float alpha = std::sin(w0) / (2.0f * q);

    const float a0 = 1.0f + alpha;

    if constexpr (Type == FilterType::LowPass) {
      b0 = ((1.0f - cosW0) / 2.0f) / a0;
      b1 = (1.0f - cosW0) / a0;
      b2 = ((1.0f - cosW0) / 2.0f) / a0;
    }
    else if constexpr (Type == FilterType::HighPass) {
      b0 = ((1.0f + cosW0) / 2.0f) / a0;
      b1 = -(1.0f + cosW0) / a0;
      b2 = ((1.0f + cosW0) / 2.0f) / a0;
    }
    else if constexpr (Type == FilterType::BandPass) {
      b0 = alpha / a0;
      b1 = 0.0f;
      b2 = -alpha / a0;
    }

    a1 = (-2.0f * cosW0) / a0;
    a2 = (1.0f - alpha) / a0;
  }

  FilterType filterType;
  bool bypass{false};
  double sampleRate{44100.0};

  juce::SmoothedValue<float> mixSmoothed{1.0f};
  juce::SmoothedValue<float> cutoffSmoothed{1000.0f};
  juce::SmoothedValue<float> resonanceSmoothed{0.707f};

  std::vector<BiquadState> states;
  float b0{0.f}, b1{0.f}, b2{0.f}, a1{0.f}, a2{0.f};
};
} // namespace efx