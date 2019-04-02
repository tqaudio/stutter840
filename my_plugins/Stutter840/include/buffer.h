#pragma once

#include <math.h>

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

protected:
  int mSampleRate;
  int mCapacity;
  int mReadHead;
  int mWriteHead;
  int mLoopEndPosition;
  LoopState mLoopState;
  double mTempo;
  double mRatio;
  int mMinDenominator;
  int mMaxDenominator;
  double *mBuffer;
};
} // namespace Stutter840
