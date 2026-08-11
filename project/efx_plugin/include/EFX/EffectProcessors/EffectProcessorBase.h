#pragma once

namespace efx {
class EffectProcessorBase {
public:
  EffectProcessorBase() = default;

  virtual ~EffectProcessorBase() = default;

  virtual void prepare(const juce::dsp::ProcessSpec &spec) noexcept = 0;

  virtual void process(juce::dsp::ProcessContextReplacing<float> &context) noexcept = 0;

  virtual void reset() noexcept = 0;

protected:
  const float rampLength = 0.025f;
};
}  // namespace efx
