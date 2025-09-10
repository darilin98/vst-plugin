# High-level project design overview

As mentioned before, the Polynomial Equalizer project is a `VST3` plugin.

This means that it needs to respect the design pattern provided by the `VST3SDK`.

---

*The plugin itself is divided into 2 core parts:*

## PluginProcessor

Manages the actual audio processing.

Is called by the host to perform actions on the provided samples.

### FFTProcessor

Transforms time-domain samples to frequency-domain data. 

Delegates the data to an `Equalizer` which transforms it by its own definition.

Reconstructs modified audio data.

---

## PluginController

Keeps track of parameters and states.

Acts as a bridge between the UI and the audio pipeline.

Registers the main `VSTGUI` view.

### VSTGUI

This GUI framework provided by the SDK constructs a view defined in a `.uidesc` file.

It also intializes custom views with a defined factory class such as `EQCurveView` which displays a live function graph.

---

## Further possible improvements

The bypass handling could be implemented in a more sturdy way to handle frequent on/off switching without unwanted audio fragmentation.

In the current state the EQ allows for audio mangling for example when the user sets the intensity too high. This is a bit controversial in my opinion. 
The user probably expects some artifacting when they turn all knobs to the limit. However, some kind of *safe mode* would be an interesting toggle to implement.