#ifndef __OBJ_LOADER_HPP_INCLUDED__
#define __OBJ_LOADER_HPP_INCLUDED__

#include "mesh.hpp"

#include <memory>
#include <string>

namespace indigo
{
	class obj_loader
	{
	public:
		obj_loader();

		std::unique_ptr<mesh> load(const std::string& file);
	};
}

#endif
