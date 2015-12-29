#ifndef __TEXTTURE_HPP_INCLUDED__
#define __TEXTTURE_HPP_INCLUDED__

#include <string>

namespace indigo
{
	class texture
	{
	public:
		explicit texture(const std::string& file);
		texture(unsigned char* buffer, unsigned len);
		virtual ~texture();

		virtual void bind() const;

	private:
		texture() = delete;
		texture(const texture&) = delete;

		unsigned int obj_;
	};
}

#endif
