#include "../include/processor.h"

namespace Stutter840 {
Processor::Processor() { setControllerClass(ControllerID); }

tresult PLUGIN_API Processor::initialize(FUnknown *context) {
  tresult result = AudioEffect::initialize(context);

  if (result != kResultTrue) {
    return kResultFalse;
  }

  addEventInput(STR16("Event Input"), 1);
  addAudioInput(STR16("AudioInput"), SpeakerArr::kStereo);
  addAudioOutput(STR16("AudioOutput"), SpeakerArr::kStereo);

  return kResultTrue;
}

tresult PLUGIN_API Processor::setBusArrangements(SpeakerArrangement *inputs,
                                                 int32 numIns,
                                                 SpeakerArrangement *outputs,
                                                 int32 numOuts) {
  if (numIns == 1 && numOuts == 1 && inputs[0] == outputs[0]) {
    return AudioEffect::setBusArrangements(inputs, numIns, outputs, numOuts);
  }

  return kResultFalse;
}

tresult PLUGIN_API Processor::setupProcessing(ProcessSetup &newSetup) {
  return AudioEffect::setupProcessing(newSetup);
}

tresult PLUGIN_API Processor::setActive(TBool state) {
  SpeakerArrangement arrangement;

  if (getBusArrangement(kOutput, 0, arrangement) != kResultTrue) {
    return kResultFalse;
  }

  int channelCount = SpeakerArr::getChannelCount(arrangement);

  if (channelCount != 2) {
    return kResultFalse;
  }
  if (state) {
    mRatios = new AutomationParameter[processSetup.maxSamplesPerBlock];
    mNoteOns = new AutomationParameter[processSetup.maxSamplesPerBlock];
    mNoteOffs = new AutomationParameter[processSetup.maxSamplesPerBlock];
    mBuffer = new Buffer *[channelCount];

    for (int channel = 0; channel < channelCount; channel++) {
      mBuffer[channel] =
          new Buffer(processSetup.sampleRate, Constants::maxDuration);
      mBuffer[channel]->setMinDenominator(mMinDenominator *
                                          Constants::baseDenominator);
      mBuffer[channel]->setMaxDenominator(mMaxDenominator *
                                          Constants::baseDenominator);
      mBuffer[channel]->setRatio(mRatio);
    }
  } else {
    for (int channel = 0; channel < channelCount; channel++) {
      delete mBuffer[channel];
    }

    delete[] mRatios;
    delete[] mNoteOns;
    delete[] mNoteOffs;
    delete mBuffer;
  }

  return AudioEffect::setActive(state);
}

tresult PLUGIN_API Processor::process(ProcessData &data) {
  if (data.processContext != nullptr) {
    ProcessContext *ctx = data.processContext;
    mBuffer[0]->setTempo(ctx->tempo);
    mBuffer[1]->setTempo(ctx->tempo);
  }
  if (data.inputParameterChanges) {
    int32 numParamsChanged = data.inputParameterChanges->getParameterCount();

    for (int32 index = 0; index < numParamsChanged; index++) {
      IParamValueQueue *paramQueue =
          data.inputParameterChanges->getParameterData(index);

      if (paramQueue == nullptr) {
        continue;
      }

      int32 sampleOffset;
      int32 numPoints = paramQueue->getPointCount();
      ParamValue value;

      if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) !=
          kResultTrue) {
        continue;
      }
      if (sampleOffset < 0) {
        sampleOffset = 0;
      }
      if (sampleOffset > data.numSamples - 1) {
        sampleOffset = data.numSamples - 1;
      }
      switch (paramQueue->getParameterId()) {
      case Parameters::kBypassId:
        mBypass = (value > 0.5);
        break;
      case Parameters::kRatioId:
        mRatio = value;
        mRatios[sampleOffset].value = value;
        mRatios[sampleOffset].hasChanged = true;
        break;
      case Parameters::kMinDenominatorId:
        mMinDenominator = value;
        mBuffer[0]->setMinDenominator(value * Constants::baseDenominator);
        mBuffer[1]->setMinDenominator(value * Constants::baseDenominator);
        break;
      case Parameters::kMaxDenominatorId:
        mMaxDenominator = value;
        mBuffer[0]->setMaxDenominator(value * Constants::baseDenominator);
        mBuffer[1]->setMaxDenominator(value * Constants::baseDenominator);
        break;
      }
    }
  }
  if (data.inputEvents) {
    int eventCount = data.inputEvents->getEventCount();

    for (int i = 0; i < eventCount; i++) {
      Event incomingEvent;

      if (data.inputEvents->getEvent(i, incomingEvent) != kResultOk) {
        continue;
      }

      int sampleOffset = incomingEvent.sampleOffset;

      if (sampleOffset < 0) {
        sampleOffset = 0;
      }
      if (sampleOffset > data.numSamples - 1) {
        sampleOffset = data.numSamples - 1;
      }
      switch (incomingEvent.type) {
      case Event::kNoteOnEvent:
        mNoteOns[sampleOffset].hasChanged = true;
        break;
      case Event::kNoteOffEvent:
        mNoteOffs[sampleOffset].hasChanged = true;
        break;
      }
    }
  }
  if (data.numInputs == 0 || data.numOutputs == 0) {
    return kResultTrue;
  }
  if (data.numSamples > 0) {
    SpeakerArrangement arrangement;
    getBusArrangement(kOutput, 0, arrangement);
    int32 channelCount = SpeakerArr::getChannelCount(arrangement);
    float *inputChannel = nullptr;
    float *outputChannel = nullptr;

    for (int32 sample = 0; sample < data.numSamples; sample++) {
      for (int32 channel = 0; channel < channelCount; channel++) {
        inputChannel = data.inputs[0].channelBuffers32[channel];
        outputChannel = data.outputs[0].channelBuffers32[channel];

        if (mRatios[sample].hasChanged) {
          mBuffer[channel]->setRatio(mRatios[sample].value);
          if (channel == channelCount - 1) {
            mRatios[sample].hasChanged = false;
          }
        }
        if (mNoteOns[sample].hasChanged) {
          mBuffer[channel]->enableLoop();
          if (channel == channelCount - 1) {
            mNoteOns[sample].hasChanged = false;
          }
        }
        if (mNoteOffs[sample].hasChanged) {
          mBuffer[channel]->disableLoop();
          if (channel == channelCount - 1) {
            mNoteOffs[sample].hasChanged = false;
          }
        }

        mBuffer[channel]->write(inputChannel[sample]);
        outputChannel[sample] = mBuffer[channel]->read();
      }
    }
  }

  return kResultTrue;
}

tresult PLUGIN_API Processor::setState(IBStream *state) {
  if (!state) {
    return kResultFalse;
  }

  IBStreamer streamer(state, kLittleEndian);
  int32 savedBypass{0};
  float savedRatio{0.0};
  float savedMinDenominator{0.0};
  float savedMaxDenominator{0.0};

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

  mBypass = savedBypass > 0;
  mRatio = savedRatio;
  mMinDenominator = savedMinDenominator;
  mMaxDenominator = savedMaxDenominator;

  return kResultTrue;
}

tresult PLUGIN_API Processor::getState(IBStream *state) {
  IBStreamer streamer(state, kLittleEndian);
  int32 saveBypass = mBypass ? 1 : 0;
  float saveRatio = mRatio;
  float saveMinDenominator = mMinDenominator;
  float saveMaxDenominator = mMaxDenominator;

  streamer.writeInt32(saveBypass);
  streamer.writeFloat(saveRatio);
  streamer.writeFloat(saveMinDenominator);
  streamer.writeFloat(saveMaxDenominator);

  return kResultTrue;
}
} // namespace Stutter840
