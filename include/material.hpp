#ifndef __MATERIAL_HPP_INCLUDED__
#define __MATERIAL_HPP_INCLUDED__

namespace indigo
{
	class material
	{
	public:
		virtual ~material() {}

		virtual void upload() = 0;
		virtual void render() const = 0;
	};
}

#endif
