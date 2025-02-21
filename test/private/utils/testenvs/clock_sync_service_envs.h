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

#include "clock_service_envs.h"

#include <fep3/native_components/clock_sync/clock_sync_service.h>

namespace fep3 {
namespace test {
namespace env {

using namespace ::testing;

struct NativeClockSyncService : NativeClockService {
    NativeClockSyncService()
    {
    }

    void SetUp() override
    {
        EXPECT_CALL(*_service_bus, getServer()).Times(1).WillOnce(::testing::Return(_rpc_server));
        EXPECT_CALL(*_rpc_server,
                    registerService(rpc::IRPCClockSyncMasterDef::getRPCDefaultName(), _))
            .Times(1)
            .WillOnce(::testing::Return(fep3::Result()));
        EXPECT_CALL(*_rpc_server, registerService(rpc::IRPCClockServiceDef::getRPCDefaultName(), _))
            .Times(1)
            .WillOnce(::testing::Return(fep3::Result()));
        EXPECT_CALL(*_configuration_service_mock, registerNode(_))
            .Times(2)
            .WillRepeatedly(
                DoAll(WithArg<0>(Invoke([&](const std::shared_ptr<IPropertyNode>& node) {
                          if (node->getName() == FEP3_CLOCK_SERVICE_CONFIG) {
                              _clock_service_property_node = node;
                          }
                          else {
                              _clock_sync_service_property_node = node;
                          }
                      })),
                      ::testing::Return(Result())));
        registerComponents();
        setComponents();

        ASSERT_FEP3_NOERROR(_component_registry->create());
    }

    void registerComponents() override
    {
        ASSERT_FEP3_NOERROR(_component_registry->registerComponent<fep3::IServiceBus>(
            _service_bus, _dummyComponentVersionInfo));

        auto logging_service = std::make_shared<LoggingService>();
        EXPECT_CALL(*logging_service, createLogger(_)).WillRepeatedly(Return(_logger));
        ASSERT_FEP3_NOERROR(_component_registry->registerComponent<fep3::ILoggingService>(
            logging_service, _dummyComponentVersionInfo));

        ASSERT_FEP3_NOERROR(_component_registry->registerComponent<fep3::IConfigurationService>(
            _configuration_service_mock, _dummyComponentVersionInfo));

        _clock_service_impl = std::make_shared<fep3::native::ClockService>();
        ASSERT_FEP3_NOERROR(_component_registry->registerComponent<fep3::IClockService>(
            _clock_service_impl, _dummyComponentVersionInfo));

        _clock_sync_service_impl = std::make_shared<fep3::native::ClockSynchronizationService>();

        ASSERT_FEP3_NOERROR(_component_registry->registerComponent<fep3::IClockSyncService>(
            _clock_sync_service_impl, _dummyComponentVersionInfo));
    }

    void setComponents() override
    {
        _clock_sync_service_intf = _component_registry->getComponent<fep3::IClockSyncService>();
        ASSERT_NE(_clock_sync_service_intf, nullptr);
    }

    std::shared_ptr<fep3::native::ClockSynchronizationService> _clock_sync_service_impl{};
    fep3::IClockSyncService* _clock_sync_service_intf{nullptr};
    std::shared_ptr<IPropertyNode> _clock_sync_service_property_node;
    fep3::ComponentVersionInfo _dummyComponentVersionInfo{"3.0.1", "dummyPath", "3.1.0"};
};

} // namespace env
} // namespace test
} // namespace fep3
