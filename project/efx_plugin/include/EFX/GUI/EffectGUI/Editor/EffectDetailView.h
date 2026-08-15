#pragma once

namespace efx {
class EffectDetailView : public juce::Component {
public:
  EffectDetailView(PluginProcessor &processor)
      : tremoloEditor(processor), flangerEditor(processor), filterEditor(processor) {
    addChildComponent(tremoloEditor);
    addChildComponent(flangerEditor);
    addChildComponent(filterEditor);
  }

  void resized() override {
    auto bounds = getLocalBounds();

    tremoloEditor.setBounds(bounds);
    flangerEditor.setBounds(bounds);
    filterEditor.setBounds(bounds);
  }

  void showEditor(const juce::String &effectName) {
    tremoloEditor.setVisible(false);
    flangerEditor.setVisible(false);
    filterEditor.setVisible(false);

    if (effectName == "Tremolo") tremoloEditor.setVisible(true);
    else if (effectName == "Flanger") flangerEditor.setVisible(true);
    else if (effectName == "Filter") filterEditor.setVisible(true);
  }

private:
  TremoloEditor tremoloEditor;
  FlangerEditor flangerEditor;
  FilterEditor filterEditor;
};
} // namespace efx