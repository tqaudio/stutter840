#pragma once

#include "constants.h"

namespace Stutter840 {
enum LoopState { kEnabled = 100, kDisabled = 101 };

class Buffer {
public:
  Buffer(int sampleRate, double capacity /* ms */);
  ~Buffer();

  void setMinDenominator(double denominator);
  void setMaxDenominator(double denominator);
  void setTempo(double tempo);
  void setRatio(double ratio);

  void enableLoop();
  void disableLoop();

  double read();
  void write(double value);

private:
  int mSampleRate;                                   // e.g. 44100 (Hz)
  int mCapacity;                                     // e.g. 441000 (samples)
  double mTempo = 120.0;                             // e.g. 120.0 (bpm)
  double mRatio = Constants::defaultRatioNormalized; // e.g.
  int mMinDenominator = Constants::defaultMinDenominator;
  int mMaxDenominator = Constants::defaultMaxDenominator;
  int mReadHead = 0;
  int mWriteHead = 0;
  int mLoopEndPosition = 0;
  LoopState mLoopState = LoopState::kDisabled;
  double *mBuffer = nullptr;
};
} // namespace Stutter840
