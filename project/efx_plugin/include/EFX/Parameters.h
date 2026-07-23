#pragma once

namespace efx {
struct Parameters {
  explicit Parameters(juce::AudioProcessor&);

  juce::AudioParameterFloat& gain;
  juce::AudioParameterFloat& pan;
  juce::AudioParameterBool& bypass;

  JUCE_DECLARE_NON_COPYABLE(Parameters);
  JUCE_DECLARE_NON_MOVEABLE(Parameters);
};
}  // namespace efx
