// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (C) 2025 Petr Vyazovik <xen@f-m.fm>

#pragma once

#include "config.h"
#include "virtio_bus.h"

namespace ui {

void ListVirtIODevices();
void VirtIODevDetailedInfo();
void VirtIODevFeaturesDiff();
void ListVirtIODevTypes();
void VirtIODevRawFeaturesInfo();

} // namespace ui
