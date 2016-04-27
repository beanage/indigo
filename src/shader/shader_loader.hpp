#pragma once

#include "shader/shader.hpp"
#include "platform/resource_manager.hpp"

#include <memory>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

namespace indigo
{
    class shader_loader : public resource_loader<shader>
    {
        static const std::string vert_shader_extension;
        static const std::string frag_shader_extension;

    public:
        shader_loader();

        bool can_load(std::string const& extension) const override;
        std::shared_ptr<shader> load(std::string const& stream) override;
    };
}
