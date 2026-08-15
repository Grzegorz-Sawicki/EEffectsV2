#pragma once

namespace efx {
class Background : public juce::Component {
public:
  explicit Background(juce::Colour color) : color(color) {};

  void paint(juce::Graphics &g) override {
    g.fillAll(color);
  }

private:
  juce::Colour color;
};
} // namespace efx

