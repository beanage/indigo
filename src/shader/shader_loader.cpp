#include "shader/shader_loader.hpp"
#include <fstream>
#include <sstream>

using namespace indigo;

const std::string shader_loader::vert_shader_extension = "vert";
const std::string shader_loader::frag_shader_extension = "frag";

shader_loader::shader_loader()
{

}

bool shader_loader::can_load(std::string const &extension) const
{
    return extension == vert_shader_extension || extension == frag_shader_extension;
}

std::shared_ptr<shader> shader_loader::load(std::string const& filename)
{
    std::ifstream stream(filename);
    if (stream.fail())
        throw std::runtime_error(std::string("[shader_loader] failed to open file ")+filename+"!");

    stream.imbue(std::locale("C"));

    std::string content{std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>()};

    shader::type type = shader::vertex;
    if(filesystem::extension(filename) == frag_shader_extension)
        type = shader::fragment;

    std::shared_ptr<shader> result = std::make_shared<shader>(type);
    result->source(content);

    if(!result->compile())
        throw std::runtime_error(std::string("[shader_loader] failed to compile ")+filename+"!");

    return result;
}
