// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (C) 2025 Petr Vyazovik <xen@f-m.fm>

#pragma once

#include <cstdint>
#include <string>

namespace cfg {

enum class OperationMode
{
    ListAvailDevs,
    ShowDevInfo,
};

struct CmdLOpts
{
    OperationMode            mode_;
    std::string   target_dev_name_;
};

void ParseCmdLineOptions(CmdLOpts &cmdl_opts, int argc, char *argv[]);
} // namespace cfg

