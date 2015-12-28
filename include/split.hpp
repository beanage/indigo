#ifndef __SPLIT_HPP_INCLUDED__
#define __SPLIT_HPP_INCLUDED__

#include <string>
#include <algorithm>

namespace indigo
{
	template <class T>
	std::vector<std::basic_string<T>> split(const std::basic_string<T>& str, const std::basic_string<T>& sep)
	{
		std::vector<std::basic_string<T>> res;

		std::string::size_type old = 0, pos = 0;
		while ((pos = str.find(sep, old+1)) != std::string::npos) {
			res.push_back(str.substr(old, pos));
			old = pos;
		}

		if (old < str.length()-1)
			res.push_back(str.substr(old, -1));
	
		return res;
	}
}

#endif
