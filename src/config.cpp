// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (C) 2025 Petr Vyazovik <xen@f-m.fm>

#include "config.h"
#include "virtio_bus.h"
#include "vi_version.h"

#include <fmt/core.h>

#include <CLI/CLI.hpp>

namespace cfg {

class ExistingDeviceValidator : public CLI::Validator {
    public:
        ExistingDeviceValidator()
            : CLI::Validator("VIRTIO_DEV_NAME")
        {
            func_ = [&](std::string &device_name) {
                std::filesystem::path path {virtio::virtio_devs_path};
                path /= device_name;

                auto check_res = CLI::detail::check_path(path.c_str());
                if(check_res == CLI::detail::path_type::nonexistent) {
                    return "Non-existent VirtIO device: " + device_name;
                }
                return std::string{};
            };
        }
};

void ParseCmdLineOptions(CmdLOpts &cmdl_opts, int argc, char *argv[])
{
    CLI::App app{"VirtIO devices info", "virtio-info"};
    app.get_formatter()->column_width(40);
    app.require_option();

    auto sgrp1 = app.add_option_group("+info");
    // hide help in group
    sgrp1->set_help_flag();

    sgrp1->add_option_function<std::string>(
            "-i,--info",
            [&](const std::string &val) {
                cmdl_opts.mode_ = OperationMode::ShowDevInfo;
                cmdl_opts.first_dev_name_ = val;
            },
            "show detailed info about specific VirtIO device")
        ->option_text("< existing device name (e.g. virtio0) >")
        ->check(ExistingDeviceValidator());

    sgrp1->add_flag_callback(
            "--no-status",
            [&]() {
                cmdl_opts.no_status_ = true;
            },
            "don't show device status bits decoding");

    auto sgrp2 = app.add_option_group("+list");
    // hide help in group
    sgrp2->set_help_flag();
    sgrp2->excludes(sgrp1);

    sgrp2->add_flag_callback(
            "-l,--list",
            [&]() {
                cmdl_opts.mode_ = OperationMode::ListAvailDevs;
            },
            "show registered VirtIO devices")
        ->allow_extra_args(false);

    auto sgrp3 = app.add_option_group("+diff");
    sgrp3->set_help_flag();
    sgrp3->excludes(sgrp1);
    sgrp3->excludes(sgrp2);
    sgrp3->add_option_function<std::pair<std::string, std::string>>(
            "-d,--diff",
            [&](const std::pair<std::string, std::string> &val) {
                cmdl_opts.mode_ = OperationMode::FeaturesDiff;
                cmdl_opts.first_dev_name_ = val.first;
                cmdl_opts.second_dev_name_ = val.second;
            },
            "highlight features difference for two devices A and B")
        ->option_text("<device A> <device B>")
        ->check(ExistingDeviceValidator().application_index(0))
        ->check(ExistingDeviceValidator().application_index(1));

    app.add_flag_callback(
            "--no-desc",
            [&]() {
                cmdl_opts.no_feat_desc_ = true;
            },
            "don't show features bits description");

    app.add_flag_callback(
            "--feat-set",
            [&]() {
                cmdl_opts.feat_set_bits_only_ = true;
            },
            "display only the feature bits that have been set");

    app.add_flag("-v, --version",
            [](std::int64_t) {
                fmt::print("{} {}\n", vi_current_version, vi_current_hash);
                throw CLI::Success();
            },
            "Print version and exit");

    app.required(false);

    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError &e) {
        // app.exit(...) returns cli11-specific error
        auto _ = app.exit(e);
        std::exit(EXIT_FAILURE);
    }
}

} // namespace cfg

