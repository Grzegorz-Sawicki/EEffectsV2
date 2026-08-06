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

  float flangerRate;
  float flangerMix;
  float flangerDepth;
  float flangerFeedback;
  bool flangerBypass;

  float lowpassFrequency;
  float lowpassResonance;
  float lowpassMix;
  bool lowpassBypass;

  float highpassFrequency;
  float highpassResonance;
  float highpassMix;
  bool highpassBypass;

  float bandpassFrequency;
  float bandpassResonance;
  float bandpassMix;
  bool bandpassBypass;

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
        named("tremoloWaveform", t.tremoloWaveform),
        named("flangerRate", t.flangerRate),
        named("flangerMix", t.flangerMix),
        named("flangerDepth", t.flangerDepth),
        named("flangerFeedback", t.flangerFeedback),
        named("flangerBypass", t.flangerBypass),
        named("lowpassFrequency", t.lowpassFrequency),
        named("lowpassResonance", t.lowpassResonance),
        named("lowpassMix", t.lowpassMix),
        named("lowpassBypass", t.lowpassBypass),
        named("highpassFrequency", t.highpassFrequency),
        named("highpassResonance", t.highpassResonance),
        named("highpassMix", t.highpassMix),
        named("highpassBypass", t.highpassBypass),
        named("bandpassFrequency", t.bandpassFrequency),
        named("bandpassResonance", t.bandpassResonance),
        named("bandpassMix", t.bandpassMix),
        named("bandpassBypass", t.bandpassBypass)
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
      .flangerRate = parameters.flangerRate.get(),
      .flangerMix = parameters.flangerMix.get(),
      .flangerDepth = parameters.flangerDepth.get(),
      .flangerFeedback = parameters.flangerFeedback.get(),
      .flangerBypass = parameters.flangerBypass.get(),
      .lowpassFrequency = parameters.lowpassFrequency.get(),
      .lowpassResonance = parameters.lowpassResonance.get(),
      .lowpassMix = parameters.lowpassMix.get(),
      .lowpassBypass = parameters.lowpassBypass.get(),
      .highpassFrequency = parameters.highpassFrequency.get(),
      .highpassResonance = parameters.highpassResonance.get(),
      .highpassMix = parameters.highpassMix.get(),
      .highpassBypass = parameters.highpassBypass.get(),
      .bandpassFrequency = parameters.bandpassFrequency.get(),
      .bandpassResonance = parameters.bandpassResonance.get(),
      .bandpassMix = parameters.bandpassMix.get(),
      .bandpassBypass = parameters.bandpassBypass.get()
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

  parameters.flangerBypass = parsedParameters->flangerBypass;
  parameters.flangerRate = parsedParameters->flangerRate;
  parameters.flangerMix = parsedParameters->flangerMix;
  parameters.flangerDepth = parsedParameters->flangerDepth;
  parameters.flangerFeedback = parsedParameters->flangerFeedback;

  parameters.lowpassFrequency = parsedParameters->lowpassFrequency;
  parameters.lowpassResonance = parsedParameters->lowpassResonance;
  parameters.lowpassMix = parsedParameters->lowpassMix;
  parameters.lowpassBypass = parsedParameters->lowpassBypass;

  parameters.highpassFrequency = parsedParameters->highpassFrequency;
  parameters.highpassResonance = parsedParameters->highpassResonance;
  parameters.highpassMix = parsedParameters->highpassMix;
  parameters.highpassBypass = parsedParameters->highpassBypass;

  parameters.bandpassFrequency = parsedParameters->bandpassFrequency;
  parameters.bandpassResonance = parsedParameters->bandpassResonance;
  parameters.bandpassMix = parsedParameters->bandpassMix;
  parameters.bandpassBypass = parsedParameters->bandpassBypass;

  return juce::Result::ok();
}
}  // namespace efx
