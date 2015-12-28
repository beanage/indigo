#ifndef __OBJ_LOADER_HPP_INCLUDED__
#define __OBJ_LOADER_HPP_INCLUDED__

#include <memory>
#include <string>

namespace indigo
{
	class obj_mesh;

	class obj_loader
	{
	public:
		obj_loader();

		std::unique_ptr<obj_mesh> load(const std::string& filename);
	private:
	};
}

#endif
