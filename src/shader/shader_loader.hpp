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
    template<shader::type T>
    class shader_loader : public resource_loader<shader>
    {
    public:
        shader_loader();

        bool can_load(std::string const& extension) const override;
        std::shared_ptr<shader> load(std::string const& stream) override;
    };

    shader_loader::shader_loader()
    {

    }

    bool shader_loader::can_load(std::string const &extension) const
    {
        return extension == "shader";
    }

    std::shared_ptr<shader> shader_loader::load(std::string const& filename)
    {
        std::ifstream stream(filename);
        if (stream.fail())
            throw std::runtime_error("[shader_loader] can not open file!");

        stream.imbue(std::locale("C"));

        std::string content(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());

        std::shared_ptr<shader> result = std::make_shared<shader>();
    }
}
