// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (C) 2025 Petr Vyazovik <xen@f-m.fm>

#pragma once

#include "virtio_defs.h"

#include <cstdint>
#include <string_view>
#include <filesystem>
#include <map>

template <typename E>
constexpr auto e_to_type(E e) noexcept
{
    return static_cast<std::underlying_type_t<E>>(e);
}

namespace virtio {

struct VirtIODevDesc
{
    VirtIODevType               dev_type_;
    uint32_t                    status_;
    uint64_t                    features_;
    std::string                 aux_info_;
    const std::filesystem::path dev_path_;
};

constexpr std::string_view virtio_devs_path {"/sys/bus/virtio/devices"};

using virtio_devs_ct = std::map<std::string, VirtIODevDesc>;

virtio_devs_ct GetVirtioDevMap();
VirtIODevDesc CreateDevDesc(const std::filesystem::path &dev_path);

} //namespace virtio
