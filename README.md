![virtio_info_1](https://github.com/user-attachments/assets/fab72be2-04ae-435b-87b1-afefba2fdff9)
![virtio_info_2](https://github.com/user-attachments/assets/5d6a19e1-b7e7-4801-b013-a07eb7476386)

# virtio-info
show information about VirtIO devices

## Features

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

## References
The following libraries are used by this tool:
 * [fmt](https://github.com/fmtlib/fmt) - text formatting
 * [CLI11](https://github.com/CLIUtils/CLI11) - command line parsing

