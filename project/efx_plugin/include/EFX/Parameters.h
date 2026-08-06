#pragma once

namespace efx {
struct Parameters {
  explicit Parameters(juce::AudioProcessor&);

  juce::AudioParameterFloat& gain;
  juce::AudioParameterFloat& pan;
  juce::AudioParameterBool& bypass;

  juce::AudioParameterFloat& tremoloRate;
  juce::AudioParameterFloat& tremoloMix;
  juce::AudioParameterFloat& tremoloDepth;
  juce::AudioParameterBool& tremoloBypass;
  juce::AudioParameterChoice& tremoloWaveform;

  juce::AudioParameterFloat& flangerRate;
  juce::AudioParameterFloat& flangerMix;
  juce::AudioParameterFloat& flangerDepth;
  juce::AudioParameterFloat& flangerFeedback;
  juce::AudioParameterBool& flangerBypass;

  juce::AudioParameterFloat& lowpassFrequency;
  juce::AudioParameterFloat& lowpassResonance;
  juce::AudioParameterFloat& lowpassMix;
  juce::AudioParameterBool& lowpassBypass;

  juce::AudioParameterFloat& highpassFrequency;
  juce::AudioParameterFloat& highpassResonance;
  juce::AudioParameterFloat& highpassMix;
  juce::AudioParameterBool& highpassBypass;

  juce::AudioParameterFloat& bandpassFrequency;
  juce::AudioParameterFloat& bandpassResonance;
  juce::AudioParameterFloat& bandpassMix;
  juce::AudioParameterBool& bandpassBypass;

  JUCE_DECLARE_NON_COPYABLE(Parameters);
  JUCE_DECLARE_NON_MOVEABLE(Parameters);
};
}  // namespace efx
