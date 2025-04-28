//
// Created by Darek Rudiš on 25.04.2025.
//
#include "equalizer.hpp"

void PolynomialEqualizer::modulate(fftw_complex *freq_bins, int fft_size, int sample_rate)
{
    for (int i = 0; i < fft_size / 2 + 1; ++i) {
        float freq = (i * sample_rate) / fft_size;

        int gain = 1.0f;

        freq_bins[i][0] *= gain;  // real part
        freq_bins[i][1] *= gain;  // imag part
    }
}
