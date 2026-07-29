namespace efx {
namespace {
struct SerializableParameters {
  float gain;
  float pan;
  bool bypass;

  float tremoloRate;
  float tremoloMix;
  float tremoloDepth;
  bool tremoloBypass;
  juce::String tremoloWaveform;

  static constexpr auto marshallingVersion = 1;

  template <typename Archive, typename T>
  static void serialise (Archive& archive, T& t) {
    using namespace juce;

    if (archive.getVersion() != 1) {
      return;
    }

    std::string pluginName = PLUGIN_NAME;

    archive(named("pluginName", pluginName));

    if (pluginName != PLUGIN_NAME) {
      return;
    }

    archive(
        named("gainDB", t.gain),
        named("pan", t.pan),
        named("bypass", t.bypass),
        named("tremoloRate", t.tremoloRate),
        named("tremoloMix", t.tremoloMix),
        named("tremoloDepth", t.tremoloDepth),
        named("tremoloBypass", t.tremoloBypass),
        named("tremoloWaveform", t.tremoloWaveform)
    );
  }
};

SerializableParameters from(const efx::Parameters &parameters) {
  return {
      .gain = parameters.gain.get(),
      .pan = parameters.pan.get(),
      .bypass = parameters.bypass.get(),
      .tremoloRate = parameters.tremoloRate.get(),
      .tremoloMix = parameters.tremoloMix.get(),
      .tremoloDepth = parameters.tremoloDepth.get(),
      .tremoloBypass = parameters.tremoloBypass.get(),
      .tremoloWaveform = parameters.tremoloWaveform.getCurrentChoiceName(),
  };
}
} // namespace

void JsonSerializer::serialize(const Parameters& parameters,
                               juce::OutputStream& output) {
  const auto parametersToSerialize = from(parameters);

  const auto json = juce::ToVar::convert(parametersToSerialize);

  if (!json.has_value()) {
    return;
  }

  juce::JSON::writeToStream(output, *json, juce::JSON::FormatOptions{}
    .withSpacing(juce::JSON::Spacing::multiLine)
    .withMaxDecimalPlaces(2));
}

juce::Result JsonSerializer::deserialize(juce::InputStream& input,
                                         Parameters& parameters) {
  juce::var parsedResult;
  const auto result = juce::JSON::parse(input.readEntireStreamAsString(), parsedResult);

  if(result.failed()) {
    return result;
  }

  const auto parsedParameters = juce::FromVar::convert<SerializableParameters>(parsedResult);

  if(!parsedParameters.has_value()) {
    return juce::Result::fail("Failed to parse parameters from json representation");
  }

  const auto tremoloWaveformIndex = parameters.tremoloWaveform.choices.indexOf(
      parsedParameters->tremoloWaveform);
  if(tremoloWaveformIndex == -1) {
    return juce::Result::fail("non-existent waveform name found");
  }

  parameters.gain = parsedParameters->gain;
  parameters.pan = parsedParameters->pan;
  parameters.bypass = parsedParameters->bypass;

  parameters.tremoloWaveform = tremoloWaveformIndex;
  parameters.tremoloRate = parsedParameters->tremoloRate;
  parameters.tremoloMix = parsedParameters->tremoloMix;
  parameters.tremoloDepth = parsedParameters->tremoloDepth;
  parameters.tremoloBypass = parsedParameters->tremoloBypass;

  return juce::Result::ok();
}
}  // namespace efx
