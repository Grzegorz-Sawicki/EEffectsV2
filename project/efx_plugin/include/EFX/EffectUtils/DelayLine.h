#pragma once

class DelayLine {
public:
  DelayLine() = default;

  void prepare(double sampleRate, double maxDelayMs) {
    currentSampleRate = sampleRate;

    bufferSize = static_cast<int>(std::ceil(sampleRate * (maxDelayMs / 1000.0))) + 10;
    buffer.assign(static_cast<size_t>(bufferSize), 0.0f);
    writeIndex = 0;
  }

  void reset() {
    std::fill(buffer.begin(), buffer.end(), 0.0f);
    writeIndex = 0;
  }

  void pushSample(float sample) {
    buffer.at(writeIndex) = sample;
    writeIndex = (writeIndex + 1) % bufferSize;
  }

  float popSample(float delayMs) {
    const float delaySamples = (delayMs / 1000.0f) * static_cast<float>(currentSampleRate);
    float readPosition = static_cast<float>(writeIndex) - delaySamples;

    while (readPosition < 0.0f) {
      readPosition += static_cast<float>(bufferSize);
    }

    const int indexA = static_cast<int>(readPosition);
    const int indexB = (indexA + 1) % bufferSize;
    const float fraction = readPosition - static_cast<float>(indexA);

    const float sampleA = buffer.at(indexA);
    const float sampleB = buffer.at(indexB);

    return sampleA + fraction * (sampleB - sampleA);
  }

private:
  std::vector<float> buffer;
  int bufferSize{0};
  int writeIndex{0};
  double currentSampleRate{44100.0};
};