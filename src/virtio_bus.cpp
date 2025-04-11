// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (C) 2025 Petr Vyazovik <xen@f-m.fm>

#include "virtio_bus.h"

#include <algorithm>
#include <bitset>
#include <fstream>
#include <fmt/core.h>

namespace virtio {

namespace fs = std::filesystem;

// output format is defined in drivers/virtio/virtio.c
constexpr uint32_t virtio_dev_id_buf_len {6};
constexpr uint32_t virtio_dev_status_buf_len {10};
constexpr uint32_t virtio_dev_features_buf_len {64};

static VirtIODevType
DevGetType(const fs::path &vd_dev_path)
{
    std::string tmp_buf;

    // read device id
    auto path = vd_dev_path / "device";
    std::ifstream vdev_id_file {path.c_str(), std::ios::in};
    if (!vdev_id_file.is_open()) {
        fmt::print("Failed to obtain device type for {}\n", vd_dev_path.c_str());
        throw std::runtime_error("Failed to process VirtIO device");
    }

    std::getline(vdev_id_file, tmp_buf);
    if (tmp_buf.length() != virtio_dev_id_buf_len) {
        fmt::print("Failed to read device type for {}\n", vd_dev_path.c_str());
        throw std::runtime_error("Failed to process VirtIO device");
    }

    uint32_t type;
    auto res = std::sscanf(tmp_buf.c_str(), "0x%04x", &type);
    if (res != 1) {
        fmt::print("Failed to parse device type for {}\n", vd_dev_path.c_str());
        throw std::runtime_error("Failed to process VirtIO device");
    }

    if (type > e_to_type(VirtIODevType::dev_type_max)) {
        fmt::print("Parsed device type (%u) for {} > max defined\n",
                   vd_dev_path.c_str());
    }

    return VirtIODevType {type};
}

static uint32_t
DevGetStatus(const fs::path &vd_dev_path)
{
    std::string tmp_buf;

    // read device status
    auto path = vd_dev_path / "status";
    std::ifstream vdev_status_file {path.c_str(), std::ios::in};
    if (!vdev_status_file.is_open()) {
        fmt::print("Failed to obtain device status for {}\n", vd_dev_path.c_str());
        throw std::runtime_error("Failed to process VirtIO device");
    }

    std::getline(vdev_status_file, tmp_buf);
    if (tmp_buf.length() != virtio_dev_status_buf_len) {
        fmt::print("Failed to read device status for {}\n", vd_dev_path.c_str());
        throw std::runtime_error("Failed to process VirtIO device");
    }

    uint32_t status;
    auto res = std::sscanf(tmp_buf.c_str(), "0x%08x", &status);
    if (res != 1) {
        fmt::print("Failed to parse device status for {}\n", vd_dev_path.c_str());
        throw std::runtime_error("Failed to process VirtIO device");
    }

    return status;
}

static uint64_t
DevGetFeatures(const fs::path &vd_dev_path)
{
    std::string tmp_buf;

    // read device features
    auto path = vd_dev_path / "features";
    std::ifstream vdev_features_file {path.c_str(), std::ios::in};
    if (!vdev_features_file.is_open()) {
        fmt::print("Failed to obtain device features for {}\n", vd_dev_path.c_str());
        throw std::runtime_error("Failed to process VirtIO device");
    }

    std::getline(vdev_features_file, tmp_buf);
    if (tmp_buf.length() != virtio_dev_features_buf_len) {
        fmt::print("Failed to read device features for {}\n", vd_dev_path.c_str());
        throw std::runtime_error("Failed to process VirtIO device");
    }

    // see drivers/virtio/virtio.c: features_show()
    std::ranges::reverse(tmp_buf);

    std::bitset<virtio_dev_features_buf_len> feat_bset(tmp_buf);
    return feat_bset.to_ulong();
}

static std::string
DevAuxInfoNotImpl([[maybe_unused]] const fs::path &dev_path)
{
    return {};
}

static std::string
NetdevGetAuxInfo(const fs::path &dev_path)
{
    // return iface name
    auto path = dev_path / "net";
    if (!fs::directory_entry(path).exists()) {
        fmt::print("{} doesn't exist for network device\n", path.c_str());
        return {};
    }

    std::string iface_name{};

    // single entry expected in net/
    for (const auto &iface_entry : fs::directory_iterator {path}) {
        iface_name = iface_entry.path().filename().string();
    }

    return iface_name;
}

static std::string
BlockdevGetAuxInfo(const fs::path &dev_path)
{
    // return block dev full name
    auto path = dev_path / "block";
    if (!fs::directory_entry(path).exists()) {
        fmt::print("{} doesn't exist for block device\n", path.c_str());
        return {};
    }

    std::string block_dev_name{"/dev/"};

    // single entry expected in block/
    for (const auto &block_dev_entry : fs::directory_iterator {path}) {
        block_dev_name += block_dev_entry.path().filename().string();
    }

    return block_dev_name;
}

// Return some information about device based on the type
static std::string
DevGetAuxInfo(const VirtIODevType dev_type, const fs::path &dev_path)
{
    switch (dev_type) {
    case VirtIODevType::network_card:
        return NetdevGetAuxInfo(dev_path);
    case VirtIODevType::block:
        return BlockdevGetAuxInfo(dev_path);
    default:
        return DevAuxInfoNotImpl(dev_path);
    }
}

VirtIODevDesc
CreateDevDesc(const fs::path &dev_path)
{
    auto device_type = DevGetType(dev_path);
    auto device_status = DevGetStatus(dev_path);
    auto device_features = DevGetFeatures(dev_path);
    auto aux_info = DevGetAuxInfo(device_type, dev_path);

    return {device_type, device_status, device_features,
            aux_info, dev_path};
}

static virtio_devs_ct
GetDevDescs(const fs::path &vd_path)
{
    virtio_devs_ct devs;

    for (const auto &bus_entry : fs::directory_iterator {vd_path}) {
        if (!fs::is_symlink(bus_entry)) {
            fmt::print("VirtIO bus entry is not a symlink\n");
            return {};
        }

        auto res = devs.insert({bus_entry.path().filename().string(),
                                CreateDevDesc(bus_entry.path())});
        if (!res.second) {
            fmt::print("Failed to insert VirtIO dev entry into map\n");
            throw std::runtime_error("Failed to populate VirtIO devices tree\n");
        }
    }

    return devs;
}

virtio_devs_ct GetVirtioDevMap()
{
    fs::path virtio_path {virtio_devs_path};
    return GetDevDescs(virtio_path);
}

} // namespace virtio
