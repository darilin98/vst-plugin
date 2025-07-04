//
// Created by Darek Rudiš on 25.04.2025.
//
#include "equalizer.hpp"

void PolynomialEqualizer::modulate(fftwf_complex *freq_bins, int fft_size, int sample_rate)
{
    constexpr float minFreq = EQ::minFreq;
    const float maxFreq = sample_rate * 0.5f; // Nyquist frequency
    constexpr float dBMaxBoost = EQ::dBMaxBoost;

    float centerFreq = expf( logf(minFreq) + param_shift_ * (logf(maxFreq) - logf(minFreq)) ); // Knob has a logarithmic feel -> converting back to linear
    float flip = 1.0f - 2.0f * param_direction_;
    float dBpeakGain = flip * param_intensity_ * dBMaxBoost;

    for (int i = 0; i < fft_size / 2 + 1; ++i) {
        float freq = (i * sample_rate) / fft_size;

        // Normalize curve so that high/low frequencies get treated equally
        float x = (freq - centerFreq) / (centerFreq * param_width_);

        //float shape = 1.0f - x*x;
        float shape = -powf(x, 4.0f) + powf(x, 3.0f) + powf(x, 2.0f) + 0.5f;
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

    auto tryUpdate = [&](CustomParamID id, float& param) {
        if (getParameterValue(data, id, temp_val)) {
            param = static_cast<float>(temp_val);
        }
    };

    tryUpdate(kParamShift,param_shift_);
    tryUpdate(kParamIntensity,param_intensity_);
    tryUpdate(kParamDirection,param_direction_);
    tryUpdate(kParamWidth,param_width_);

}

