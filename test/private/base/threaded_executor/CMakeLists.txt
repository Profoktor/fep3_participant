#
# Copyright @ 2021 VW Group. All rights reserved.
#
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.


add_executable(test_threaded_executor test_threaded_executor.cpp)

add_test(NAME test_threaded_executor
    COMMAND test_threaded_executor
)

target_link_libraries(test_threaded_executor PRIVATE
    GTest::gtest_main
    GTest::gmock
    fep3_thread_utilities
)

set_target_properties(test_threaded_executor PROPERTIES FOLDER "test/private/base")
