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

#include <fep3/fep3_participant_version.h>
#include <fep3/plugin/base/plugin_base_intf.h>

/**
 * Defines the symbol name of the function that checks if the plugin has been compiled in debug mode
 * symbols that must never change, because they are not bound to a namespace version
 */
#define SYMBOL_fep3_plugin_cpp_isDebugPlugin "fep3_plugin_cpp_isDebugPlugin"
/**
 * Defines the symbol name of the function that returns the version namespace string (e. g. "arya",
 * "bronn", etc.)
 * @note Depending on the return value the corresponding
 * fep3_plugin_cpp_${version_namespace}_getFactory will be called by the plugin system
 */
#define SYMBOL_fep3_plugin_getVersionNamespace "fep3_plugin_getVersionNamespace"

/**
 * @brief   Defines the symbol name of the function that creates a factory in version "catelyn"
 * @note    The FEP Participant Library does not support loading old plugins, so only the
 *          latest version (arya/bronn/etc. ) of the function is defined here.
 */
#define SYMBOL_fep3_plugin_cpp_catelyn_getFactory "fep3_plugin_cpp_catelyn_getFactory"
