![virtio_info_3](https://github.com/user-attachments/assets/5ac53202-9b58-4b5b-8187-475f5d9895e9)

# virtio-info
This simple tool allows to quickly identify all VirtIO devices currently registered.  
For a given device, features bits decoding makes it easy to understand which particular features have been negotiated between device and driver.  
__diff__ mode highlights features differences for two devices of the same type.  

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
```
virtio-info [OPTIONS]

OPTIONS:
  -h,        --help                     Print this help message and exit 
             --no-desc                  don't show features bits description 
             --feat-set                 display only the feature bits that have been set 
  -v,        --version                  Print version and exit 
  -i,        --info < existing device name (e.g. virtio0) > 
                                        show detailed info about specific VirtIO device 
             --no-status                don't show device status bits decoding 
  -l,        --list                     show registered VirtIO devices 
  -d,        --diff <device A> <device B> 
                                        highlight features difference for two devices A and B 
  -t,        --types                    show defined VirtIO device types 
  -f,        --feat <device type> <features (non-negative)> 
                                        decode given features for a particular device type 
```

## References
The following libraries are used by this tool:
 * [fmt](https://github.com/fmtlib/fmt) - text formatting
 * [CLI11](https://github.com/CLIUtils/CLI11) - command line parsing
 * [ftxui](https://github.com/ArthurSonzogni/FTXUI) - visual representation
 * [magic_enum](https://github.com/Neargye/magic_enum) - reflection for enums
