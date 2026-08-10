/*
==============================================================================

BEGIN_JUCE_MODULE_DECLARATION

   ID:            efx_plugin
   vendor:        Embo
   version:       0.0.0
   name:          EFX
   description:   EFX
   dependencies:  juce_audio_utils, juce_dsp

   website:       null
   license:       MIT

END_JUCE_MODULE_DECLARATION

==============================================================================
*/

#pragma once

#include <juce_graphics/juce_graphics.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_core/juce_core.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_events/juce_events.h>
#include <juce_dsp/juce_dsp.h>
#include <vector>
#include <memory>
#include <functional>
#include <ranges>
#include <algorithm>
#include <array>
#include <cmath>

#include "include/EFX/Params/Parameters.h"
#include "include/EFX/GUI/CustomLookAndFeel.h"
#include "include/EFX/Params/JsonSerializer.h"

#include "include/EFX/EffectProcessors/EffectProcessorBase.h"
#include "include/EFX/EffectProcessors/GainProcessor.h"
#include "include/EFX/EffectProcessors/PanProcessor.h"

#include "include/EFX/EffectUtils/LFO.h"
#include "include/EFX/EffectUtils/DelayLine.h"

#include "include/EFX/EffectProcessors/Complex/ComplexEffectProcessorBase.h"
#include "include/EFX/EffectProcessors/Complex/TremoloProcessor.h"
#include "include/EFX/EffectProcessors/Complex/FlangerProcessor.h"
#include "include/EFX/EffectProcessors/Complex/FilterProcessor.h"

#include "include/EFX/GUI/VUMeter.h"
#include "include/EFX/PluginProcessor.h"
#include "include/EFX/PluginEditor.h"
