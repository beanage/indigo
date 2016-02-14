#ifndef __BITMAP_LOADER_HPP_INCLUDED__
#define __BITMAP_LOADER_HPP_INCLUDED__

#include "bitmap.hpp"
#include "resource_manager.hpp"

namespace indigo {
class bitmap_loader : public resource_loader<bitmap>
{
public:
    bitmap_loader() = default;

    bool can_load(std::string const& extension) const override;
    std::shared_ptr<bitmap> load(std::string const& filename) override;

private:
    bitmap_loader(bitmap_loader const&) = delete;
};
}

#endif
