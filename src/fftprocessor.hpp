//
// Created by Darek Rudiš on 11.04.2025.
//
#pragma once
#ifndef FFTPROCESSOR_HPP
#define FFTPROCESSOR_HPP

#include <fftw3.h>
#include <algorithm>
#include <vector>
#include "base/ftypes.h"
#include "equalizer.hpp"

using array_t = std::vector<float>;

/**
 * @brief Manages an FFT pipeline.
 *
 * Decomposes, delegates signal processing and recomposes processed audio samples.
 * @warning A separate FFTProcessor should be used for each channel.
 */
class FFTProcessor {
public:
    FFTProcessor();
    ~FFTProcessor();
    /**
     * @brief Sets up buffers and other constants based on the given FFT size.
     * @warning Must be used before the @ref process method is being called in order for buffers to have proper states.
     * @param fft_size Specified FFT size planned to be used.
     */
    void prepare(int32_t fft_size);
    /**
     * @brief Flushes internal buffers.
     */
    void reset();
    /**
     * @brief Main signal processing pipeline method.
     *
     * @param input Input samples array.
     * @param output Output array filled upon method completion.
     * @param sample_rate Plugin host's set sample rate.
     * @param num_samples Size of the input array.
     */
    void process(float* input, float* output, float sample_rate, Steinberg::int32 num_samples);
    /**
     * @brief Injects any object derived from the @ref Equalizer
     *
     * @see Equalizer
     * @param equalizer Injected equalizer that will be used in the processing step.
     */
    void setEqualizer(std::shared_ptr<Equalizer> equalizer);
private:
    int fft_size_; ///< Should be 2^n
    int hop_size_;
    int write_pos_;
    array_t input_buffer_;
    array_t overlap_add_buffer_; ///< Buffer for FFT window overlaps.
    float* in_;
    float* processed_;
    array_t window_; ///< Window function that changes perceived intensity of samples.
    fftwf_complex* out_;
    fftwf_plan plan_fwd_;
    fftwf_plan plan_inv_;
    std::shared_ptr<Equalizer> equalizer_; ///< Any Equalizer, used during process method.
};
#endif //FFTPROCESSOR_HPP
