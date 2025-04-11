// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (C) 2025 Petr Vyazovik <xen@f-m.fm>

#pragma once
#include <cstdint>
#include <string_view>

namespace virtio {

// VirtIO spec 1.2 paragraph 5
enum class VirtIODevType: unsigned
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
    transport_9p            = 9,
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
    rdma                    = 42,

    dev_type_max = rdma
};

constexpr std::string_view VirtIODevTypeName(const VirtIODevType type)
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
	return "ioMemory";
    case VirtIODevType::rpmsg:
	return "remote processor messaging";
    case VirtIODevType::scsi_host:
	return "SCSI host";
    case VirtIODevType::transport_9p:
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

enum class VirtIOStatusBits : uint32_t
{
    VIRTIO_CONFIG_S_ACKNOWLEDGE = 0,
    VIRTIO_CONFIG_S_DRIVER      = 1,
    VIRTIO_CONFIG_S_DRIVER_OK   = 2,
    VIRTIO_CONFIG_S_FEATURES_OK = 3,

    VIRTIO_CONFIG_S_NEEDS_RESET = 6,
    VIRTIO_CONFIG_S_FAILED      = 7
};

constexpr std::string_view VirtIOStatusBitDesc(const VirtIOStatusBits bit)
{
    switch (bit) {
    case VirtIOStatusBits::VIRTIO_CONFIG_S_ACKNOWLEDGE:
	return "device has been seen; processed generic fields";
    case VirtIOStatusBits::VIRTIO_CONFIG_S_DRIVER:
	return "driver has been found";
    case VirtIOStatusBits::VIRTIO_CONFIG_S_DRIVER_OK:
	return "driver has used its parts of the config";
    case VirtIOStatusBits::VIRTIO_CONFIG_S_FEATURES_OK:
	return "driver has finished configuring features";
    case VirtIOStatusBits::VIRTIO_CONFIG_S_NEEDS_RESET:
	return "device has to be reset by driver";
    case VirtIOStatusBits::VIRTIO_CONFIG_S_FAILED:
	return "critical configuration error";
    default:
	return "";
    }
}

constexpr uint32_t feature_bit_transport_start = 28;
constexpr uint32_t   feature_bit_transport_end = 42;

static inline bool FeatureBitIsTransport(uint32_t bit_pos)
{
    return bit_pos >= feature_bit_transport_start &&
	   bit_pos <= feature_bit_transport_end;
}

// Feature bits for network card virtio device.
// XXX: this enum contains not only network-specific features, but transport-specific
// bits as well. These transport-specific bits are 28 through 42.
// See include/uapi/linux/virtio_config.h in Linux sources
enum class VirtIONetFeature : uint32_t
{
    VIRTIO_NET_F_CSUM                = 0, // Host handles pkts w/ partial csum
    VIRTIO_NET_F_GUEST_CSUM 	     = 1, // Guest handles pkts w/ partial csum
    VIRTIO_NET_F_CTRL_GUEST_OFFLOADS = 2, // Dynamic offload configuration
    VIRTIO_NET_F_MTU                 = 3, // Initial MTU advice
    VIRTIO_NET_F_MAC                 = 5, // Host has given MAC address
    VIRTIO_NET_F_GSO                 = 6, // Host handles pkts w/ any GSO type. XXX: VIRTIO_CONFIG_NO_LEGACY
    VIRTIO_NET_F_GUEST_TSO4          = 7, // Guest can handle TSOv4 in.
    VIRTIO_NET_F_GUEST_TSO6          = 8, // Guest can handle TSOv6 in.
    VIRTIO_NET_F_GUEST_ECN           = 9, // Guest can handle TSO[6] w/ ECN in.
    VIRTIO_NET_F_GUEST_UFO           = 10, // Guest can handle UFO in.
    VIRTIO_NET_F_HOST_TSO4           = 11, // Host can handle TSOv4 in.
    VIRTIO_NET_F_HOST_TSO6           = 12, // Host can handle TSOv6 in.
    VIRTIO_NET_F_HOST_ECN            = 13, // Host can handle TSO[6] w/ ECN in.
    VIRTIO_NET_F_HOST_UFO            = 14, // Host can handle UFO in.
    VIRTIO_NET_F_MRG_RXBUF           = 15, // Host can merge receive buffers.
    VIRTIO_NET_F_STATUS              = 16, // virtio_net_config.status available
    VIRTIO_NET_F_CTRL_VQ             = 17, // Control channel available
    VIRTIO_NET_F_CTRL_RX             = 18, // Control channel RX mode support
    VIRTIO_NET_F_CTRL_VLAN           = 19, // Control channel VLAN filtering
    VIRTIO_NET_F_CTRL_RX_EXTRA       = 20, // Extra RX mode control support
    VIRTIO_NET_F_GUEST_ANNOUNCE      = 21, // Guest can announce device on the network
    VIRTIO_NET_F_MQ                  = 22, // Device supports automatic Receive Flow Steering
    VIRTIO_NET_F_CTRL_MAC_ADDR       = 23, // Set MAC address


    // XXX: VIRTIO_CONFIG_NO_LEGACY
    // Do we get callbacks when the ring is completely used, even if we've suppressed them?
    VIRTIO_F_NOTIFY_ON_EMPTY 	 = 24,

    // XXX: VIRTIO_CONFIG_NO_LEGACY
    // Can the device handle any descriptor layout?
    VIRTIO_F_ANY_LAYOUT	     	 = 27,

    // >>>   transport-specific features start   >>>

    // v1.0 compliant. */
    VIRTIO_F_VERSION_1 	     	 = 32,

    //
    // If clear - device has the platform DMA (e.g. IOMMU) bypass quirk feature.
    // If set - use platform DMA tools to access the memory.
    //
    // Note the reverse polarity (compared to most other features),
    // this is for compatibility with legacy systems.
    VIRTIO_F_ACCESS_PLATFORM     = 33,

    // This feature indicates support for the packed virtqueue layout. */
    VIRTIO_F_RING_PACKED	 = 34,

     // Inorder feature indicates that all buffers are used by the device
     // in the same order in which they have been made available.
    VIRTIO_F_IN_ORDER	 	 = 35,

    // This feature indicates that memory accesses by the driver and the
    // device are ordered in a way described by the platform.
    VIRTIO_F_ORDER_PLATFORM	 = 36,

    // Does the device support Single Root I/O Virtualization?
    VIRTIO_F_SR_IOV		 = 37,

    // This feature indicates that the driver passes extra data (besides
    // identifying the virtqueue) in its device notifications.
    VIRTIO_F_NOTIFICATION_DATA   = 38,

    // This feature indicates that the driver uses the data provided by the device
    // as a virtqueue identifier in available buffer notifications.
    VIRTIO_F_NOTIF_CONFIG_DATA   = 39,

    // This feature indicates that the driver can reset a queue individually.
    VIRTIO_F_RING_RESET	 	 = 40,

    // This feature indicates that the device support administration virtqueues.
    VIRTIO_F_ADMIN_VQ	 	 = 41,

    // <<<   transport-specific features end   <<<

    VIRTIO_NET_F_DEVICE_STATS = 50, // Device can provide device-level statistics.
    VIRTIO_NET_F_VQ_NOTF_COAL = 52, // Device supports virtqueue notification coalescing
    VIRTIO_NET_F_NOTF_COAL    = 53, // Device supports notifications coalescing
    VIRTIO_NET_F_GUEST_USO4   = 54, // Guest can handle USOv4 in.
    VIRTIO_NET_F_GUEST_USO6   = 55, // Guest can handle USOv6 in.
    VIRTIO_NET_F_HOST_USO     = 56, // Host can handle USO in.
    VIRTIO_NET_F_HASH_REPORT  = 57, // Supports hash report
    VIRTIO_NET_F_GUEST_HDRLEN = 59, // Guest provides the exact hdr_len value.
    VIRTIO_NET_F_RSS          = 60, // Supports RSS RX steering
    VIRTIO_NET_F_RSC_EXT      = 61, // extended coalescing info
    VIRTIO_NET_F_STANDBY      = 62, // Act as standby for another device with the same MAC.
    VIRTIO_NET_F_SPEED_DUPLEX = 63  // Device set linkspeed and duplex

};

constexpr std::string_view VirtIONetDevFeatureDesc(const VirtIONetFeature feature)
{
    switch (feature) {
    case VirtIONetFeature::VIRTIO_NET_F_CSUM:
	return "host handles pkts w/ partial csum";
    case VirtIONetFeature::VIRTIO_NET_F_GUEST_CSUM:
	return "guest handles pkts w/ partial csum";
    case VirtIONetFeature::VIRTIO_NET_F_CTRL_GUEST_OFFLOADS:
	return "dynamic offload configuration";
    case VirtIONetFeature::VIRTIO_NET_F_MTU:
	return "initial MTU advice";
    case VirtIONetFeature::VIRTIO_NET_F_MAC:
	return "host has given MAC address";
    case VirtIONetFeature::VIRTIO_NET_F_GSO: //XXX: VIRTIO_CONFIG_NO_LEGACY
	return "host handles pkts w/ any GSO type. ";
    case VirtIONetFeature::VIRTIO_NET_F_GUEST_TSO4:
	return "guest can handle TSOv4 in";
    case VirtIONetFeature::VIRTIO_NET_F_GUEST_TSO6:
	return "guest can handle TSOv6 in";
    case VirtIONetFeature::VIRTIO_NET_F_GUEST_ECN:
	return "guest can handle TSO[6] w/ ECN in";
    case VirtIONetFeature::VIRTIO_NET_F_GUEST_UFO:
	return "guest can handle UFO in";
    case VirtIONetFeature::VIRTIO_NET_F_HOST_TSO4:
	return "host can handle TSOv4 in";
    case VirtIONetFeature::VIRTIO_NET_F_HOST_TSO6:
	return "host can handle TSOv6 in";
    case VirtIONetFeature::VIRTIO_NET_F_HOST_ECN:
	return "host can handle TSO[6] w/ ECN in";
    case VirtIONetFeature::VIRTIO_NET_F_HOST_UFO:
	return "host can handle UFO in";
    case VirtIONetFeature::VIRTIO_NET_F_MRG_RXBUF:
	return "host can merge receive buffers";
    case VirtIONetFeature::VIRTIO_NET_F_STATUS:
	return "configuration status field is available";
    case VirtIONetFeature::VIRTIO_NET_F_CTRL_VQ:
	return "control channel available";
    case VirtIONetFeature::VIRTIO_NET_F_CTRL_RX:
	return "control channel RX mode support";
    case VirtIONetFeature::VIRTIO_NET_F_CTRL_VLAN:
	return "control channel VLAN filtering";
    case VirtIONetFeature::VIRTIO_NET_F_CTRL_RX_EXTRA:
	return "extra RX mode control support";
    case VirtIONetFeature::VIRTIO_NET_F_GUEST_ANNOUNCE:
	return "guest can announce device on the network";
    case VirtIONetFeature::VIRTIO_NET_F_MQ:
	return "device supports auto Receive Flow Steering";
    case VirtIONetFeature::VIRTIO_NET_F_CTRL_MAC_ADDR:
	return "set MAC address";


    // XXX: VIRTIO_CONFIG_NO_LEGACY
    // Do we get callbacks when the ring is completely used, even if we've suppressed them?
    case VirtIONetFeature::VIRTIO_F_NOTIFY_ON_EMPTY:
	return "guest receives notifications when the ring is fully used";

    // XXX: VIRTIO_CONFIG_NO_LEGACY
    // Can the device handle any descriptor layout?
    case VirtIONetFeature::VIRTIO_F_ANY_LAYOUT:
	return "host device can handle any descriptor layout";

    // >>>   transport-specific features start   >>>

    // v1.0 compliant. */
    case VirtIONetFeature::VIRTIO_F_VERSION_1:
	return "v1.0 spec compliant";

    //
    // If clear - device has the platform DMA (e.g. IOMMU) bypass quirk feature.
    // If set - use platform DMA tools to access the memory.
    //
    // Note the reverse polarity (compared to most other features),
    // this is for compatibility with legacy systems.
    case VirtIONetFeature::VIRTIO_F_ACCESS_PLATFORM:
	return "use platform DMA tools to access the memory";

    // This feature indicates support for the packed virtqueue layout. */
    case VirtIONetFeature::VIRTIO_F_RING_PACKED:
	return "packed virtqueue layout supported";

     // Inorder feature indicates that all buffers are used by the device
     // in the same order in which they have been made available.
    case VirtIONetFeature::VIRTIO_F_IN_ORDER:
	return "device uses buffer in order they've been made available";

    // This feature indicates that memory accesses by the driver and the
    // device are ordered in a way described by the platform.
    case VirtIONetFeature::VIRTIO_F_ORDER_PLATFORM:
	return "memory accesses by device/driver are platform-defined";

    // Does the device support Single Root I/O Virtualization?
    case VirtIONetFeature::VIRTIO_F_SR_IOV:
	return "SR-IOV support";

    // This feature indicates that the driver passes extra data (besides
    // identifying the virtqueue) in its device notifications.
    case VirtIONetFeature::VIRTIO_F_NOTIFICATION_DATA:
	return "guest -> host notification contains extra data";

    // This feature indicates that the driver uses the data provided by the device
    // as a virtqueue identifier in available buffer notifications.
    case VirtIONetFeature::VIRTIO_F_NOTIF_CONFIG_DATA:
	return "guest -> host notification has virtqueue idx as data";

    // This feature indicates that the driver can reset a queue individually.
    case VirtIONetFeature::VIRTIO_F_RING_RESET:
	return "guest supports selective queue reset";

    // This feature indicates that the device support administration virtqueues.
    case VirtIONetFeature::VIRTIO_F_ADMIN_VQ:
	return "host supports admin vq";

    // <<<   transport-specific features end   <<<

    case VirtIONetFeature::VIRTIO_NET_F_DEVICE_STATS:
	return "device can provide device-level statistics";
    case VirtIONetFeature::VIRTIO_NET_F_VQ_NOTF_COAL:
	return "device supports virtqueue notification coalescing";
    case VirtIONetFeature::VIRTIO_NET_F_NOTF_COAL:
	return "device supports notifications coalescing";
    case VirtIONetFeature::VIRTIO_NET_F_GUEST_USO4:
	return "guest can handle USOv4 in";
    case VirtIONetFeature::VIRTIO_NET_F_GUEST_USO6:
	return "guest can handle USOv6 in";
    case VirtIONetFeature::VIRTIO_NET_F_HOST_USO:
	return "host can handle USO in";
    case VirtIONetFeature::VIRTIO_NET_F_HASH_REPORT:
	return "host supports per-packet hash value/type report";
    case VirtIONetFeature::VIRTIO_NET_F_GUEST_HDRLEN:
	return "guest provides the exact hdr_len value";
    case VirtIONetFeature::VIRTIO_NET_F_RSS:
	return "host supports RSS RX steering";
    case VirtIONetFeature::VIRTIO_NET_F_RSC_EXT:
	return "host supports duplicated ACKs, extended coalescing info";
    case VirtIONetFeature::VIRTIO_NET_F_STANDBY:
	return "host can act as standby for another device with the same MAC";
    case VirtIONetFeature::VIRTIO_NET_F_SPEED_DUPLEX:
	return "host reports linkspeed and duplex";
    default:
	return "< no desc >";
    }
};

} // namespace virtio
