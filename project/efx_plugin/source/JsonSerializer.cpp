namespace efx {
namespace {
struct SerializableParameters {
  float gain;
  float pan;
  bool bypass;

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
        named("bypass", t.bypass)
    );
  }
};

SerializableParameters from(const efx::Parameters &parameters) {
  return {
      .gain = parameters.gain.get(),
      .pan = parameters.pan.get(),
      .bypass = parameters.bypass.get()
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

  parameters.gain = parsedParameters->gain;
  parameters.pan = parsedParameters->pan;
  parameters.bypass = parsedParameters->bypass;

  return juce::Result::ok();
}
}  // namespace efx
