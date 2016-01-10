#include "md5_model.hpp"

#include <algorithm>

using namespace indigo;

md5_model::~md5_model()
{}

std::vector<bone*> md5_model::bones()
{
    std::vector<bone*> result;
    std::transform(bones_.begin(), bones_.end(), std::back_insert_iterator<typeof(result)>(result), [&](std::unique_ptr<md5_bone> const& m){
       return m.get();
    });

    return result;
}

std::vector<mesh*> md5_model::meshes()
{
    std::vector<mesh*> result;
    std::transform(meshes_.begin(), meshes_.end(), std::back_insert_iterator<typeof(result)>(result), [&](std::unique_ptr<md5_mesh> const& m){
       return m.get();
    });

    return result;
}

void md5_model::render() const
{
    upload_bones();

    for (auto const& m : meshes_)
        m->render();
}

void md5_model::upload()
{
    for (auto const& m : meshes_)
        m->upload();
}

void md5_model::upload_bones() const
{
    // TODO: Upload bone matrixes to union array
}
