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

#include <fep3/components/base/component_iid.h>
#include <string>

class ITestComponentA
{
protected:
    //we dont want not be deleted thru the interface!!
    virtual ~ITestComponentA() = default;
public:
    FEP_COMPONENT_IID("ITestComponentA");

public:
    virtual std::string getIdentifier() const = 0;
    virtual int32_t get() = 0;
    virtual void set(int32_t value) = 0;
};



