#include "filesystem.hpp"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

bool indigo::filesystem::is_directory(const std::string& path)
{
    struct stat s = {0};
    if (stat(path.c_str(), &s) == 0)
        return S_ISDIR(s.st_mode);

    return false;
}

bool indigo::filesystem::is_file(const std::string& path)
{
    struct stat s = {0};
    if (stat(path.c_str(), &s) == 0)
        return S_ISREG(s.st_mode);

    return false;
}

std::string indigo::filesystem::join(const std::string& first,
                                     const std::string& second)
{
    return first + "/" + second;
}
