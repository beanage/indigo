#pragma once

#include <memory>
#include <string>
#include <map>
#include <algorithm>
#include <vector>

#include "platform/filesystem.hpp"

namespace indigo {
template <class Type>
class resource_loader
{
public:
    virtual ~resource_loader()
    {
    }

    virtual bool can_load(std::string const& extension) const = 0;
    virtual std::shared_ptr<Type> load(std::string const& filename) = 0;
};

template <class Type>
class resource_manager
{
public:
    static resource_manager<Type>& shared()
    {
        static resource_manager<Type> instance;
        return instance;
    }

    virtual ~resource_manager()
    {
    }

    std::shared_ptr<Type> load(std::string const& name)
    {
        if (name.empty())
            return {nullptr};

        auto name_parts = split_name(name);
        auto result = query_cache(name_parts.first);
        if (result.get())
            return result;

        return load_and_cache(name_parts.first, name_parts.second);
    }

    void unload(std::string const& name)
    {
        if (name.empty())
            return;

        auto name_parts = split_name(name);
        auto cache_iter = cache_.find(name_parts.first);
        if (cache_iter != cache_.end()) {
            std::shared_ptr<Type> resource(nullptr);
            cache_iter->second.swap(resource);
        }
    }

    void add_path(std::string const& path)
    {
        if (filesystem::is_directory(path))
            search_paths_.push_back(path);
        else
            throw std::runtime_error(
                "[resource_manager::add_path] path is not a directory!");
    }

    void add_loader(std::unique_ptr<resource_loader<Type>>&& loader)
    {
        loaders_.push_back(std::move(loader));
    }

private:
    std::pair<bool, std::string> full_path(std::string const& filename)
    {
        for (auto const& prefix : search_paths_) {
            std::string path = filesystem::join(prefix, filename);

            if (filesystem::is_file(path))
                return {true, path};
        }

        return {false, ""};
    }

    std::shared_ptr<Type> query_cache(std::string const& name)
    {
        auto cache_iter = cache_.find(name);
        if (cache_iter != cache_.end() && cache_iter->second.get()) {
            return cache_iter->second;
        }

        return {nullptr};
    }

    std::shared_ptr<Type> load_and_cache(std::string const& obj,
                                         std::string const& ext)
    {
        auto loader_iter =
            std::find_if(loaders_.begin(), loaders_.end(),
                         [&](std::unique_ptr<resource_loader<Type>> const& loader) {
                             return loader->can_load(ext);
                         });

        if (loader_iter != loaders_.end()) {
            auto path = full_path(obj + "." + ext);
            if (path.first) {
                auto result = (*loader_iter)->load(path.second);
                if (result.get()) {
                    cache_[obj] = result;
                }

                return result;
            }
        }
    }

    std::pair<std::string, std::string> split_name(std::string const& name)
    {
        std::string::size_type ext_pos(name.find_last_of('.'));
        if (ext_pos < name.length() - 1) {
            return std::make_pair(name.substr(0, ext_pos), name.substr(ext_pos + 1));
        }

        return std::make_pair(name, name);
    }

    std::vector<std::string> search_paths_;
    std::vector<std::unique_ptr<resource_loader<Type>>> loaders_;
    std::map<std::string, std::shared_ptr<Type>> cache_;
};
}
