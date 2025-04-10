//
// Created by Darek Rudiš on 20.03.2025.
//
#include "processor.hpp"

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

tresult PLUGIN_API PluginProcessor::process(ProcessData& data)
{
    if (!(data.numInputs > 0 && data.inputs[0].numChannels > 0))
        return kResultOk;

    const int32 numSamples = data.numSamples;
    if (numSamples == 0)
        return kResultOk;

    // Assuming only mono input for now
    float* input = data.inputs[0].channelBuffers32[0];

    auto* out = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * (numSamples /2 + 1));
    auto* in = (float*)fftwf_malloc(sizeof(float) * numSamples);

    std::copy(input, input + numSamples, in);
    fftwf_plan plan = fftwf_plan_dft_r2c_1d(numSamples, in, out, FFTW_ESTIMATE);

    fftwf_execute(plan);

    fftwf_destroy_plan(plan);
    fftwf_free(out);
    fftwf_free(in);

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

