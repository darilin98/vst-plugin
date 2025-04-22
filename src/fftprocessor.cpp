//
// Created by Darek Rudiš on 11.04.2025.
//
#include "fftprocessor.hpp"

FFTProcessor::FFTProcessor()
: fft_size_(0)
, hop_size_(0)
, write_pos_(0)
, in_(nullptr)
, processed_(nullptr)
, out_(nullptr)
, plan_fwd_(nullptr)
, plan_inv_(nullptr)
{}

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
    hop_size_ = fft_size_ / 2;
    input_buffer_.assign(fft_size_, 0.0f);

    in_ = (float*)fftwf_malloc(sizeof(float) * fft_size_);
    out_ = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * (fft_size_ /2 + 1));
    processed_ = (float*)fftwf_malloc(sizeof(float) * fft_size_);

    plan_fwd_ = fftwf_plan_dft_r2c_1d(fft_size_, in_, out_, FFTW_MEASURE);
    plan_inv_ = fftwf_plan_dft_c2r_1d(fft_size_, out_, processed_, FFTW_MEASURE);

    // Hann window
    window_.resize(fft_size_);
    for (int i = 0; i < fft_size_; ++i)
        window_[i] = 0.5f * (1.0f - cosf((2.0f * M_PI * i) / (fft_size_ - 1.0f)));
    overlap_add_buffer_.resize(fft_size_ + hop_size_, 0.0f);
    write_pos_ = 0;
}

void FFTProcessor::process(float *input, float *output, float sample_rate, Steinberg::int32 num_samples)
{
    if (!in_ || !out_ || !processed_ || !plan_fwd_ || !plan_inv_)
        return;

    const int safe_samples = std::min(num_samples, fft_size_);

    std::fill(in_, in_ + fft_size_, 0.0f);

    std::copy(input, input + safe_samples, in_);

    for (int i = 0; i < safe_samples; ++i)
        in_[i] *= window_[i];

    fftwf_execute(plan_fwd_);

    for (int i = 0; i < fft_size_ / 2 + 1; ++i) {
        float freq = (i * sample_rate) / fft_size_;

        float gain = 1.0f;

        float cut_center = 600.0f;
        float cut_width = 300.0f;
        gain *= 1.0f - 0.3f * expf(-powf((freq - cut_center) / cut_width, 2.0f));

        float boost_center = 4500.0f;
        float boost_width = 800.0f;
        gain *= 1.0f + 0.2f * expf(-powf((freq - boost_center) / boost_width, 2.0f));

        out_[i][0] *= gain;  // real part
        out_[i][1] *= gain;  // imag part
    }
    fftwf_execute(plan_inv_);

    for (int i = 0; i < safe_samples; ++i)
        output[i] = processed_[i] / fft_size_;
}
