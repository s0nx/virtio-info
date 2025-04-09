// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (C) 2025 Petr Vyazovik <xen@f-m.fm>

#pragma once

#include <string>
#include <cstdint>

namespace cfg {

enum class OperationMode
{
    ListAvailDevs,
    ShowDevInfo,
    FeaturesDiff,
    ListDevTypes,
    RawFeaturesDecoding
};

struct CmdLOpts
{
    OperationMode              mode_ {OperationMode::ListAvailDevs};
    std::string      first_dev_name_ {};
    std::string     second_dev_name_ {};
    uint8_t	           dev_type_ {0};
    uint64_t	       raw_features_ {0};

    // do not show bit description
    bool               no_feat_desc_ {false};
    // show only the features bits that have been set
    bool         feat_set_bits_only_ {false};
    // do not display device status bits decoding
    bool                  no_status_ {false};
};

void ParseCmdLineOptions(CmdLOpts &cmdl_opts, int argc, char *argv[]);
} // namespace cfg

