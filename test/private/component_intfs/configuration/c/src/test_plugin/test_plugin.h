/**
 * @file
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

    This Source Code Form is subject to the terms of the Mozilla
    Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

If it is not possible or desirable to put the notice in a particular file, then
You may include the notice in a location (such as a LICENSE file in a
relevant directory) where a recipient would be likely to look for such a notice.

You may add additional accurate notices of copyright ownership.

@endverbatim
 */


#pragma once

#include <fep3/plugin/c/c_plugin.h>
#include <fep3/plugin/base/fep3_calling_convention.h>
#include <fep3/plugin/base/fep3_plugin_export.h>
#include <fep3/components/base/c_access_wrapper/transferable_component_base.h>
// Note: a C plugin must not link against the fep3 participant private (object) library, so we need an
// explicit relative path to the mock class file; a better solution might be to create dedicated header-only
// private library containing the mock files.
#include "../../../../../../../src/fep3/components/configuration/mock/mock_configuration_service.h"

extern fep3::mock::TransferableConfigurationService* g_mock_configuration_service;

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * Sets the mock Configuration Service
 * @param[in] mock_configuration_service Pointer to the mock Configuration Service to be set to the plugin
 * @note This destroys binary compatibilty of the plugin, because a C++ interface is introduced.
 *       This is ok, as long as plugin and test are compiled with the same compiler and compiler settings
 *       (which is guaranteed in the unit test context).
 */
FEP3_PLUGIN_EXPORT void FEP3_PLUGIN_CALL setMockConfigurationService(fep3::mock::TransferableConfigurationService* mock_configuration_service);

#ifdef __cplusplus
}
#endif
