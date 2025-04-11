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
    fft_processor_.prepare(1024);
    addAudioInput(USTRING("Stereo In"), SpeakerArr::kStereo);
    addAudioOutput(USTRING("Stereo Out"), SpeakerArr::kStereo);
    return kResultOk;
}

tresult PLUGIN_API PluginProcessor::process(ProcessData& data)
{
    if (!(data.numInputs > 0 && data.inputs[0].numChannels > 0))
        return kResultOk;

    const int32 numSamples = data.numSamples;
    if (numSamples == 0)
        return kResultOk;

    float sampleRate = this->processSetup.sampleRate;

    // Assuming only mono input for now
    float* input = data.inputs[0].channelBuffers32[0];
    float* output = data.outputs[0].channelBuffers32[0];

    fft_processor_.process(input, output, sampleRate, numSamples);

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

