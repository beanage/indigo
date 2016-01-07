#ifndef __RESOURCE_MANAGER_HPP_INCLUDED__
#define __RESOURCE_MANAGER_HPP_INCLUDED__

#include <memory>
#include <string>
#include <map>

#include "mesh.hpp"
#include "filesystem.hpp"

namespace indigo
{
template <class Type>
class resource_loader
{
public:
    virtual ~resource_loader() {}

    virtual std::shared_ptr<Type> load(std::istream& stream) = 0;
};

template <class Type>
class resource_manager
{
public:
    virtual ~resource_manager() {}

    std::shared_ptr<mesh> load(std::string const& name)
    {
        if (name.empty())
            return std::shared_ptr<mesh>(nullptr);

        std::string::size_type ext_pos(name.find_last_of('.'));
        if (ext_pos < name.length()-1) {
            std::string obj(name.substr(0, ext_pos));
            std::string ext(name.substr(ext_pos + 1));

            auto result = query_cache(obj);
            if (result.get())
                return result;

            auto loader_iter = loaders_.find(ext);
            if (loader_iter != loaders_.end()) {
                auto path = full_path(name);
                if (path.first) {
                    std::ifstream stream(path.second);
                    result = loader_iter->second->load(stream);
                    if (result.get()) {
                        cache_[obj] = result;
                    }

                    return result;
                }
            }
        }
    }

    void add_search_directory(std::string const& path)
    {
        if (filesystem::is_directory(path))
            search_paths_.push_back(path);
        else
            throw std::runtime_error("[resource_manager::add_search_path] path is not a directory!");
    }

    template <class Loader, class ...Values>
    void instantiate_loader(std::string const& filetype, Values... values)
    {
        loaders_[filetype] = std::unique_ptr<Loader>(new Loader(values...));
    }

    void add_loader(std::string const& filetype, std::unique_ptr<resource_loader<Type>>&& loader)
    {
        loaders_[filetype] = std::move(loader);
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
        if (cache_iter != cache_.end()) {
            return cache_iter->second;
        }

        return {nullptr};
    }

    std::vector<std::string> search_paths_;
    std::map<std::string, std::unique_ptr<resource_loader<Type>>> loaders_;
    std::map<std::string, std::shared_ptr<Type>> cache_;
};
}

#endif
