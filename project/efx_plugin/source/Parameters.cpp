
namespace efx {
namespace {
auto& addParameterToProcessor(juce::AudioProcessor& processor, auto parameter) {
  auto &parameterReference = *parameter;
  processor.addParameter(parameter.release());
  return parameterReference;
}

constexpr auto defaultVersionHint = 1;

juce::AudioParameterFloat &createGainParameter(juce::AudioProcessor &processor) {
  auto parameter = std::make_unique<juce::AudioParameterFloat>(
      juce::ParameterID{"gain", defaultVersionHint},
      juce::String{"Gain"},
      juce::NormalisableRange{-24.f, 24.f, 0.1f},
      0.f,
      juce::AudioParameterFloatAttributes{}.withLabel("dB")
        .withStringFromValueFunction([](float x, int) { return juce::String(x,1); })
      );
  return addParameterToProcessor(processor, std::move(parameter));
}

juce::AudioParameterFloat &createPanParameter(juce::AudioProcessor &processor) {
  auto parameter = std::make_unique<juce::AudioParameterFloat>(
      juce::ParameterID{"pan", defaultVersionHint},
      juce::String{"Pan"},
      juce::NormalisableRange{-1.f, 1.f, 0.01f},
      0.f
  );
  return addParameterToProcessor(processor, std::move(parameter));
}

juce::AudioParameterBool &createBypassParameter(juce::AudioProcessor &processor) {
  auto parameter = std::make_unique<juce::AudioParameterBool>(
      juce::ParameterID("bypass", defaultVersionHint),
      juce::String("Bypass"),
      false
      );
  return addParameterToProcessor(processor, std::move(parameter));
}

juce::AudioParameterFloat &createTremoloRateParameter(juce::AudioProcessor &processor) {
  auto parameter = std::make_unique<juce::AudioParameterFloat>(
      juce::ParameterID{"tremolo.modulation.rate", defaultVersionHint},
      juce::String{"Tremolo modulation rate"},
      juce::NormalisableRange{0.1f, 20.f, 0.01f, 0.4f},
      5.f,
      juce::AudioParameterFloatAttributes{}.withLabel("Hz")
  );
  return addParameterToProcessor(processor, std::move(parameter));
}

juce::AudioParameterFloat &createTremoloMixParameter(juce::AudioProcessor &processor) {
  auto parameter = std::make_unique<juce::AudioParameterFloat>(
      juce::ParameterID{"tremolo.mix", defaultVersionHint},
      juce::String{"Tremolo mix"},
      juce::NormalisableRange{0.f, 1.f, 0.001f},
      1.f
  );
  return addParameterToProcessor(processor, std::move(parameter));
}

juce::AudioParameterFloat &createTremoloDepthParameter(juce::AudioProcessor &processor) {
  auto parameter = std::make_unique<juce::AudioParameterFloat>(
      juce::ParameterID{"tremolo.depth", defaultVersionHint},
      juce::String{"Tremolo depth"},
      juce::NormalisableRange{0.f, 1.f, 0.001f},
      0.4f,
      juce::AudioParameterFloatAttributes{}
          .withStringFromValueFunction([](float x, int) { return juce::String(x, 3); })
  );
  return addParameterToProcessor(processor, std::move(parameter));
}

juce::AudioParameterBool &createTremoloBypassParameter(juce::AudioProcessor &processor) {
  auto parameter = std::make_unique<juce::AudioParameterBool>(
      juce::ParameterID{"tremolo.bypass", defaultVersionHint},
      juce::String{"Tremolo bypass"},
      false
  );
  return addParameterToProcessor(processor, std::move(parameter));
}

juce::AudioParameterChoice &createTremoloWaveformParameter(juce::AudioProcessor &processor) {
  auto parameter = std::make_unique<juce::AudioParameterChoice>(
      juce::ParameterID{"tremolo.waveform", defaultVersionHint},
      juce::String{"Tremolo waveform"},
      juce::StringArray{"Sine", "Triangle"},
      0
  );
  return addParameterToProcessor(processor, std::move(parameter));
}

}  // namespace

Parameters::Parameters(juce::AudioProcessor& processor)
  : gain(createGainParameter(processor)),
    pan(createPanParameter(processor)),
    bypass(createBypassParameter(processor)),
    tremoloRate(createTremoloRateParameter(processor)),
    tremoloMix(createTremoloMixParameter(processor)),
    tremoloDepth(createTremoloDepthParameter(processor)),
    tremoloBypass(createTremoloBypassParameter(processor)),
    tremoloWaveform(createTremoloWaveformParameter(processor)) {
}
}  // namespace efx
