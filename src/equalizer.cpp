//
// Created by Darek Rudiš on 25.04.2025.
//
#include "equalizer.hpp"

void PolynomialEqualizer::modulate(fftwf_complex *freq_bins, int fft_size, int sample_rate)
{
    const float minFreq = 20.f;
    const float maxFreq = sample_rate * 0.5f;

    float centerFreq = expf( logf(minFreq) + param_shift_ * (logf(maxFreq) - logf(minFreq)) );
    float flip = 1.0f - 2.0f * param_direction_;
    float dBpeakGain = flip * param_intensity_ * 12.0f; //12 dB max boost

    for (int i = 0; i < fft_size / 2 + 1; ++i) {
        float freq = (i * sample_rate) / fft_size;

        // Normalize curve so that values way beyond the target range are left intact
        float x = (freq - centerFreq) / (centerFreq * param_width_);

        float shape = 1.0f - x*x;
        if (shape < 0.0f) shape = 0.0f;

        float dBgain = dBpeakGain * shape;

        float gain = powf(10.0f, dBgain * 0.05f);

        freq_bins[i][0] *= gain;  // real part
        freq_bins[i][1] *= gain;  // imag part
    }
}
void PolynomialEqualizer::update_params(ProcessData& data)
{
    ParamValue temp_val;
    if (getParameterValue(data, kParamShift, temp_val))
        param_shift_ = static_cast<float>(temp_val);
    if (getParameterValue(data, kParamIntensity, temp_val))
        param_intensity_ = static_cast<float>(temp_val);
    if (getParameterValue(data, kParamDirection, temp_val))
        param_direction_ = static_cast<float>(temp_val);
    if (getParameterValue(data, kParamWidth, temp_val))
        param_width_ = static_cast<float>(temp_val);
}

