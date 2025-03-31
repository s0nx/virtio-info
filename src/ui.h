// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (C) 2025 Petr Vyazovik <xen@f-m.fm>

#pragma once

#include "virtio_bus.h"

namespace ui {

void ListVirtIODevices();

void VirtIODevDetailedInfo(const std::string &dev_name);

} // namespace ui
