# The documentation of Polynomial Equalizer

Welcome to the full documentation of Polynomial Equalizer. 

Polynomial Equalizer is a VST plugin capable of being used in any VST3-supporting DAW.

---

## Feature list

- **Full control over many parameters**
  - Change the intensity or range to your liking.
- **EQ Curve visualization**
  - Live visuals that paint a clear picture on which frequencies are being affected.
- **Support for any amount of audio channels**
  - The plugin applies the EQ to all channels equally and does not mix them.
- **Live audio EQ**
  - The plugin works smoothly in real time.

## How it works

Polynomial Equalizer works thanks to an FFT processing pipeline.

For each channel:

- The live audio is deconstructed into frequency data.
- A frequency-domain transformation defined in the Equalizer is applied to it.
- The transformed result is then reconstructed and outputted.

Visit [this page](architecture.md) for a more detailed description + a full project diagram.

## External dependencies

- [`VST3SDK`](https://github.com/steinbergmedia/vst3sdk)
- [`FFTW`](https://fftw.org/)

## System Requirements

- **Windows**: 64-bit Windows 10 or later
  - With [`MSVC`](https://visualstudio.microsoft.com/vs/features/cplusplus/) installed for compiling purposes.
  - And the [`MSVC Runtime`](https://learn.microsoft.com/en-us/cpp/windows/latest-supported-vc-redist?view=msvc-170) for building and linking.
- **macOS**: Apple Silicon (ARM64) macOS 11 or later
- 64-bit DAW with VST3 support

## Build process

For a detailed guide on how to build the plugin [visit here.](build_guide.md)