![virtio_info_screen](https://github.com/user-attachments/assets/ec4858ff-c961-42b8-894b-e43cf8a0eb98)
![virtio_info_diff_mode png](https://github.com/user-attachments/assets/3449faad-f6b2-4b0b-863d-a46e56b08ced)


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
  -v,        --version                  Print version and exit 
  -i,        --info < existing device name (e.g. virtio0) > 
                                        show detailed info about specific VirtIO device 
             --no-desc                  don't show features bits description 
             --feat-set                 display only the feature bits that have been set 
             --no-status                don't show device status bits decoding 
  -l,        --list                     show registered VirtIO devices 
  -d,        --diff <device A> <device B> 
                                        highlight features difference for two devices A and B 
```

## References
The following libraries are used by this tool:
 * [fmt](https://github.com/fmtlib/fmt) - text formatting
 * [CLI11](https://github.com/CLIUtils/CLI11) - command line parsing
 * [ftxui](https://github.com/ArthurSonzogni/FTXUI) - visual representation
 * [magic_enum](https://github.com/Neargye/magic_enum) - reflection for enums
