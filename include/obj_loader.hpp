#ifndef __OBJ_LOADER_HPP_INCLUDED__
#define __OBJ_LOADER_HPP_INCLUDED__

#include "mesh.hpp"
#include "resource_manager.hpp"

#include <memory>
#include <string>
#include <vector>

namespace indigo
{
class obj_loader : public resource_loader<mesh>
{
public:
    obj_loader();

    bool can_load(const std::string &extension) override;
    std::shared_ptr<mesh> load(std::istream& stream) override;
};
}

#endif
