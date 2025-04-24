//
// Created by Darek Rudiš on 11.04.2025.
//
#pragma once
#include <fftw3.h>
#include <algorithm>
#include <vector>
#include <chrono>
#include "base/ftypes.h"

#ifndef FFTPROCESSOR_HPP
#define FFTPROCESSOR_HPP

using array_t = std::vector<float>;

class FFTProcessor {
public:
    FFTProcessor();
    ~FFTProcessor();
    void prepare(int32_t fft_size);
    void reset();
    void process(float* input, float* output, float sample_rate, Steinberg::int32 num_samples);
private:
    int fft_size_; ///< Should be 2^n
    int hop_size_;
    int write_pos_;
    array_t input_buffer_;
    array_t overlap_add_buffer_;
    float* in_;
    float* processed_;
    array_t window_;
    fftwf_complex* out_;
    fftwf_plan plan_fwd_;
    fftwf_plan plan_inv_;
};

#include <fstream>

inline void log_debug(const std::string& msg)
{
    auto now = std::chrono::system_clock::now();
    std::string formatted_time = std::format("{0:%F_%T}", now);
    std::ofstream log("/Users/darek/Documents/vst3_debug_log.txt", std::ios::app);
    log << formatted_time << ": "<< msg << std::endl;
}
#endif //FFTPROCESSOR_HPP
