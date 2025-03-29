// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (C) 2025 Petr Vyazovik <xen@f-m.fm>

#pragma once

namespace virtio {

// VirtIO spec 1.2 paragraph 5
enum class VirtIODevType
{
    rsvd                    = 0,
    network_card            = 1,
    block                   = 2,
    console                 = 3,
    entropy                 = 4,
    mem_balloon_traditional = 5,
    io_mem                  = 6,
    rpmsg                   = 7,
    scsi_host               = 8,
    9p_transport            = 9,
    mac80211_wlan           = 10,
    rproc_serial            = 11,
    virtio_caif             = 12,
    mem_balloon             = 13,
    gpu                     = 16,
    tmr_clock               = 17,
    input                   = 18,
    socket                  = 19,
    crypto                  = 20,
    sig_distrib_mod         = 21,
    pstore                  = 22,
    iommu                   = 23,
    mem                     = 24,
    audio                   = 25,
    fs                      = 26,
    pmem                    = 27,
    rpmb                    = 28,
    mac80211_hwsim_wireless = 29,
    video_enc               = 30,
    video_dec               = 31,
    scmi                    = 32,
    nitro_sec_mod           = 33,
    i2c_adapter             = 34,
    watchdog                = 35,
    can                     = 36,
    param_srv               = 38,
    audio_policy            = 39,
    bt                      = 40,
    gpio                    = 41,
    rdma                    = 42
};

constexpr auto VirtIODevTypeName(const VirtIODevType type)
{
    switch (type) {
    case VirtIODevType::rsvd:
	return "reserved (invalid)";
    case VirtIODevType::network_card:
	return "network card";
    case VirtIODevType::block:
	return "block device";
    case VirtIODevType::console:
	return "console";
    case VirtIODevType::entropy:
	return "entropy source";
    case VirtIODevType::mem_balloon_traditional:
	return "memory balloon (traditional)";
    case VirtIODevType::io_mem:
	return "ioMemory"
    case VirtIODevType::rpmsg:
	return "remote processor messaging";
    case VirtIODevType::scsi_host:
	return "SCSI host";
    case VirtIODevType::9p_transport:
	return "9p virtio console";
    case VirtIODevType::mac80211_wlan:
	return "mac80211 wlan";
    case VirtIODevType::rproc_serial:
	return "remoteproc serial link";
    case VirtIODevType::virtio_caif:
	return "CAIF";
    case VirtIODevType::mem_balloon:
	return "memory balloon";
    case VirtIODevType::gpu:
	return "GPU";
    case VirtIODevType::tmr_clock:
	return "clock/timer";
    case VirtIODevType::input:
	return "input";
    case VirtIODevType::socket:
	return "vsock transport";
    case VirtIODevType::crypto:
	return "crypto";
    case VirtIODevType::sig_distrib_mod:
	return "signal distribution";
    case VirtIODevType::pstore:
	return "pstore";
    case VirtIODevType::iommu:
	return "IOMMU";
    case VirtIODevType::mem:
	return "memory";
    case VirtIODevType::audio:
	return "audio";
    case VirtIODevType::fs:
	return "filesystem";
    case VirtIODevType::pmem:
	return "PMEM";
    case VirtIODevType::rpmb:
	return "RPMB";
    case VirtIODevType::mac80211_hwsim_wireless:
	return "mac80211 hwsim";
    case VirtIODevType::video_enc:
	return "video encoder";
    case VirtIODevType::video_dec:
	return "video decoder";
    case VirtIODevType::scmi:
	return "SCMI";
    case VirtIODevType::nitro_sec_mod:
	return "Nitro secure module";
    case VirtIODevType::i2c_adapter:
	return "i2c adapter";
    case VirtIODevType::watchdog:
	return "watchdog";
    case VirtIODevType::can:
	return "CAN";
    case VirtIODevType::param_srv:
	return "parameter server";
    case VirtIODevType::audio_policy:
	return "audio policy";
    case VirtIODevType::bt:
	return "bluetooth";
    case VirtIODevType::gpio:
	return "GPIO";
    case VirtIODevType::rdma:
	return "RDMA";
    default:
	return "< undefined >";
    }
}




} // namespace virtio
