// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (C) 2025 Petr Vyazovik <xen@f-m.fm>

#pragma once

#include <string_view>

namespace virtio {

constexpr std::string_view virtio_devs_path {"/sys/bus/virtio/devices"};

} //namespace virtio
