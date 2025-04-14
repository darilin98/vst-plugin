//
// Created by Darek Rudiš on 20.03.2025.
//
#include "processor.hpp"

#include <sys/proc.h>

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
            fft_processors_[i]->prepare(1024);
        }
    }
    return kResultOk;
}

tresult PLUGIN_API PluginProcessor::process(ProcessData& data)
{
    if (!(data.numInputs > 0 && data.inputs[0].numChannels > 0))
        return kResultOk;

    const int32 numSamples = data.numSamples;
    if (numSamples == 0)
        return kResultOk;

    const float sampleRate = this->processSetup.sampleRate;

    const int32 numChannels = data.inputs[0].numChannels;

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
    return kResultOk;
}

tresult PluginProcessor::getState(IBStream *state)
{
    return kResultOk;
}

