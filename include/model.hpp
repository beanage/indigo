#ifndef __MODEL_HPP_INCLUDED__
#define __MODEL_HPP_INCLUDED__

#include <vector>

namespace indigo
{
class bone;
class mesh;

class model
{
public:
	virtual ~model() {}

	virtual std::vector<bone *> bones() = 0;
	// virtual std::vector<bone const *> bones() const; // TODO: add this!
	
    virtual std::vector<mesh *> meshes() = 0;
	// virtual std::vector<mesh const *> meshes() const = 0;
	
	virtual void render() const = 0;
    virtual void upload() = 0;
};
}

#endif
