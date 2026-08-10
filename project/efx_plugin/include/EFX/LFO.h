#pragma once

namespace efx {
enum class Waveform : std::uint8_t {
  sine = 0,
  triangle = 1
};

class LFO {
public:
  LFO() = default;

  void prepare(double sampleRate) {
    jassert (sampleRate > 0.0);

    currentSampleRate = sampleRate;
    updatePhaseDelta();
    reset();
  }

  void setFrequency(float newFrequency) {
    if (frequency == newFrequency) return;
    frequency = newFrequency;
    updatePhaseDelta();
  }

  void setWaveform(Waveform newWaveform) {
    waveform = newWaveform;
  }

  void reset() {
    phase = 0.0f;
  }

  float getNextSample() {
    float out = 0.0f;

    switch (waveform) {
      case Waveform::sine:
        out = std::sin(phase * juce::MathConstants<float>::twoPi);
        break;

      case Waveform::triangle:
        out = 2.0f * std::abs(2.0f * phase - 1.0f) - 1.0f;
        break;
    }

    advancePhase();
    return out;
  }

private:
  void advancePhase() {
    phase += phaseDelta;

    if (phase >= 1.0f)
      phase -= 1.0f;
  }

  void updatePhaseDelta() {
    if (currentSampleRate > 0.0)
      phaseDelta = frequency / static_cast<float> (currentSampleRate);
  }

  Waveform waveform{Waveform::sine};
  float frequency{1.0f};
  double currentSampleRate{44100.0};

  float phase{0.0f};
  float phaseDelta{0.0f};
};
}