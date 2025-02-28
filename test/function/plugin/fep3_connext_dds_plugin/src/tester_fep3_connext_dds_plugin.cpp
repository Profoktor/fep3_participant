/**
 * @file
 * @copyright
 * @verbatim
Copyright @ 2022 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */

#include <fep3/plugin/cpp/cpp_plugin_component_factory_intf.h>

#include <fep_components_plugin_test.h>

/**
 * General note on below tests:
 * All function names and types appearing in the plugin interface, are hard coded here rather than
 * taken from the FEP Participant Library exports, because we test the interfaces that are
 * potentially already released. This way we ensure, that if a potentially released interface is
 * changed, the test fails. For example, the (forbidden) change of the name of an existing exported
 * symbol (e. g. "fep3_plugin_cpp_arya_getFactory") will be detected by this test (while it wouldn't
 * be detected if the test would take the symbol name from the FEP Participant Library exports).
 * Such test logic also applies to the IIDs, so the IID's string is hard coded in the tests rather
 * than taken from Interface::FEP3_COMP_IID.
 */

using test::helper::FEPComponentsPluginFixture;

INSTANTIATE_TEST_SUITE_P(FEPNativeComponentsPlugin,
                         FEPComponentsPluginFixture,
                         ::testing::Values(FEP_COMPONENTS_PLUGIN_FILE_PATH));

/**
 * Tests loading components from the fep_components_plugin via fep3_plugin_cpp_arya_getFactory
 */
TEST_P(FEPComponentsPluginFixture, test_arya_createComponent)
{
    auto get_factory_function = get<fep3::plugin::cpp::arya::ICPPPluginComponentFactory*()>(
        "fep3_plugin_cpp_arya_getFactory");
    ASSERT_NE(nullptr, get_factory_function);
    std::unique_ptr<fep3::plugin::cpp::arya::ICPPPluginComponentFactory> component_factory(
        get_factory_function());
    ASSERT_TRUE(component_factory);

    auto arya_simulation_bus = component_factory->createComponent("simulation_bus.arya.fep3.iid");
    EXPECT_TRUE(arya_simulation_bus);
}

/**
 * Tests loading components from the fep_components_plugin via fep3_plugin_cpp_catelyn_getFactory
 */
TEST_P(FEPComponentsPluginFixture, test_catelyn_createComponent)
{
    auto get_factory_function =
        get<fep3::plugin::cpp::catelyn::IComponentFactory*()>("fep3_plugin_cpp_catelyn_getFactory");
    ASSERT_NE(nullptr, get_factory_function);
    std::unique_ptr<fep3::plugin::cpp::catelyn::IComponentFactory> component_factory(
        get_factory_function());
    ASSERT_TRUE(component_factory);

    auto arya_simulation_bus = component_factory->createComponent("simulation_bus.arya.fep3.iid");
    EXPECT_TRUE(arya_simulation_bus);
}
