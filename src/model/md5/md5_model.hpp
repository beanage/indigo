#ifndef __MD5_MODEL_HPP_INCLUDED__
#define __MD5_MODEL_HPP_INCLUDED__

#include "model/model.hpp"
#include "md5_bone.hpp"
#include "md5_mesh.hpp"

#include <memory>

namespace indigo {
class md5_model : public model
{
    friend class md5_loader;

public:
    virtual ~md5_model();

    std::vector<bone*> bones();
    std::vector<mesh*> meshes();

    void render() const;
    void upload();

private:
    void upload_bones() const;

    std::vector<std::unique_ptr<md5_bone>> bones_;
    std::vector<std::unique_ptr<md5_mesh>> meshes_;
};
}

#endif
