// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (C) 2025 Petr Vyazovik <xen@f-m.fm>

#include "ui.h"
#include <fmt/core.h>
#include <fmt/color.h>

#include <vector>

#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>
#include <ftxui/screen/screen.hpp>

#include "ftxui/dom/node.hpp"
#include "ftxui/screen/color.hpp"

#include "magic_enum/magic_enum.hpp"

using namespace ftxui;

namespace ui {

void ListVirtIODevices()
{
    auto devs = virtio::GetVirtioDevMap();

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

    auto screen = Screen::Create(Dimension::Fit(doc, true));
    Render(screen, doc);

    screen.Print();
    fmt::print("\n");
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

static void
VirtIONetDevFeaturesTablePopulate(const uint64_t features, std::vector<Elements> &tbl)
{
    constexpr auto vfeature_fields = magic_enum::enum_values<virtio::VirtIONetFeature>();
    for (const auto &field : vfeature_fields) {
        Elements row_elems;

        bool bit_is_set = (features >> magic_enum::enum_integer(field)) & 0x1;

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
        bool is_feature_transport_specific = virtio::FeatureBitIsTransport(e_to_type(field));

        auto f_type_elem = text(fmt::format("{:#>1}",
                                            is_feature_transport_specific ? 'T' : 'D' ));
        row_elems.push_back(f_type_elem);

        auto bit_name_elem = text(std::string(magic_enum::enum_name(field)));
        if (bit_is_set) {
            bit_name_elem |= bgcolor(Color::Green) | color(Color::Grey15);
        } else {
            bit_name_elem |= dim;
        }

        row_elems.push_back(bit_name_elem);

        auto field_desc_elem = text(std::string{virtio::VirtIONetDevFeatureDesc(field)});
        if (!bit_is_set)
            field_desc_elem |= dim;

        row_elems.push_back(field_desc_elem);

        tbl.push_back(std::move(row_elems));
    }
}

static void
VirtIODevFeaturesTablePopulate(const uint64_t features,
                               const virtio::VirtIODevType dev_type,
                               std::vector<Elements> &tbl)
{
    switch (dev_type) {
    case virtio::VirtIODevType::network_card:
        return VirtIONetDevFeaturesTablePopulate(features, tbl);
    default:
        return;
    }
}

static Element
VirtIODevCreateFeaturesElement(const uint64_t dev_features,
                               const virtio::VirtIODevType dev_type)
{
    std::vector<Elements> tbl;
    tbl.push_back({text("bit "), text("ftype "), text("name "), text("desc ")});

    VirtIODevFeaturesTablePopulate(dev_features, dev_type, tbl);
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


void VirtIODevDetailedInfo(const std::string &dev_name)
{
    std::filesystem::path virtio_path {virtio::virtio_devs_path};
    auto dev_path = virtio_path / dev_name;

    auto dev_desc = virtio::CreateDevDesc(dev_path);

    auto dev_desc_elem = hbox({
            text(" Device ->"),
            separatorEmpty(),
            text(dev_name) | bold,
            separatorEmpty(),
            text(fmt::format("type [{:#2}]:", e_to_type(dev_desc.dev_type_))),
            separatorEmpty(),
            text(fmt::format("{}", virtio::VirtIODevTypeName(dev_desc.dev_type_)))
            | inverted,
            separatorEmpty(),
            text(dev_desc.aux_info_.empty() ?
                 "" : fmt::format("({})", dev_desc.aux_info_)) |
            color(Color::Green) | bold,
            filler()
    });

    auto status_element = VirtIODevCreateStatusElement(dev_desc.status_);
    auto features_element = VirtIODevCreateFeaturesElement(dev_desc.features_,
                                                           dev_desc.dev_type_);

    auto doc = vbox({
        dev_desc_elem,
        status_element,
        features_element
    });

    auto screen = Screen::Create(Dimension::Fit(doc, true));
    Render(screen, doc);

    screen.Print();
    fmt::print("\n");
}

} // namespace ui
