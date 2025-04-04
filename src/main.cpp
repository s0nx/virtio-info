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

        if (cmdl_opts.mode_ == cfg::OperationMode::ListAvailDevs)
            ui::ListVirtIODevices();
        else if (cmdl_opts.mode_ == cfg::OperationMode::ShowDevInfo)
            ui::VirtIODevDetailedInfo();
        else
            ui::VirtIODevFeaturesDiff();
    } catch (std::exception &ex) {
        fmt::print("{}\n", ex.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
