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


#include <fep3/components/configuration/c_access_wrapper/configuration_service_c_access_wrapper.h>
// Note: a C plugin must not link against the fep3 participant private (object) library, so we need an
// explicit relative path to the mock class file; a better solution might be to create dedicated header-only
// private library containing the mock files.
#include "../../../../../../../src/fep3/components/configuration/mock/mock_configuration_service.h"

extern fep3::mock::TransferableConfigurationService* g_mock_configuration_service;

fep3_plugin_c_InterfaceError fep3_plugin_c_arya_createConfigurationService
    (fep3_arya_SIConfigurationService* access
    , fep3_plugin_c_arya_SISharedBinary shared_binary_access
    , const char* iid
    )
{
    // for testing we must use the factory function in "detail" because we need a custom factory
    // to be able to return the mock object
    auto result = ::fep3::plugin::c::wrapper::arya::detail::createConfigurationService
        ([]()
            {
                return g_mock_configuration_service;
            }
        , access
        , shared_binary_access
        , iid
        );
    return result;
}