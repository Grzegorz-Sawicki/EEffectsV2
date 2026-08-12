
namespace efx {
namespace {
auto &addParameterToProcessor(juce::AudioProcessor &processor, auto parameter) {
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
      juce::AudioParameterFloatAttributes{}
          .withLabel("dB")
          .withStringFromValueFunction([](float x, int) {
            return juce::String(x, 1) + "dB";
          })
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
      juce::NormalisableRange{0.1f, 20.0f, 0.01f, 0.4f},
      5.f,
      juce::AudioParameterFloatAttributes{}
          .withLabel("Hz")
          .withStringFromValueFunction([](float value, int /* maximumStringLength */) {
            return juce::String(value, 2) + " Hz";
          })
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

juce::AudioParameterBool &createTremoloActiveParameter(juce::AudioProcessor &processor) {
  auto parameter = std::make_unique<juce::AudioParameterBool>(
      juce::ParameterID{"tremolo.active", defaultVersionHint},
      juce::String{"Tremolo active"},
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

juce::AudioParameterFloat &createFlangerRateParameter(juce::AudioProcessor &processor) {
  auto parameter = std::make_unique<juce::AudioParameterFloat>(
      juce::ParameterID{"flanger.modulation.rate", defaultVersionHint},
      juce::String{"Flanger modulation rate"},
      juce::NormalisableRange{0.05f, 10.f, 0.01f, 0.4f},
      1.0f,
      juce::AudioParameterFloatAttributes{}
          .withLabel("Hz")
          .withStringFromValueFunction([](float value, int /* maximumStringLength */) {
            return juce::String(value, 2) + " Hz";
          })
  );
  return addParameterToProcessor(processor, std::move(parameter));
}

juce::AudioParameterFloat &createFlangerMixParameter(juce::AudioProcessor &processor) {
  auto parameter = std::make_unique<juce::AudioParameterFloat>(
      juce::ParameterID{"flanger.mix", defaultVersionHint},
      juce::String{"Flanger mix"},
      juce::NormalisableRange{0.f, 1.f, 0.001f},
      1.f
  );
  return addParameterToProcessor(processor, std::move(parameter));
}

juce::AudioParameterFloat &createFlangerDepthParameter(juce::AudioProcessor &processor) {
  auto parameter = std::make_unique<juce::AudioParameterFloat>(
      juce::ParameterID{"flanger.depth", defaultVersionHint},
      juce::String{"Flanger depth"},
      juce::NormalisableRange{0.f, 1.f, 0.001f},
      0.5f,
      juce::AudioParameterFloatAttributes{}
          .withStringFromValueFunction([](float x, int) { return juce::String(x, 3); })
  );
  return addParameterToProcessor(processor, std::move(parameter));
}

juce::AudioParameterFloat &createFlangerFeedbackParameter(juce::AudioProcessor &processor) {
  auto parameter = std::make_unique<juce::AudioParameterFloat>(
      juce::ParameterID{"flanger.feedback", defaultVersionHint},
      juce::String{"Flanger feedback"},
      juce::NormalisableRange{-0.95f, 0.95f, 0.01f},
      0.3f,
      juce::AudioParameterFloatAttributes{}
          .withStringFromValueFunction([](float x, int) { return juce::String(x, 3); })
  );
  return addParameterToProcessor(processor, std::move(parameter));
}

juce::AudioParameterBool &createFlangerActiveParameter(juce::AudioProcessor &processor) {
  auto parameter = std::make_unique<juce::AudioParameterBool>(
      juce::ParameterID{"flanger.active", defaultVersionHint},
      juce::String{"Flanger active"},
      false
  );
  return addParameterToProcessor(processor, std::move(parameter));
}

juce::AudioParameterFloat &createLowpassFrequencyParameter(juce::AudioProcessor &processor) {
  juce::NormalisableRange<float> freqRange{20.0f, 20000.0f, 1.0f};
  freqRange.setSkewForCentre(1000.0f);

  auto parameter = std::make_unique<juce::AudioParameterFloat>(
      juce::ParameterID{"lowpass.frequency", defaultVersionHint},
      juce::String{"Lowpass cutoff frequency"},
      freqRange,
      20000.0f,
      juce::AudioParameterFloatAttributes{}
          .withLabel("Hz")
          .withStringFromValueFunction([](float value, int /* maximumStringLength */) {
            return juce::String(value, 2) + " Hz";
          })
  );
  return addParameterToProcessor(processor, std::move(parameter));
}

juce::AudioParameterFloat &createLowpassResonanceParameter(juce::AudioProcessor &processor) {
  juce::NormalisableRange<float> qRange{0.1f, 10.0f, 0.01f};
  qRange.setSkewForCentre(1.0f);

  auto parameter = std::make_unique<juce::AudioParameterFloat>(
      juce::ParameterID{"lowpass.resonance", defaultVersionHint},
      juce::String{"Lowpass resonance"},
      qRange,
      0.7071f,
      juce::AudioParameterFloatAttributes{}.withLabel("Q")
  );
  return addParameterToProcessor(processor, std::move(parameter));
}

juce::AudioParameterFloat &createLowpassMixParameter(juce::AudioProcessor &processor) {
  auto parameter = std::make_unique<juce::AudioParameterFloat>(
      juce::ParameterID{"lowpass.mix", defaultVersionHint},
      juce::String{"Lowpass mix"},
      juce::NormalisableRange{0.f, 1.f, 0.001f},
      1.f
  );
  return addParameterToProcessor(processor, std::move(parameter));
}

juce::AudioParameterBool &createLowpassActiveParameter(juce::AudioProcessor &processor) {
  auto parameter = std::make_unique<juce::AudioParameterBool>(
      juce::ParameterID{"lowpass.active", defaultVersionHint},
      juce::String{"Lowpass active"},
      false
  );
  return addParameterToProcessor(processor, std::move(parameter));
}

juce::AudioParameterFloat &createHighpassFrequencyParameter(juce::AudioProcessor &processor) {
  juce::NormalisableRange<float> freqRange{20.0f, 20000.0f, 1.0f};
  freqRange.setSkewForCentre(1000.0f);

  auto parameter = std::make_unique<juce::AudioParameterFloat>(
      juce::ParameterID{"highpass.frequency", defaultVersionHint},
      juce::String{"Highpass cutoff frequency"},
      freqRange,
      20.0f,
      juce::AudioParameterFloatAttributes{}
          .withLabel("Hz")
          .withStringFromValueFunction([](float value, int /* maximumStringLength */) {
            return juce::String(value, 2) + " Hz";
          })
  );
  return addParameterToProcessor(processor, std::move(parameter));
}

juce::AudioParameterFloat &createHighpassResonanceParameter(juce::AudioProcessor &processor) {
  juce::NormalisableRange<float> qRange{0.1f, 10.0f, 0.01f};
  qRange.setSkewForCentre(1.0f);

  auto parameter = std::make_unique<juce::AudioParameterFloat>(
      juce::ParameterID{"highpass.resonance", defaultVersionHint},
      juce::String{"Highpass resonance"},
      qRange,
      0.7071f,
      juce::AudioParameterFloatAttributes{}.withLabel("Q")
  );
  return addParameterToProcessor(processor, std::move(parameter));
}

juce::AudioParameterFloat &createHighpassMixParameter(juce::AudioProcessor &processor) {
  auto parameter = std::make_unique<juce::AudioParameterFloat>(
      juce::ParameterID{"highpass.mix", defaultVersionHint},
      juce::String{"Highpass mix"},
      juce::NormalisableRange{0.f, 1.f, 0.001f},
      1.f
  );
  return addParameterToProcessor(processor, std::move(parameter));
}

juce::AudioParameterBool &createHighpassActiveParameter(juce::AudioProcessor &processor) {
  auto parameter = std::make_unique<juce::AudioParameterBool>(
      juce::ParameterID{"highpass.active", defaultVersionHint},
      juce::String{"Highpass active"},
      false
  );
  return addParameterToProcessor(processor, std::move(parameter));
}

juce::AudioParameterFloat &createBandpassFrequencyParameter(juce::AudioProcessor &processor) {
  juce::NormalisableRange<float> freqRange{20.0f, 20000.0f, 1.0f};
  freqRange.setSkewForCentre(1000.0f);

  auto parameter = std::make_unique<juce::AudioParameterFloat>(
      juce::ParameterID{"bandpass.frequency", defaultVersionHint},
      juce::String{"Bandpass cutoff frequency"},
      freqRange,
      20.0f,
      juce::AudioParameterFloatAttributes{}
          .withLabel("Hz")
          .withStringFromValueFunction([](float value, int /* maximumStringLength */) {
            return juce::String(value, 2) + " Hz";
          })
  );
  return addParameterToProcessor(processor, std::move(parameter));
}

juce::AudioParameterFloat &createBandpassResonanceParameter(juce::AudioProcessor &processor) {
  juce::NormalisableRange<float> qRange{0.1f, 10.0f, 0.01f};
  qRange.setSkewForCentre(1.0f);

  auto parameter = std::make_unique<juce::AudioParameterFloat>(
      juce::ParameterID{"bandpass.resonance", defaultVersionHint},
      juce::String{"Bandpass resonance"},
      qRange,
      1.0f,
      juce::AudioParameterFloatAttributes{}.withLabel("Q")
  );
  return addParameterToProcessor(processor, std::move(parameter));
}

juce::AudioParameterFloat &createBandpassMixParameter(juce::AudioProcessor &processor) {
  auto parameter = std::make_unique<juce::AudioParameterFloat>(
      juce::ParameterID{"bandpass.mix", defaultVersionHint},
      juce::String{"Bandpass mix"},
      juce::NormalisableRange{0.f, 1.f, 0.001f},
      1.f
  );
  return addParameterToProcessor(processor, std::move(parameter));
}

juce::AudioParameterBool &createBandpassActiveParameter(juce::AudioProcessor &processor) {
  auto parameter = std::make_unique<juce::AudioParameterBool>(
      juce::ParameterID{"bandpass.active", defaultVersionHint},
      juce::String{"Bandpass active"},
      false
  );
  return addParameterToProcessor(processor, std::move(parameter));
}

}  // namespace

Parameters::Parameters(juce::AudioProcessor &processor)
    : gain(createGainParameter(processor)),
      pan(createPanParameter(processor)),
      bypass(createBypassParameter(processor)),

      tremoloRate(createTremoloRateParameter(processor)),
      tremoloMix(createTremoloMixParameter(processor)),
      tremoloDepth(createTremoloDepthParameter(processor)),
      tremoloActive(createTremoloActiveParameter(processor)),
      tremoloWaveform(createTremoloWaveformParameter(processor)),

      flangerRate(createFlangerRateParameter(processor)),
      flangerMix(createFlangerMixParameter(processor)),
      flangerDepth(createFlangerDepthParameter(processor)),
      flangerActive(createFlangerActiveParameter(processor)),
      flangerFeedback(createFlangerFeedbackParameter(processor)),

      lowpassFrequency(createLowpassFrequencyParameter(processor)),
      lowpassResonance(createLowpassResonanceParameter(processor)),
      lowpassMix(createLowpassMixParameter(processor)),
      lowpassActive(createLowpassActiveParameter(processor)),

      highpassFrequency(createHighpassFrequencyParameter(processor)),
      highpassResonance(createHighpassResonanceParameter(processor)),
      highpassMix(createHighpassMixParameter(processor)),
      highpassActive(createHighpassActiveParameter(processor)),

      bandpassFrequency(createBandpassFrequencyParameter(processor)),
      bandpassResonance(createBandpassResonanceParameter(processor)),
      bandpassMix(createBandpassMixParameter(processor)),
      bandpassActive(createBandpassActiveParameter(processor)) {
}
}  // namespace efx
