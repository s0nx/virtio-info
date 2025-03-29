// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (C) 2025 Petr Vyazovik <xen@f-m.fm>

#include <unistd.h>
#include <fmt/core.h>

#include "config.h"
#include "virtio_bus.h"

cfg::CmdLOpts    cmdline_options;

int main(int argc, char *argv[])
{
    try {
        cfg::ParseCmdLineOptions(cmdline_options, argc, argv);

        if (cmdline_options.mode_ == cfg::OperationMode::ListAvailDevs) {
            fmt::print("LIST AVAIL DEVS\n");
        } else {
            fmt::print("SHOW DEV INFO for [{}]\n",
                       cmdline_options.target_dev_name_);

        }
    } catch (std::exception &ex) {
        fmt::print("{}\n", ex.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
