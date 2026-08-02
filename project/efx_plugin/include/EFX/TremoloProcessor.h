#pragma once

namespace efx {
class TremoloProcessor : public EffectProcessorBase {
public:
  enum class LfoWaveform : std::uint8_t {
    sine = 0,
    triangle = 1,
  };

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

  void setLfoWaveform(LfoWaveform waveform) {
    jassert(waveform == LfoWaveform::sine || waveform == LfoWaveform::triangle);

    if (waveform == LfoWaveform::sine)
      lfo.setWaveform(TremoloLFO::Waveform::sine);
    else if (waveform == LfoWaveform::triangle)
      lfo.setWaveform(TremoloLFO::Waveform::triangle);
  }

  void setModulationRate(float rateHz) {
    lfo.setFrequency(rateHz);
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

    const auto& block = context.getOutputBlock();

    for (const auto frameIndex : std::views::iota(static_cast<size_t>(0), block.getNumSamples())) {
      const auto lfoValue = lfo.getNextSample();

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
    lfo.reset();
  }
private:
  bool bypass{false};

  juce::SmoothedValue<float> depthSmoothed{0.4f};
  juce::SmoothedValue<float> mixSmoothed{1.0f};

  TremoloLFO lfo;
};
}