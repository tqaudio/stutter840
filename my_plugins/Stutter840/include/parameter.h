#pragma once

#define DEFAULT_RATIO 0.0f /* percentage */
#define DEFAULT_RATIO_NORMALIZED DEFAULT_RATIO / 100.0f

#define MAX_DURATION 10000.0f /* ms */
#define BASE_DENOMINATOR 256.0f

#define DEFAULT_MIN_DENOMINATOR 8.0f
#define DEFAULT_MIN_DENOMINATOR_NORMALIZED                                     \
  DEFAULT_MIN_DENOMINATOR / BASE_DENOMINATOR

#define DEFAULT_MAX_DENOMINATOR 32.0f
#define DEFAULT_MAX_DENOMINATOR_NORMALIZED                                     \
  DEFAULT_MAX_DENOMINATOR / BASE_DENOMINATOR

#define DEFAULT_MIDI_CHANNEL_NORMALIZED 0.0f /* OMNI */
#define DEFAULT_MIDI_NOTE_NORMALIZED 1.0f    /* ALL */

#include "public.sdk/source/vst/vsteditcontroller.h"
#include <math.h>

#include "pluginterfaces/base/ustring.h"
#include "public.sdk/source/vst/vsteditcontroller.h"

#define MAX_DENOMINATOR 128.0f

using namespace Steinberg;
using namespace Steinberg::Vst;

namespace Stutter840 {
enum Parameters : ParamID {
  kBypassId = 100,
  kRatioId = 101,
  kMinDenominatorId = 102,
  kMaxDenominatorId = 103,
  kNoteChannelId = 104,
  kNoteNumberId = 105
};

struct AutomationParameter {
public:
  ParamValue value;
  bool hasChanged;
  AutomationParameter() : value(0.0f), hasChanged(false){};
};

class LinearParameter : public Parameter {
public:
  LinearParameter(const char *title, const char *unit, double maxValue,
                  int32 precision, int32 flags, int32 id);
  void toString(ParamValue normValue, String128 text) const SMTG_OVERRIDE;
  bool fromString(const TChar *string,
                  ParamValue &normValue) const SMTG_OVERRIDE;

private:
  double mMaxValue;
  int32 mPrecision;
};

class NoteNumberParameter : public Parameter {
public:
  NoteNumberParameter(const char *title, int32 flags, int32 id);
  void toString(ParamValue normValue, String128 text) const SMTG_OVERRIDE;
  bool fromString(const TChar *string,
                  ParamValue &normValue) const SMTG_OVERRIDE;
};

class MIDIChannelParameter : public Parameter {
public:
  MIDIChannelParameter(const char *title, int32 flags, int32 id);
  void toString(ParamValue normValue, String128 text) const SMTG_OVERRIDE;
  bool fromString(const TChar *string,
                  ParamValue &normValue) const SMTG_OVERRIDE;
};
} // namespace Stutter840
