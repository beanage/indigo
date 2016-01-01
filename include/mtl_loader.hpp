#ifndef __MTL_LOADER_HPP_INCLUDED__
#define __MTL_LOADER_HPP_INCLUDED__

#include <memory>
#include <string>

namespace indigo
{
class material;

class mtl_loader
{
public:
        mtl_loader();

        std::unique_ptr<material> load(const std::string& file);
};
}

#endif

