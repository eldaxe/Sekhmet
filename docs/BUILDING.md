# Building Sekhmet for Android

## Reproducible framework version

Sekhmet uses PortableKit as a git submodule, pinned initially to the same commit used by the DyausPita Android APK reference build:

`986119616a86104ce7eac557351726c19ee13cd9`

## Android APK

The GitHub Actions workflow `.github/workflows/android-apk.yml` follows DyausPita's known APK pipeline:

- Ubuntu 24.04
- Java 17
- Android SDK platform 35 and build-tools 35.0.0
- Android NDK 27.2.12479018
- Android API 29, ABI arm64-v8a
- SDL 3.2.16
- Vulkan renderer and bundled FFmpeg
- PortableKit's `packaging/android/build_apk.sh`

Run it from Actions → Android APK → Run workflow. The APK is a debug bootstrap build. It contains no game dump, extracted game assets, or generated AOT code.

A successful build proves native compilation and APK packaging only. It does not prove that God Eater Burst boots or that all HLE functions are implemented.

## Local build

Follow the workflow's CMake configuration, then package:

```sh
APP_ID=com.eldaxe.sekhmet \
GAME_RES="$PWD/android-res" \
DEBUGGABLE=1 \
portablekit/packaging/android/build_apk.sh \
  out/android .deps/SDL .deps/sdl-install/lib/libSDL3.so \
  out/Sekhmet-arm64-debug.apk
```

Do not commit game dumps, EBOOT files, extracted assets, generated AOT code, save data, or signing keys.
