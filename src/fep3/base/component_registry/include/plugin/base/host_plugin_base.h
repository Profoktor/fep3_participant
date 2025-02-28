/**
 * @file
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */

#pragma once

#include "shared_library.h"

#include <fep3/base/component_registry/component_version_info.h>
#include <fep3/plugin/base/plugin_base_intf.h>

namespace fep3 {
namespace plugin {

/**
 * Class representing the participant library version
 */
class ParticipantLibraryVersion {
public:
    ParticipantLibraryVersion() = default;
    ParticipantLibraryVersion(
        const std::string& id, int32_t major, int32_t minor, int32_t patch, int32_t build);
    ParticipantLibraryVersion(
        const fep3_plugin_base_ParticipantLibraryVersion& participant_library_version);
    bool operator==(const ParticipantLibraryVersion& other) const;

    std::string getId() const;
    int32_t getMajor() const;
    int32_t getMinor() const;
    int32_t getPatch() const;
    int32_t getBuild() const;
    std::string toString() const;

private:
    std::string _id;
    int32_t _major{0};
    int32_t _minor{0};
    int32_t _patch{0};
    int32_t _build{0};
};

class HostPluginBase : /*public boost::dll::shared_library,*/ public SharedLibrary {
public:
    explicit HostPluginBase(const std::string& file_path, bool prevent_unloading = false);
    HostPluginBase(HostPluginBase&&) = default;
    virtual ~HostPluginBase();

    virtual std::string getPluginVersion() const;
    virtual ParticipantLibraryVersion getParticipantLibraryVersion() const;

    ComponentVersionInfo getPluginInfo() const;

protected:
    std::string _plugin_version{"unknown"};
    ParticipantLibraryVersion _participant_library_version{};
};

} // namespace plugin
} // namespace fep3