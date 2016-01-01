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
        std::unique_ptr<obj_material> mat(new obj_material);
        // ...

        return mat;
}
