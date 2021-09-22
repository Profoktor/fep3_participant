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


#include "components_configuration.h"
#include <a_util/xml.h>
#include <a_util/filesystem.h>
#include <a_util/strings.h>

namespace fep3
{
namespace arya
{

    ComponentsConfiguration::ComponentsConfiguration(const std::string& file_path)
        : _items(load(file_path))
    {
    }
    std::vector<ComponentsConfiguration::ComponentConfiguration> ComponentsConfiguration::load(const std::string& file_path)
    {
        std::vector<ComponentConfiguration> items;
        a_util::xml::DOM loaded_file;
        if (!loaded_file.load(file_path))
        {
            auto error_message = a_util::strings::format("can not loaded %s - Error : %s",
                file_path.c_str(),
                loaded_file.getLastError().c_str());
            throw std::runtime_error(error_message);
        }

        a_util::xml::DOMElement schema_version;
        if (loaded_file.getRoot().findNode("schema_version", schema_version))
        {
            auto schema_version_string = schema_version.getData();
            if (a_util::strings::trim(schema_version_string) == "1.0.0")
            {

            }
            else
            {
                auto error_message = a_util::strings::format("can not loaded %s - Error : wrong schema version found : expect %s - found %s",
                    file_path.c_str(),
                    "1.0.0",
                    schema_version_string.c_str());
                throw std::runtime_error(error_message);
            }
        }
        else
        {
            auto error_message = a_util::strings::format("can not loaded %s - Error : %s",
                file_path.c_str(),
                "no schema version tag found");
            throw std::runtime_error(error_message);
        }


        { //sets the path
            a_util::filesystem::Path filepath = file_path;
            if (filepath.isRelative())
            {
                filepath = a_util::filesystem::getWorkingDirectory().append(filepath);
                filepath.makeCanonical();
            }
        }

        a_util::xml::DOMElementList comps;
        if (loaded_file.getRoot().findNodes("component", comps))
        {
            { //validate it

                for (const auto& comp_node : comps)
                {
                    auto source_node = comp_node.getChild("source");
                    auto comp_iid = comp_node.getChild("iid");

                    if (source_node.isNull())
                    {
                        auto error_message = a_util::strings::format("can not loaded %s - Error : %s",
                            file_path.c_str(),
                            "no iid node for component tag found");
                        throw std::runtime_error(error_message);
                    }
                    if (comp_iid.isNull())
                    {
                        auto error_message = a_util::strings::format("can not loaded %s - Error : %s",
                            file_path.c_str(),
                            "no source node for component tag found");
                        throw std::runtime_error(error_message);
                    }
                }
            }
            { // fill items
                for (const auto& comp_node : comps)
                {
                    auto source_node = comp_node.getChild("source");
                    auto comp_iid = comp_node.getChild("iid").getData();
                    auto source_type = fep3::arya::getComponentSourceType(source_node.getAttribute("type"));
                    auto source_file_string = source_node.getData();
                    a_util::strings::trim(source_file_string);
                    a_util::filesystem::Path source_file_path;
                    if (!source_file_string.empty())
                    {
                        source_file_path = source_file_string;
                    }
                    if (!source_file_path.isEmpty() && source_file_path.isRelative())
                    {
                        //we make it relative to the File! (not the workingdirectory!!)
                        source_file_path = a_util::filesystem::Path(file_path).getParent().append(source_file_path);
                        source_file_path.makeCanonical();
                    }
                    items.push_back({comp_iid, {source_type, source_file_path}});
                }
            }
        }
        else
        {
            //everything is fine, but there are no components in the file defined
        }
        return items;
    }

    std::vector<ComponentsConfiguration::ComponentConfiguration> ComponentsConfiguration::getItems() const
    {
        return _items;
    }
}
}