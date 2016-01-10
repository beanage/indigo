#ifndef __MD5_LOADER_HPP_INCLUDED__
#define __MD5_LOADER_HPP_INCLUDED__

#include "model.hpp"
#include "resource_manager.hpp"

#include <memory>
#include <istream>
#include <string>

namespace indigo
{
class md5_mesh;

class md5_loader : public resource_loader<model>
{
public:
    class scanner;

    md5_loader() {}

	bool can_load(std::string const& extension) const override;
    std::shared_ptr<model> load(std::istream& stream) override;

private:
    std::unique_ptr<md5_mesh> parse_mesh(scanner& s);
};
}

#endif
