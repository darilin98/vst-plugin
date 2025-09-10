# Plugin build guide

The build process is simple but the prerequisites differ based on your OS.

It is recommended to build the project in an IDE such as `CLion` or `Visual Studio Community` in order to have more control over the build options.

Make sure you are building the plugin in **Release** mode.

Follow the step-by-step guide for your platform.

---

## Windows

Select the `MSVC` toolchain for the build. (Other compilers run into issues when building the VSTGUI library).

Make sure you target the `amd64` (or `x86_64`) platform.  

Load the project from the root `CMakeLists.txt`

Now you should be able to build the target **polynomial-eq**. Simply select it and let the process complete.

The result should automatically get copied to a common VST location for your computer. (e.g. `C:\Program Files (x86)\Common Files\VST3`) If not - copy the *plugin folder* manually.

The *plugin folder* is located in the generated build folder under `VST3\Release`. Check that it has a `.vst3` suffix.

---

## macOS

Load the project from the root `CMakeLists.txt`

Now you should be able to build the target **polynomial-eq**. Simply select it and let the process complete.

The result should automatically get copied to a common VST location for your computer. (e.g. `Library/Audio/Plug-ins/VST3`) If not - copy the *plugin bundle* manually.

The *plugin bundle* is located in the generated build folder under `VST3/Release`. Check that it has a `.vst3` suffix.

---

When the plugin is correctly placed in its common location, your DAW of choice should detect it and prompt you to include it.
