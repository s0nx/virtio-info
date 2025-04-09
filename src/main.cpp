// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (C) 2025 Petr Vyazovik <xen@f-m.fm>

#include <unistd.h>
#include <fmt/core.h>

#include "config.h"
#include "ui.h"

cfg::CmdLOpts cmdl_opts;

int main(int argc, char *argv[])
{
    try {
        cfg::ParseCmdLineOptions(cmdl_opts, argc, argv);

        switch (cmdl_opts.mode_) {
        case cfg::OperationMode::ListAvailDevs:
            ui::ListVirtIODevices();
            break;
        case cfg::OperationMode::ShowDevInfo:
            ui::VirtIODevDetailedInfo();
            break;
        case cfg::OperationMode::FeaturesDiff:
            ui::VirtIODevFeaturesDiff();
            break;
        case cfg::OperationMode::ListDevTypes:
            ui::ListVirtIODevTypes();
            break;
        default:
            break;
        }
    } catch (std::exception &ex) {
        fmt::print("{}\n", ex.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
