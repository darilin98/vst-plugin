//
// Created by Darek Rudiš on 20.03.2025.
//
#include "processor.hpp"
#include "controller.hpp"

#include <sys/proc.h>

#include "base/source/fstreamer.h"
#include "vst/ivstparameterchanges.h"

tresult PLUGIN_API PluginProcessor::initialize(FUnknown *context)
{
    tresult result = AudioEffect::initialize(context);
    if (result != kResultOk) {
        return result;
    }
    addAudioInput(USTRING("Stereo In"), SpeakerArr::kStereo);
    addAudioOutput(USTRING("Stereo Out"), SpeakerArr::kStereo);
    return kResultOk;
}
tresult PLUGIN_API PluginProcessor::setupProcessing(ProcessSetup &setup)
{
    tresult result = AudioEffect::setupProcessing(setup);
    if(result != kResultOk)
        return result;

    BusInfo busInfo;
    if (getBusInfo(kAudio, kInput, 0, busInfo) == kResultOk)
    {
        int32 maxChannels = busInfo.channelCount;
        fft_processors_.resize(maxChannels);

        for (int32 i = 0; i < maxChannels; ++i)
        {
            fft_processors_[i] = std::make_unique<FFTProcessor>();
            fft_processors_[i]->prepare(FFT_SIZE);
        }
    }
    return kResultOk;
}

tresult PLUGIN_API PluginProcessor::process(ProcessData& data)
{
    if (!(data.numInputs > 0 && data.inputs[0].numChannels > 0))
        return kResultOk;

    bool isBypass = getBypassState(data);

    const int32 numSamples = data.numSamples;
    const float sampleRate = this->processSetup.sampleRate;
    const int32 numChannels = std::min(data.inputs[0].numChannels, static_cast<int32>(fft_processors_.size()));

    if (isBypass)
    {
        for (int32 ch = 0; ch < numChannels; ++ch)
        {
            float* input = data.inputs[0].channelBuffers32[ch];
            float* output = data.outputs[0].channelBuffers32[ch];

            if (input && output) {
                std::copy(input, input + numSamples, output);
            }
        }
        return kResultOk;
    }
    for (int32 ch = 0; ch < numChannels; ++ch)
    {
        float* input = data.inputs[0].channelBuffers32[ch];
        float* output = data.outputs[0].channelBuffers32[ch];

        if (input && output) {
            std::copy(input, input + numSamples, output);
            fft_processors_[ch]->process(input, output, sampleRate, numSamples);
        }
    }
    return kResultOk;
}

bool PluginProcessor::getBypassState(const ProcessData &data) const
{
    bool bypassActive = bypassState == 1.0f;  // Default to current stored state

    if (auto* pc = data.inputParameterChanges) {
        for (int32 i = 0; i < pc->getParameterCount(); ++i) {
            if (auto* queue = pc->getParameterData(i)) {
                if (queue->getParameterId() == kParamBypass) {
                    int32 sampleOffset;
                    ParamValue value;
                    if (queue->getPoint(queue->getPointCount() - 1, sampleOffset, value) == kResultTrue) {
                        if (!bypassActive && value == 1.0f) {
                            for (auto& fft : fft_processors_)
                                if (fft) fft->reset();
                        }
                        bypassActive = value == 1.0f;
                        const_cast<PluginProcessor*>(this)->bypassState = value;
                    }
                }
            }
        }
    }
    return bypassActive;
}

tresult PLUGIN_API PluginProcessor::getControllerClassId(TUID classId)
{
    if (!classId)
        return kInvalidArgument;
    // Copy the controller's FUID into cid

    PluginControllerUID.toTUID(classId);
    return kResultOk;
}

tresult PLUGIN_API PluginProcessor::terminate()
{
    return AudioEffect::terminate();
}

tresult PluginProcessor::setState(IBStream *state)
{
    if (!state)
        return kResultFalse;

    IBStreamer streamer(state, kLittleEndian);

    if (!streamer.readDouble(bypassState))
        return kResultFalse;
    return kResultOk;
}

tresult PluginProcessor::getState(IBStream *state)
{
    if (!state)
        return kResultFalse;

    IBStreamer streamer(state, kLittleEndian);

    if (!streamer.writeDouble(bypassState))
        return kResultFalse;

    return kResultOk;
}

