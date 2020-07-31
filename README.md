# Yet Another Chip8 Emulator: QChip8

***Another Chip8 emulator with a better performance.***

## What's included
- `QChip8`: Executable for the emulator using Qt.
   - `CChip8`: Core chip8 emulator for calculation only.
- `CChip8Tests`: Tests for `CChip8` using QtTest.

## How to build

### Install Qt

Both `Qt 5.12` and `At 5.15` are tested. See https://wiki.qt.io/Main 's **Install / Build** part, and make sure CMake can detect it. You may want to modify `CMAKE_PREFIX_PATH`.


### Compile

```bash
$ git clone https://github.com/xiaoyu2006/YACE.git
$ cd YACE
# If CMake doesn't work.
$ export CMAKE_PREFIX_PATH=< where are your Qt5Config.cmake and qt5-config.cmake >
$ cmake .
```

To build
```bash
$ make
```

To test
```bash
$ make test
```

## TODOs
- [ ] Add sound
- [ ] Better error handling.
- [ ] Document for `CChip8`
- [ ] ***Beep!***
