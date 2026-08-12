#pragma once

namespace efx {
class ComplexEffectProcessorBase : public EffectProcessorBase {
public:
  ComplexEffectProcessorBase() = default;

  virtual ~ComplexEffectProcessorBase() = default;

  void setMix(float mix, bool force = false) {
    if (force) {
      mixSmoothed.setCurrentAndTargetValue(mix);
    } else {
      mixSmoothed.setTargetValue(mix);
    }
  }

  void setActive(bool active) {
    bypass = !active;
  }

protected:
  bool bypass{false};
  juce::SmoothedValue<float> mixSmoothed{1.0f};
};
}  // namespace efx
