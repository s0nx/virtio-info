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

    auto sgrp = app.add_option_group("+grp");
    // hide help in group
    sgrp->set_help_flag();
    sgrp->require_option(1);

    sgrp->add_option_function<std::string>(
            "-i,--info",
            [&](const std::string &val) {
                cmdl_opts.mode_ = OperationMode::ShowDevInfo;
                cmdl_opts.target_dev_name_ = val;
            },
            "show detailed info about specific VirtIO device")
        ->option_text("< existing device name (e.g. virtio0) >")
        ->check(ExistingDeviceValidator());

    sgrp->add_flag_callback(
            "-l,--list",
            [&]() {
                cmdl_opts.mode_ = OperationMode::ListAvailDevs;
            },
            "show registered VirtIO devices")
        ->allow_extra_args(false);

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

