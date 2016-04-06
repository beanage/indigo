#pragma once

namespace indigo {
class mesh
{
public:
    virtual ~mesh()
    {
    }

    virtual void render() const = 0;
    virtual void upload() = 0;
};
}
