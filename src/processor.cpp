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

    auto* in = (float*)fftwf_malloc(sizeof(float) * numSamples);
    auto* out = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * (numSamples /2 + 1));
    auto* processed = (float*)fftwf_malloc(sizeof(float) * numSamples);

    std::copy(input, input + numSamples, in);
    fftwf_plan planFwd = fftwf_plan_dft_r2c_1d(numSamples, in, out, FFTW_ESTIMATE);
    fftwf_plan planInv = fftwf_plan_dft_c2r_1d(numSamples, out, processed, FFTW_ESTIMATE);

    fftwf_execute(planFwd);

    for (int i = 0; i < numSamples / 2 + 1; ++i) {
        float freq = (i * sampleRate) / numSamples;

        float gain = 1.0f;
        if (freq > 100 && freq < 200)
            gain = 0.5f;  // cut
        else if (freq > 1000 && freq < 3000)
            gain = 2.0f;  // boost

        out[i][0] *= gain;  // real part
        out[i][1] *= gain;  // imag part
    }

    fftwf_execute(planInv);

    for (int i = 0; i < numSamples; ++i)
        output[i] = processed[i] / numSamples;

    fftwf_destroy_plan(planFwd);
    fftwf_destroy_plan(planInv);

    fftwf_free(in);
    fftwf_free(out);
    fftwf_free(processed);

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

