
#ifndef STDEXT_TRAITS_H
#define STDEXT_TRAITS_H

#include <type_traits>

namespace stdext {

	template<class T> struct replace_extent { typedef T type; };
	template<class T> struct replace_extent<T[]> { typedef const T* type; };
	template<class T, unsigned long N> struct replace_extent<T[N]> { typedef const T* type;};
	template<typename T> struct remove_const_ref { typedef typename std::remove_const<typename std::remove_reference<T>::type>::type type; };

};

#endif
