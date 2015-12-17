#ifndef __WINDOW_H_INCLUDED__
#define __WINDOW_H_INCLUDED__

#include <string>
#include <memory>
#include "rect.hpp"

namespace indigo
{
	class window
	{
	public:
		window(const irect& frame);
		virtual ~window();

		std::string title() const;
		window& title(const std::string& t);

		irect frame() const;
		window& frame(const irect& v);

		void swap() const;

	private:
		struct impl;
		std::unique_ptr<impl> impl_;
	};
}

#endif
