//
// Created by Darek Rudiš on 25.04.2025.
//
#include "equalizer.hpp"

void PolynomialEqualizer::modulate(fftwf_complex *freq_bins, int fft_size, int sample_rate)
{
    for (int i = 0; i < fft_size / 2 + 1; ++i) {
        float freq = (i * sample_rate) / fft_size;

        float gain = 1.0f;
        float cut_center = 600.0f, cut_width = 300.0f;
        gain *= 1.0f - 0.9f * expf(-powf((freq - cut_center) / cut_width, 2.0f));

        float boost_center = 4500.0f, boost_width = 800.0f;
        gain *= 1.0f + 5.0f * expf(-powf((freq - boost_center) / boost_width, 2.0f));

        freq_bins[i][0] *= gain;  // real part
        freq_bins[i][1] *= gain;  // imag part
    }
}
