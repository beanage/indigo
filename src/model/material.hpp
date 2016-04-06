#pragma once

namespace indigo {
class material
{
public:
    virtual ~material()
    {
    }

    virtual void upload() = 0;
    virtual void render() const = 0;
};
}
