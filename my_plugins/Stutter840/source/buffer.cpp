#include "../include/buffer.h"

#include <stdio.h>

namespace Stutter840 {
Buffer::Buffer(int sampleRate, double capacity /* ms */)
    : mSampleRate(sampleRate), mCapacity(sampleRate * capacity / 1000.0f),
      mReadHead(0), mWriteHead(0), mLoopEndPosition(0), mRatio(0.0f),
      mTempo(120.0f), mLoopState(kDisabled), mMinDenominator(8),
      mMaxDenominator(64) {
  mBuffer = new double[mCapacity];

  for (int i = 0; i < mCapacity; i++) {
    mBuffer[i] = 0.0f;
  }
}

Buffer::~Buffer() { delete[] mBuffer; }

double Buffer::read() {
  double result = mBuffer[mReadHead];

  if (mLoopState == kDisabled) {
    return result;
  }

  mReadHead += 1;

  if (mReadHead > mLoopEndPosition - 1 || mReadHead > mCapacity - 1) {
    mReadHead = 0;
  }

  double p = (double)mReadHead / (double)mLoopEndPosition;
  double gain = 1.0f - pow(p, 7.912f);

  return result * gain;
}

void Buffer::write(double value) {
  mBuffer[mWriteHead] = value;

  if (mLoopState == kDisabled) {
    return;
  }
  if (mWriteHead > mCapacity - 1) {
    return;
  }

  mWriteHead += 1;
}

void Buffer::enableLoop() { mLoopState = kEnabled; }

void Buffer::disableLoop() {
  mLoopState = kDisabled;
  mWriteHead = 0;
  mReadHead = 0;
}

void Buffer::setTempo(double tempo) { mTempo = tempo; }

void Buffer::setMinDenominator(double denominator) {
  mMinDenominator = (int)round(denominator);
  if (mMinDenominator < 1) {
    mMinDenominator = 1;
  }
  setRatio(mRatio);
}

void Buffer::setMaxDenominator(double denominator) {
  mMaxDenominator = (int)round(denominator);
  if (mMaxDenominator < 1) {
    mMaxDenominator = 1;
  }
  setRatio(mRatio);
}

void Buffer::setRatio(double ratio) {
  double base = (double)mSampleRate * 60.0f / mTempo;
  double denominator =
      (mMinDenominator + (mMaxDenominator - mMinDenominator) * ratio) / 4.0f;

  mRatio = ratio;
  mLoopEndPosition = (int)(base / denominator);
}
} // namespace Stutter840
