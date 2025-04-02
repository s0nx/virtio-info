![virtio_info_screen](https://github.com/user-attachments/assets/ec4858ff-c961-42b8-894b-e43cf8a0eb98)

# virtio-info
show information about VirtIO devices

## Features
* listing registered VirtIO devices
* status/features bits decoding (currently for network devices only)

## Requirements
 * compiler supporting `C++20`
 * `cmake`
 * [fmt](https://github.com/fmtlib/fmt) library

## Installation
### fmt packages
 * Ubuntu 24.04: `libfmt-dev`,`libfmt9`
 * Arch Linux: `fmt`
 * Fedora 40: `fmt`,`fmt-devel`

### Building
```
git clone https://github.com/s0nx/virtio-info.git
cd virtio-info && mkdir build
cmake -B build -S .
make -C build -j
```

## Usage
`./virtio-info -l` - list all registered devices and their types  
`./virtio-info -i <virtio device name>` - show detailed info about the status/features of a specific device

## References
The following libraries are used by this tool:
 * [fmt](https://github.com/fmtlib/fmt) - text formatting
 * [CLI11](https://github.com/CLIUtils/CLI11) - command line parsing
 * [ftxui](https://github.com/ArthurSonzogni/FTXUI) - visual representation
 * [magic_enum](https://github.com/Neargye/magic_enum) - reflection for enums

