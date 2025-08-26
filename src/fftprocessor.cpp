//
// Created by Darek Rudiš on 11.04.2025.
//
#include "fftprocessor.hpp"
#include <ranges>
#include <utility>

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

void FFTProcessor::setEqualizer(std::shared_ptr<Equalizer> equalizer) {
    equalizer_ = std::move(equalizer);
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

void FFTProcessor::reset()
{
    std::ranges::fill(overlap_add_buffer_, 0.0f);
    std::ranges::fill(input_buffer_, 0.0f);
    write_pos_ = 0;
}


void FFTProcessor::process(float *input, float *output, float sample_rate, Steinberg::int32 num_samples)
{
    if (!in_ || !out_ || !processed_ || !plan_fwd_ || !plan_inv_)
        return;

    int samples_written = 0;

    while (samples_written < num_samples) {
        int new_samples = std::min<int>(num_samples - samples_written, hop_size_);

        std::memmove(input_buffer_.data(),
                     input_buffer_.data() + hop_size_,
                     sizeof(float) * (fft_size_ - hop_size_));

        std::copy(input + samples_written,
                  input + samples_written + new_samples,
                  input_buffer_.data() + (fft_size_ - hop_size_));

        for (int i = 0; i < fft_size_; ++i)
            in_[i] = input_buffer_[i] * window_[i];

        fftwf_execute(plan_fwd_);

        if (equalizer_) {
            equalizer_->modulate(out_, fft_size_, static_cast<int>(sample_rate));
        }

        fftwf_execute(plan_inv_);

        for (int i = 0; i < fft_size_; ++i) {
            float sample = (processed_[i] / fft_size_);
            overlap_add_buffer_[(write_pos_ + i) % overlap_add_buffer_.size()] += sample;
        }

        for (int i = 0; i < hop_size_; ++i) {
            output[samples_written + i] =
              overlap_add_buffer_[(write_pos_ + i) % overlap_add_buffer_.size()];
            overlap_add_buffer_[(write_pos_ + i) % overlap_add_buffer_.size()] = 0.f;
        }

        write_pos_ = (write_pos_ + hop_size_) % overlap_add_buffer_.size();
        samples_written += hop_size_;
    }
}
