//
// Created by Darek Rudiš on 11.04.2025.
//
#include "fftprocessor.hpp"

FFTProcessor::~FFTProcessor()
{
    fftwf_destroy_plan(plan_fwd_);
    fftwf_destroy_plan(plan_inv_);
    fftwf_free(in_);
    fftwf_free(out_);
    fftwf_free(processed_);
}

void FFTProcessor::prepare(int32_t fft_size)
{
    fft_size_ = fft_size;
    in_ = (float*)fftwf_malloc(sizeof(float) * fft_size_);
    out_ = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * (fft_size_ /2 + 1));
    processed_ = (float*)fftwf_malloc(sizeof(float) * fft_size_);

    plan_fwd_ = fftwf_plan_dft_r2c_1d(fft_size_, in_, out_, FFTW_ESTIMATE);
    plan_inv_ = fftwf_plan_dft_c2r_1d(fft_size_, out_, processed_, FFTW_ESTIMATE);
}

void FFTProcessor::process(float *input, float *output, float sample_rate, Steinberg::int32 num_samples)
{
    std::copy(input, input + num_samples, in_);

    fftwf_execute(plan_fwd_);

    for (int i = 0; i < num_samples / 2 + 1; ++i) {
        float freq = (i * sample_rate) / num_samples;

        float gain = 1.0f;
        if (freq > 100 && freq < 200)
            gain = 0.5f;  // cut
        else if (freq > 1000 && freq < 3000)
            gain = 1.5f;  // boost

        out_[i][0] *= gain;  // real part
        out_[i][1] *= gain;  // imag part
    }
    fftwf_execute(plan_inv_);

    for (int i = 0; i < fft_size_; ++i)
        output[i] = processed_[i] / num_samples;
}
