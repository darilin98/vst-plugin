//
// Created by Darek Rudiš on 20.03.2025.
//
#include "processor.hpp"

tresult PLUGIN_API PluginProcessor::initialize(FUnknown *context)
{
    AudioEffect::initialize(context);
    addAudioInput(USTRING("Stereo In"), SpeakerArr::kStereo);
    addAudioOutput(USTRING("Stereo Out"), SpeakerArr::kStereo);
    return kResultOk;
}

tresult PLUGIN_API PluginProcessor::process(ProcessData& data)
{
    // Later audio processing
    for (int32 i = 0; i < data.numOutputs; i++)
    {
        for (int32 j = 0; j < data.outputs[i].numChannels; j++)
        {
            memset(data.outputs[i].channelBuffers32[j], 0, data.numSamples * sizeof(float));
        }
    }
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

