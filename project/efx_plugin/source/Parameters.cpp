
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
}  // namespace

Parameters::Parameters(juce::AudioProcessor& processor)
  : gain(createGainParameter(processor)),
    pan(createPanParameter(processor)),
    bypass(createBypassParameter(processor)) {
}
}  // namespace efx
