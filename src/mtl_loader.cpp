#include "mtl_loader.hpp"
#include "material.hpp"

using namespace indigo;

class obj_material : public material
{
public:
};

mtl_loader::mtl_loader()
{}

std::unique_ptr<material> mtl_loader::load(const std::string& file)
{
        obj_material* mat(nullptr/*new obj_material*/);
        // ...

        return std::unique_ptr<material>(mat);
}
