#pragma once

#include "id.h"
#include "parameter.h"

#include "base/source/fstreamer.h"
#include "buffer.h"
#include "pluginterfaces/base/ibstream.h"
#include "pluginterfaces/vst/ivstevents.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "pluginterfaces/vst/ivstprocesscontext.h"
#include "public.sdk/source/vst/vstaudioeffect.h"

using namespace Steinberg;
using namespace Steinberg::Vst;

namespace Stutter840 {
class Processor : public AudioEffect {
public:
  static FUnknown *createInstance(void *) {
    return (IAudioProcessor *)new Processor();
  }

  Processor();

  tresult PLUGIN_API initialize(FUnknown *context) SMTG_OVERRIDE;
  tresult PLUGIN_API setBusArrangements(SpeakerArrangement *inputs,
                                        int32 numIns,
                                        SpeakerArrangement *outputs,
                                        int32 numOuts) SMTG_OVERRIDE;
  tresult PLUGIN_API setupProcessing(ProcessSetup &newSetup) SMTG_OVERRIDE;
  tresult PLUGIN_API setActive(TBool state) SMTG_OVERRIDE;
  tresult PLUGIN_API process(ProcessData &data) SMTG_OVERRIDE;
  tresult PLUGIN_API setState(IBStream *state) SMTG_OVERRIDE;
  tresult PLUGIN_API getState(IBStream *state) SMTG_OVERRIDE;

protected:
  Buffer *mBuffer[2];

  AutomationParameter *mNoteOns;
  AutomationParameter *mNoteOffs;
  AutomationParameter *mRatios;

  bool mBypass;
  ParamValue mRatio;
  ParamValue mMinDenominator;
  ParamValue mMaxDenominator;
  ParamValue mNoteChannel;
  ParamValue mNoteNumber;
};
} // namespace Stutter840
