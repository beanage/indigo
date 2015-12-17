#ifndef __RECT_H_INCLUDED__
#define __RECT_H_INCLUDED__

#include <utility>
#include <ostream>

namespace indigo
{
	template <typename T>
	struct basic_rect
	{
		using type = T;

		T left;
		T top;
		T right;
		T bottom;

		basic_rect()
			: left{}, top{}, right{}, bottom{}
		{}

		basic_rect(T l, T t, T r, T b)
			: left{l}, top{t}, right{r}, bottom{b}
		{}

		basic_rect(const basic_rect<T>& src)
			: left(src.left), top(src.top), right(src.right), bottom(src.bottom)
		{}

		T width() const
		{
			return right - left;
		}

		T height() const
		{
			return bottom - top;
		}

		void normalize()
		{
			if (width() < 0)
				swap(top, bottom);
			if (height() < 0)
				swap(left, right);
		}

		basic_rect<T>& operator=(const basic_rect<T>& r)
		{
			basic_rect<T> copy(r);
			swap(*this, r);
			
			return *this;
		}

		bool operator==(const basic_rect<T>& r) const
		{
			return top == r.top &&
			       left == r.left &&
			       bottom == r.bottom &&
			       right == r.right;
		} 

		bool operator!=(const basic_rect<T>& r) const
		{
			return !this->operator==(r);
		}
	};

	template<typename T>
	void swap(basic_rect<T>& l, basic_rect<T>& r)
	{
		using std::swap;
		
		swap(l.left, r.left);
		swap(l.top, r.top);
		swap(l.right, r.right);
		swap(l.bottom, r.bottom);
	}

	template<typename T>
	std::ostream& operator<<(std::ostream& out, const basic_rect<T>& r)
	{
		return out << "{{" << r.left << ", " << r.top << "}, {" << r.right << ", " << r.bottom << "}}";
	}

	template<typename T>
	basic_rect<T> centered_rect(typename basic_rect<T>::type bounds_w, typename basic_rect<T>::type bounds_h, const basic_rect<T>& rect)
	{
		typename basic_rect<T>::type w_diff = bounds_w - rect.width(), h_diff = bounds_h - rect.height();
		typename basic_rect<T>::type w_off = w_diff / static_cast<T>(2), h_off = h_diff / static_cast<T>(2);
		
		return {w_off, h_off, w_off + rect.width(), h_off + rect.height()};
	}

	using rect = basic_rect<float>;
	using irect = basic_rect<int>;
}

#endif
