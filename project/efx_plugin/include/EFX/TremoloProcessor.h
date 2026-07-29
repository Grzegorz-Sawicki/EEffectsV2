#pragma once

namespace efx {
class TremoloProcessor : public EffectProcessorBase {
public:
  enum class LfoWaveform : std::uint8_t {
    sine = 0,
    triangle = 1,
  };

  TremoloProcessor() {

  }

  void prepare(const juce::dsp::ProcessSpec& spec) noexcept override {
    const auto sampleRate = spec.sampleRate;
    const auto maxBlockSize = spec.maximumBlockSize;

    bypass = false;

    lfoSmoothed.reset(sampleRate, rampLength);
    lfoSmoothed.setCurrentAndTargetValue(0.0f);

    mixSmoothed.reset(sampleRate, rampLength);
    mixSmoothed.setCurrentAndTargetValue(1.0f);

    depthSmoothed.reset(sampleRate, rampLength);
    depthSmoothed.setCurrentAndTargetValue(0.4f);

    const juce::dsp::ProcessSpec lfoSpec{
      .sampleRate = sampleRate,
      .maximumBlockSize = maxBlockSize,
      .numChannels = 1u
    };

    for(auto& lfo : lfos) {
      lfo.prepare(lfoSpec);
    }
  }

  void setLfoWaveform(LfoWaveform waveform, bool force = false) {
    jassert(waveform == LfoWaveform::sine || waveform == LfoWaveform::triangle);
    lfoToSet = waveform;
    if(force) currentLfo = lfoToSet;
  }

  void setModulationRate(float rateHz) {
    for (auto& lfo : lfos) {
      lfo.setFrequency(rateHz, true);
    }
  }


  void setMix(float mix, bool force = false) {
    if(force) {
      mixSmoothed.setCurrentAndTargetValue(mix);
    } else {
      mixSmoothed.setTargetValue(mix);
    }
  }

  void setDepth(float depth, bool force = false) {
    if(force) {
      depthSmoothed.setCurrentAndTargetValue(depth);
    } else {
      depthSmoothed.setTargetValue(depth);
    }
  }

  //TODO: use built-in processor chain bypass?
  void setBypass(bool newBypass) {
    bypass = newBypass;
  }

  void process(juce::dsp::ProcessContextReplacing<float>& context) noexcept override {
    if(bypass) return;

    updateLfoWaveform();

    const auto& block = context.getOutputBlock();

    for (const auto frameIndex : std::views::iota(static_cast<size_t>(0), block.getNumSamples())) {
      const auto lfoValue = getNextLfoValue();
      const auto modulationValue = (depthSmoothed.getNextValue() * lfoValue) + 1;
      const auto mixValue = mixSmoothed.getNextValue();

      for (const auto channelIndex :
          std::views::iota(static_cast<size_t>(0), block.getNumChannels())) {
        const auto inputSample = block.getSample(channelIndex, frameIndex);

        const auto dry = inputSample * (1 - mixValue);
        const auto unmixedOutput = inputSample * modulationValue;
        const auto wet = unmixedOutput * mixValue;

        const auto mixedOutput = dry + wet;

        block.setSample(channelIndex, frameIndex, mixedOutput);
      }
    }
  }

  void reset() noexcept override {
    for (auto& lfo : lfos) {
      lfo.reset();
    }
  }
private:
  float getNextLfoValue() {
    if(lfoSmoothed.isSmoothing()) {
      return lfoSmoothed.getNextValue();
    }
    return lfos[juce::toUnderlyingType(currentLfo)].processSample(0.f);
  }

  void updateLfoWaveform() {
    if(currentLfo != lfoToSet) {
      lfoSmoothed.setCurrentAndTargetValue(getNextLfoValue());
      currentLfo = lfoToSet;
      lfoSmoothed.setTargetValue(getNextLfoValue());
    }
  }

  static float triangle(float phase) {
    const auto ft = phase / (2 * juce::MathConstants<float>::pi);
    return 4.0f * std::abs(ft - std::floor(ft + 0.5f)) - 1.0f;
  }

  bool bypass{false};
  juce::SmoothedValue<float> depthSmoothed{0.4f};
  juce::SmoothedValue<float> mixSmoothed{1.0f};
  juce::SmoothedValue<float> lfoSmoothed{0.0f};
  LfoWaveform currentLfo{LfoWaveform::sine};
  LfoWaveform lfoToSet{currentLfo};

  std::array<juce::dsp::Oscillator<float>, 2u> lfos{
    juce::dsp::Oscillator<float>{[](auto phase) { return std::sin(phase); }},
    juce::dsp::Oscillator<float>{triangle},
  };

};
}