# YACE: Yet Another Chip8 Emulator

## Components

- `QChip8`: Executable for the emulator using Qt.
- `CChip8`: Core chip8 emulator for calculation only. Provided as a dynamic library.
- `CPUTests`: Tests for `CChip8` using QtTest.

## Building & Installing

Works under Qt6.

```bash
$ git clone https://github.com/xiaoyu2006/YACE.git
$ cd YACE
$ mkdir build && cd build
$ cmake ..
```

To build
```bash
$ make
```

To test
```bash
$ make test
```

## Gameplay

### ROMs

Visit https://github.com/dmatlack/chip8/tree/master/roms to get some ROMs. Not every ROM is tested.

### Keys

`QChip8` maps the following keys for *KeyIndex 0 to 15* in `CChip8`.

```
1234
qwer
asdf
zxcv
```

## TODOs
- [ ] Add sound
- [ ] Better error handling.
- [ ] Document for `CChip8`
- [ ] C++-ify
