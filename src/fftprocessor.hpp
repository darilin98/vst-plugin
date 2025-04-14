//
// Created by Darek Rudiš on 11.04.2025.
//
#pragma once
#include <fftw3.h>
#include <algorithm>
#include "base/ftypes.h"

#ifndef FFTPROCESSOR_HPP
#define FFTPROCESSOR_HPP

class FFTProcessor {
public:
    FFTProcessor();
    ~FFTProcessor();
    void prepare(int32_t fft_size);
    void process(float* input, float* output, float sample_rate, Steinberg::int32 num_samples);
private:
    int fft_size_; ///< Should be 2^n
    float* in_;
    float* processed_;
    fftwf_complex* out_;
    fftwf_plan plan_fwd_;
    fftwf_plan plan_inv_;
};
#endif //FFTPROCESSOR_HPP
