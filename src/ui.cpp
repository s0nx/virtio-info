// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (C) 2025 Petr Vyazovik <xen@f-m.fm>

#include "ui.h"
#include <fmt/core.h>

#include <vector>

#include <ftxui/dom/table.hpp>
#include <ftxui/screen/screen.hpp>

#include "magic_enum/magic_enum.hpp"

extern cfg::CmdLOpts cmdl_opts;

using namespace ftxui;

namespace ui {

static void RenderOnScreen(Element elem)
{
    auto screen = Screen::Create(Dimension::Fit(elem, true));
    Render(screen, elem);

    screen.Print();
    fmt::print("\n");
}

void ListVirtIODevices()
{
    auto devs = virtio::GetVirtioDevMap();
    if (devs.empty()) {
        fmt::print("No registered VirtIO devices found\n");
        return;
    }

    std::vector<Elements> tbl;

    tbl.push_back({text("name "), text("type "),
                   text("features "), text("status ")});

    for (const auto &el : devs) {
        Elements row_elems;
        auto &desc = el.second;
        row_elems.push_back(text(el.first) | bold);
        auto type_elem = hbox({
                text(fmt::format("[{:>2}]", e_to_type(desc.dev_type_))),
                separatorEmpty(),
                text(fmt::format("{}", virtio::VirtIODevTypeName(desc.dev_type_))) | inverted,
                separatorEmpty(),
                text(desc.aux_info_.empty() ? "" :
                                              fmt::format("({})", desc.aux_info_)) |
                color(Color::Green) | bold,
                filler()
        });
        row_elems.push_back(type_elem);
        row_elems.push_back(text(fmt::format("{:<#016x}", desc.features_)));
        row_elems.push_back(text(fmt::format("{:#x}", desc.status_)));

        tbl.push_back(std::move(row_elems));
    }

    auto table = Table(std::move(tbl));
    table.SelectAll().Border(EMPTY);
    table.SelectAll().Separator(EMPTY);
    table.SelectRow(0).Border(EMPTY);
    table.SelectRow(0).DecorateCells(bold | bgcolor(Color::Yellow) | color(Color::Grey15));

    auto doc = vbox({
        hbox({
            separatorEmpty(),
            text(fmt::format("{} devices:", devs.size())) | underlined,
            filler()
        }),
        table.Render()
    });

    RenderOnScreen(doc);
}

static Element
VirtIODevCreateStatusElement(const uint32_t dev_status)
{
    std::vector<Elements> tbl;
    tbl.push_back({text("bit "), text("name "), text("desc ")});

    constexpr auto vstatus_fields = magic_enum::enum_values<virtio::VirtIOStatusBits>();
    for (const auto &field : vstatus_fields) {
        Elements row_elems;

        bool bit_is_set = dev_status & (1 << e_to_type(field));

        auto idx_txt_elem = text(fmt::format("[{}]", e_to_type(field)));
        if (bit_is_set)
            idx_txt_elem |= bold ;
        else
            idx_txt_elem |= dim;

        auto idx_elem = hbox({
            filler(),
            idx_txt_elem
        });
        row_elems.push_back(idx_elem);

        auto bit_name_elem = text(std::string(magic_enum::enum_name(field)));
        if (bit_is_set) {
            if (field == virtio::VirtIOStatusBits::VIRTIO_CONFIG_S_NEEDS_RESET)
                bit_name_elem |= bgcolor(Color::Yellow) | color(Color::Grey15);
            else if (field == virtio::VirtIOStatusBits::VIRTIO_CONFIG_S_FAILED)
                bit_name_elem |= bgcolor(Color::Red) | color(Color::Grey15);
            else
                bit_name_elem |= bgcolor(Color::Green) | color(Color::Grey15);
        } else {
            bit_name_elem |= dim;
        }

        row_elems.push_back(bit_name_elem);

        auto field_desc_elem = text(std::string{virtio::VirtIOStatusBitDesc(field)});
        if (!bit_is_set)
            field_desc_elem |= dim;

        row_elems.push_back(field_desc_elem);

        tbl.push_back(std::move(row_elems));
    }

    auto table = Table(std::move(tbl));
    table.SelectAll().Border(EMPTY);
    table.SelectAll().SeparatorVertical(EMPTY);
    table.SelectRow(0).Border(EMPTY);
    table.SelectRow(0).DecorateCells(bold | bgcolor(Color::Blue) | color(Color::Grey15));

    auto status_elem = vbox({
        hbox({
            separatorEmpty(),
            text(fmt::format("status -> {:#x}", dev_status)) | underlined,
            filler()
        }),
        table.Render()
    });

    return status_elem;
}

template <typename T, std::size_t S, typename F>
static void
DevFeaturesTablePopulate(const uint64_t dev1_features, const uint64_t dev2_features,
                         bool diff_mode,
                         std::vector<Elements> &tbl,
                         const std::array<T, S> &arr, F desc_fun)
{
    if (!diff_mode) {
        for (const auto &field : arr) {
            Elements row_elems;

            bool bit_is_set = (dev1_features >> magic_enum::enum_integer(field)) & 0x1;
            if (!bit_is_set && cmdl_opts.feat_set_bits_only_)
                continue;

            auto idx_txt_elem = text(fmt::format("[{}]", e_to_type(field)));
            if (bit_is_set)
                idx_txt_elem |= bold;
            else
                idx_txt_elem |= dim;

            auto idx_elem = hbox({
                filler(),
                idx_txt_elem
            });
            row_elems.push_back(idx_elem);

            // feature type (device-specific or transport-specific)
            bool is_feature_transport_specific =
                virtio::FeatureBitIsTransport(e_to_type(field));

            auto f_type_elem = text(
                    fmt::format("{:#>1}", is_feature_transport_specific ? 'T' : 'D' ));
            row_elems.push_back(f_type_elem);

            auto bit_name_elem = text(std::string(magic_enum::enum_name(field)));
            if (bit_is_set) {
                bit_name_elem |= bgcolor(Color::Green) | color(Color::Grey15);
            } else {
                bit_name_elem |= dim;
            }

            row_elems.push_back(bit_name_elem);

            if (!cmdl_opts.no_feat_desc_) {
                auto field_desc_elem =
                    text(std::string{desc_fun(field)});
                if (!bit_is_set)
                    field_desc_elem |= dim;

                row_elems.push_back(field_desc_elem);
            }

            tbl.push_back(std::move(row_elems));
        }
    } else {
        for (const auto &field : arr) {
            Elements row_elems;

            bool dev1_bit_is_set = (dev1_features >> magic_enum::enum_integer(field)) & 0x1;
            bool dev2_bit_is_set = (dev2_features >> magic_enum::enum_integer(field)) & 0x1;

            if (!dev1_bit_is_set && !dev2_bit_is_set)
                continue;

            auto idx_txt_elem = text(fmt::format("[{}]", e_to_type(field))) | bold;
            auto idx_elem = hbox({
                filler(),
                idx_txt_elem
            });
            row_elems.push_back(idx_elem);

            // feature type (device-specific or transport-specific)
            bool is_feature_transport_specific =
                virtio::FeatureBitIsTransport(e_to_type(field));

            auto f_type_elem = text(
                    fmt::format("{:#>1}", is_feature_transport_specific ? 'T' : 'D' ));
            row_elems.push_back(f_type_elem);

            auto bit_name = std::string {magic_enum::enum_name(field)};
            auto dev1_bit_elem = text(bit_name);
            auto dev2_bit_elem = text(bit_name);

            // set for both devices
            if (dev1_bit_is_set && dev2_bit_is_set) {
                dev1_bit_elem |= bgcolor(Color::Green) | color(Color::Grey15);
                dev2_bit_elem |= bgcolor(Color::Green) | color(Color::Grey15);
            }

            // set for dev1 only
            if (dev1_bit_is_set && !dev2_bit_is_set) {
                dev1_bit_elem |= bgcolor(Color::Yellow) | color(Color::Grey15);
                dev2_bit_elem |= dim;
            }

            // set for dev2 only
            if (dev2_bit_is_set && !dev1_bit_is_set) {
                dev1_bit_elem |= dim;
                dev2_bit_elem |= bgcolor(Color::Magenta) | color(Color::Grey15);
            }

            row_elems.push_back(dev1_bit_elem);
            row_elems.push_back(dev2_bit_elem);

            if (!cmdl_opts.no_feat_desc_) {
                auto field_desc_elem = text(std::string{desc_fun(field)});
                row_elems.push_back(field_desc_elem);
            }

            tbl.push_back(std::move(row_elems));
        }
    }
}

static void
VirtIODevFeaturesTablePopulate(const uint64_t dev1_features,
                               const uint64_t dev2_features,
                               bool diff_mode,
                               const virtio::VirtIODevType dev_type,
                               std::vector<Elements> &tbl)
{
    switch (dev_type) {
    case virtio::VirtIODevType::network_card:
        return DevFeaturesTablePopulate(
                dev1_features, dev2_features, diff_mode, tbl,
                magic_enum::enum_values<virtio::VirtIONetFeature>(),
                virtio::VirtIONetDevFeatureDesc);
    default:
        return;
    }
}

static Element
VirtIODevCreateFeaturesElement(const uint64_t dev_features,
                               const virtio::VirtIODevType dev_type)
{
    std::vector<Elements> tbl;
    if (cmdl_opts.no_feat_desc_)
        tbl.push_back({text("bit "), text("ft "), text("name ")});
    else
        tbl.push_back({text("bit "), text("ft "), text("name "), text("desc ")});

    VirtIODevFeaturesTablePopulate(dev_features, 0, false, dev_type, tbl);
    Element elem;
    if (tbl.size() == 1) {
        elem = text(
                fmt::format(" feature bits decoding is not implemented for [{}] device type",
                            virtio::VirtIODevTypeName(dev_type))) | dim;
    } else {
        auto table = Table(std::move(tbl));
        table.SelectAll().Border(EMPTY);
        table.SelectAll().SeparatorVertical(EMPTY);
        table.SelectRow(0).Border(EMPTY);
        table.SelectRow(0).DecorateCells(bold | bgcolor(Color::Blue) | color(Color::Grey15));
        elem = table.Render();
    }

    auto features_elem = vbox({
        hbox({
            separatorEmpty(),
            text(fmt::format("features -> {:#x}", dev_features)) | inverted,
            filler()
        }),
        elem
    });

    return features_elem;
}


void VirtIODevDetailedInfo()
{
    std::filesystem::path virtio_path {virtio::virtio_devs_path};
    auto dev_path = virtio_path / cmdl_opts.first_dev_name_;

    auto dev_desc = virtio::CreateDevDesc(dev_path);

    auto dev_desc_elem = hbox({
            text(" Device ->"),
            separatorEmpty(),
            text(cmdl_opts.first_dev_name_) | bold,
            separatorEmpty(),
            text(fmt::format("type [{:#2}]:", e_to_type(dev_desc.dev_type_))),
            separatorEmpty(),
            text(fmt::format("{}", virtio::VirtIODevTypeName(dev_desc.dev_type_)))
            | bold,
            separatorEmpty(),
            text(dev_desc.aux_info_.empty() ?
                 "" : fmt::format("({})", dev_desc.aux_info_)) |
            color(Color::Green) | bold,
            filler()
    });

    Elements elems {dev_desc_elem};

    if (!cmdl_opts.no_status_)
        elems.push_back(VirtIODevCreateStatusElement(dev_desc.status_));

    elems.push_back(VirtIODevCreateFeaturesElement(dev_desc.features_,
                                                   dev_desc.dev_type_));

    RenderOnScreen(vbox(elems));
}

void VirtIODevFeaturesDiff()
{
    std::filesystem::path virtio_path {virtio::virtio_devs_path};

    auto dev1_path = virtio_path / cmdl_opts.first_dev_name_;
    auto dev2_path = virtio_path / cmdl_opts.second_dev_name_;

    auto dev1_desc = virtio::CreateDevDesc(dev1_path);
    auto dev2_desc = virtio::CreateDevDesc(dev2_path);

    if (dev1_desc.dev_type_ != dev2_desc.dev_type_) {
        fmt::print("{} and {} devices are not of the same type\n",
                   cmdl_opts.first_dev_name_, cmdl_opts.second_dev_name_);
        return;
    }

    if (dev1_desc.features_ == dev2_desc.features_) {
        fmt::print("{} and {} devices have identical features negotiated\n",
                   cmdl_opts.first_dev_name_, cmdl_opts.second_dev_name_);
        return;
    }

    auto hdr_dev1_label_elem = hbox({
        text(fmt::format("{} ",
                         cmdl_opts.first_dev_name_)),
        dev1_desc.aux_info_.empty() ? text("")
                                    : text(fmt::format("({})", dev1_desc.aux_info_)) | bold
    });

    auto hdr_dev2_label_elem = hbox({
        text(fmt::format("{} ",
                         cmdl_opts.second_dev_name_)),
        dev2_desc.aux_info_.empty() ? text("")
                                    : text(fmt::format("({})", dev2_desc.aux_info_)) | bold
    });

    // table header for diff mode
    std::vector<Elements> tbl;
    Elements tbl_hdr_elems{text("bit "), text("ft "), hdr_dev1_label_elem, hdr_dev2_label_elem};
    if (!cmdl_opts.no_feat_desc_)
        tbl_hdr_elems.push_back(text("desc "));

    tbl.push_back(tbl_hdr_elems);

    VirtIODevFeaturesTablePopulate(dev1_desc.features_, dev2_desc.features_,
                                   true, dev1_desc.dev_type_, tbl);

    auto table = Table(std::move(tbl));
    table.SelectAll().Border(EMPTY);
    table.SelectAll().SeparatorVertical(EMPTY);
    table.SelectRow(0).Border(EMPTY);

    table.SelectRectangle(0, 1, 0, 0).DecorateCells(bold | bgcolor(Color::Blue) | color(Color::Grey15));
    table.SelectRectangle(4, 4, 0, 0).DecorateCells(bold | bgcolor(Color::Blue) | color(Color::Grey15));

    table.SelectCell(2, 0).DecorateCells(bold | bgcolor(Color::Yellow) | color(Color::Grey15));
    table.SelectCell(3, 0).DecorateCells(bold | bgcolor(Color::Magenta) | color(Color::Grey15));

    RenderOnScreen(table.Render());
}

void ListVirtIODevTypes()
{
    std::vector<Elements> tbl;

    tbl.push_back({text("id "), text("name ")});

    constexpr auto device_types = magic_enum::enum_values<virtio::VirtIODevType>();
    for (const auto &dtype : device_types) {
        Elements row_elems;

        auto idx_elem = hbox({
            filler(),
            text(fmt::format("[{}]", e_to_type(dtype)))
        });
        row_elems.push_back(idx_elem);

        row_elems.push_back(text(std::string(virtio::VirtIODevTypeName(dtype))) | bold);
        tbl.push_back(std::move(row_elems));
    }

    auto table = Table(std::move(tbl));
    table.SelectAll().Border(EMPTY);
    table.SelectAll().SeparatorVertical(EMPTY);
    table.SelectRow(0).Border(EMPTY);
    table.SelectRow(0).DecorateCells(bold | bgcolor(Color::Blue) | color(Color::Grey15));

    auto doc = vbox({
        hbox({
            separatorEmpty(),
            text(fmt::format("{} device types:", device_types.size())) | bold,
            filler()
        }),
        table.Render()
    });

    RenderOnScreen(doc);
}

void VirtIODevRawFeaturesInfo()
{
    auto feat_elem = VirtIODevCreateFeaturesElement(cmdl_opts.raw_features_,
                                                    virtio::VirtIODevType {cmdl_opts.dev_type_});

    auto doc = vbox({
        text(fmt::format(" type: [{}] -> {}", cmdl_opts.dev_type_,
                         virtio::VirtIODevTypeName(virtio::VirtIODevType{cmdl_opts.dev_type_}))) | bold,
        feat_elem
    });

    RenderOnScreen(doc);
}

} // namespace ui
