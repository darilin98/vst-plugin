//
// Created by Darek Rudiš on 25.04.2025.
//
#include "fftw3.h"
#include "base/ftypes.h"

#ifndef EQUALIZER_HPP
#define EQUALIZER_HPP

class Equalizer {
public:
    virtual ~Equalizer() = default;

    virtual void modulate(fftw_complex* freq_bins, int fft_size, int sample_size) = 0;

};

class PolynomialEqualizer : public Equalizer {
public:
    void modulate(fftw_complex* freq_bins, int fft_size, int sample_rate) override;
};
#endif //EQUALIZER_HPP
