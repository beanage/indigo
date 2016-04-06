#pragma once

#include "model/mesh.hpp"
#include "platform/resource_manager.hpp"

#include <memory>
#include <string>
#include <vector>

namespace indigo {
class obj_loader : public resource_loader<mesh>
{
public:
    obj_loader();

    bool can_load(std::string const& extension) const override;
    std::shared_ptr<mesh> load(std::string const& stream) override;
};
}
