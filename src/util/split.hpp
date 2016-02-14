#ifndef __SPLIT_HPP_INCLUDED__
#define __SPLIT_HPP_INCLUDED__

#include <string>
#include <algorithm>

namespace indigo {
template <class T>
std::vector<std::basic_string<T>> split(const std::basic_string<T>& str,
                                        const std::basic_string<T>& sep)
{
    std::vector<std::basic_string<T>> res;

    std::string::size_type old = 0, pos = 0;
    while ((pos = str.find(sep, old)) != std::string::npos) {
        res.push_back(str.substr(old, pos - old));
        old = pos + sep.length();
    }

    if (old < str.length())
        res.push_back(str.substr(old, std::string::npos));

    return res;
}
}

#endif
