#pragma once
namespace efx {
class VUMeter : public juce::Component, public juce::Timer {
public:
  VUMeter(std::atomic<float> &leftSource, std::atomic<float> &rightSource) : leftPeakSource(leftSource),
                                                                              rightPeakSource(rightSource) {
    startTimerHz(30);
  }

  void timerCallback() override
  {
    const float newLeft = leftPeakSource.load();
    const float newRight = rightPeakSource.load();

    //DBG("lps = " << newLeft << " rps = " << newRight);

    if (newLeft > visualLeft) {
      visualLeft += attackFactor * (newLeft - visualLeft);
    } else {
      visualLeft += decayFactor * (newLeft - visualLeft);
    }

    if (newRight > visualRight) {
      visualRight += attackFactor * (newRight - visualRight);
    } else {
      visualRight += decayFactor * (newRight - visualRight);
    }

    repaint();
  }

  void paint (juce::Graphics& g) override {
    auto bounds = getLocalBounds();

    g.setColour(juce::Colours::black);
    g.fillAll();

    const auto padding = 4;
    bounds = bounds.reduced(padding);

    const auto meterWidth = 8;
    const auto meterHeight = 198;
    const auto meterSpace = 2;

    auto leftMeterBounds = juce::Rectangle<int>{padding, padding, meterWidth, meterHeight};

    auto rightMeterBounds = juce::Rectangle<int>{
      leftMeterBounds.getX() + meterWidth + meterSpace,
      padding,
      meterWidth,
      meterHeight
    };

    drawMeterBar(g, leftMeterBounds, visualLeft);
    drawMeterBar(g, rightMeterBounds, visualRight);

    //DBG("left = " << visualLeft << " and right = " << visualRight << "\n");
  }

private:
  void drawMeterBar (juce::Graphics& g, juce::Rectangle<int> bounds, float peakValue) {
    float levelDecibels = juce::Decibels::gainToDecibels(peakValue, -60.0f);
    float levelProportion = juce::jmap(levelDecibels, -60.0f, 6.0f, 0.0f, 1.0f);
    levelProportion = juce::jlimit(0.0f, 1.0f, levelProportion);

    auto gradient = juce::ColourGradient::vertical(
        juce::Colours::red,
        juce::Colours::limegreen,
        bounds
        );

    gradient.addColour (0.1f, juce::Colours::yellow);

    float fillHeight = bounds.getHeight() * levelProportion;
    auto filledArea = bounds.removeFromBottom(fillHeight);

    g.setGradientFill(gradient);
    g.fillRect(filledArea);
  }

  const float attackFactor = 0.8f;
  const float decayFactor = 0.15f;

  std::atomic<float>& leftPeakSource;
  std::atomic<float>& rightPeakSource;

  float visualLeft = -100.0f;
  float visualRight = -100.0f;
};
}
