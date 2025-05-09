//
// Created by Darek Rudiš on 25.04.2025.
//
#include "equalizer.hpp"

void PolynomialEqualizer::modulate(fftwf_complex *freq_bins, int fft_size, int sample_rate)
{
    float centerFreq = 20000 * param_shift_;
    float dBpeakGain = 12;
    for (int i = 0; i < fft_size / 2 + 1; ++i) {
        float freq = (i * sample_rate) / fft_size;

        float x = (freq - centerFreq) / centerFreq;
        float dBgain = param_alpha_ * x * x + dBpeakGain;

        float gain = powf(10.0f, dBgain / 20.0f);

        freq_bins[i][0] *= gain;  // real part
        freq_bins[i][1] *= gain;  // imag part
    }
}
void PolynomialEqualizer::update_params(ProcessData& data)
{
    ParamValue temp_val;
    if (getParameterValue(data, kParamShift, temp_val))
        param_shift_ = static_cast<float>(temp_val);
    if (getParameterValue(data, kParamAlpha, temp_val))
        param_alpha_ = static_cast<float>(temp_val);
    if (getParameterValue(data, kParamBeta, temp_val))
        param_beta_ = static_cast<float>(temp_val);
}

