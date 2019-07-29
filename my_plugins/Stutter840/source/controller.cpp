#include "../include/constants.h"
#include "../include/controller.h"

namespace Stutter840 {
tresult PLUGIN_API Controller::initialize(FUnknown *context) {
  tresult result = EditController::initialize(context);

  if (result != kResultOk) {
    return result;
  }

  parameters.addParameter(STR16("Bypass"), 0, 1, 0,
                          ParameterInfo::kCanAutomate |
                              ParameterInfo::kIsBypass,
                          Parameters::kBypassId);

  LinearParameter *ratio =
      new LinearParameter("Ratio", "", 100.0f, 2, ParameterInfo::kCanAutomate,
                          Parameters::kRatioId);
  parameters.addParameter(ratio);

  LinearParameter *minDenominator = new LinearParameter(
      "Min Denominator", "", Constants::baseDenominator, 0, ParameterInfo::kCanAutomate,
      Parameters::kMinDenominatorId);
  parameters.addParameter(minDenominator);

  LinearParameter *maxDenominator = new LinearParameter(
      "Max Denominator", "", Constants::baseDenominator, 0, ParameterInfo::kCanAutomate,
      Parameters::kMaxDenominatorId);
  parameters.addParameter(maxDenominator);

  return result;
}

tresult PLUGIN_API Controller::setComponentState(IBStream *state) {
  if (!state) {
    return kResultFalse;
  }

  IBStreamer streamer(state, kLittleEndian);

  int32 savedBypass;
  float savedRatio = 0.0;
  float savedMinDenominator = 0.0;
  float savedMaxDenominator = 0.0;

  if (!streamer.readInt32(savedBypass)) {
    return kResultFalse;
  }
  if (!streamer.readFloat(savedRatio)) {
    return kResultFalse;
  }
  if (!streamer.readFloat(savedMinDenominator)) {
    return kResultFalse;
  }
  if (!streamer.readFloat(savedMaxDenominator)) {
    return kResultFalse;
  }

  setParamNormalized(Parameters::kBypassId, savedBypass ? 1 : 0);
  setParamNormalized(Parameters::kRatioId, savedRatio);
  setParamNormalized(Parameters::kMinDenominatorId, savedMinDenominator);
  setParamNormalized(Parameters::kMaxDenominatorId, savedMaxDenominator);

  return kResultOk;
}

IPlugView *PLUGIN_API Controller::createView(const char *name) {
  if (name && strcmp(name, "editor") == 0) {
    auto *view = new VST3Editor(this, "Stutter840", "main.uidesc");
    return view;
  }
  return nullptr;
}
} // namespace Stutter840
