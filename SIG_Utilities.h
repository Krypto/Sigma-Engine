#ifndef SIGMA_UTILITIES
#define SIGMA_UTILITIES

#include "EscapeColors.h"

#include <iostream>
#include <sstream>
#include <string>
#include <initializer_list>

#define SIG_FREE(x) 		if (x != 0) { delete x; x = 0; }
#define SIG_LOG(msg) 		std::cout << COLOR_WHITE_NORMAL << msg << COLOR_NORMAL << std::endl;
#define SIG_LOG_DETAIL(msg) SIG_LOG(COLOR_YELLOW_NORMAL << "(" << __FILE__ << ":" << __LINE__ << "): " << COLOR_WHITE_NORMAL << msg)
#define SIG_LOG_ERROR(msg) 	SIG_LOG(COLOR_RED_NORMAL << "ERROR: " << COLOR_WHITE_NORMAL << msg)
#define SIG_DIE(errcode) 	exit(errcode);
#define SIG_DUMP(vec) {\
	for (auto it = vec.begin(); it != vec.end(); ++it) {\
		std::cout << *it << " ";\
	}\
	std::cout << std::endl;\
}
#define SIG_DUMP_P(p, n) {\
	for (int i = 0; i < n; i++) {\
		std::cout << p[i] << " ";\
	}\
	std::cout << std::endl;\
}

#define SIG_ISINSTANCE(x, y) dynamic_cast<x>(y) != 0

#define PI 3.141592654
#define ToRadians(x) x * (PI / 180)
#define ToDegrees(x) x * (180 / PI)

///< Error Codes
#define SIG_ERR_SDL_NOT_INITIALIZED 		0x000001
#define SIG_ERR_VIDEO_MODE_ERROR		 	0x000002
#define SIG_ERR_UNKNOWN 					0xFFFFFF

///< Other Codes
#define SIG_STARTED 			0x11
#define SIG_ERROR_NULL_WINDOW 	0x12
#define SIG_ERROR_NULL		 	0x12
#define SIG_FINISHED 			0xFF

#define SIG_BUFFER_OFFSET(t, i) ((void*)(offsetof(t, i)))

#define SIG_CLAMPF(x) x > 1.0f ? 1.0f : x < 0.0f ? 0.0f : x;
#define SIG_CLAMPF_R(x, _min, _max) x > _max ? _max : x < _min ? _min : x;

#define SSTR(x) dynamic_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()
#define STR(x) #x

// the class _init_list_with_square_brackets provides [] for initializer_list
template<class T>
struct _init_list_with_square_brackets {
	const std::initializer_list<T>& list;
	_init_list_with_square_brackets(const std::initializer_list<T>& _list): list(_list) {}
	T operator[](unsigned int index) {
		return *(list.begin() + index);
	}
};

// a function, with the short name _ (underscore) for creating
// the _init_list_with_square_brackets out of a "regular" std::initializer_list
template<class T>
_init_list_with_square_brackets<T> _(const std::initializer_list<T>& list)
{
	return _init_list_with_square_brackets<T>(list);
}

template<class V, typename T>
bool in(const V &v, const T &el)
{
	return std::find(v.begin(), v.end(), el) != v.end();
}

template<typename T>
T ToNumber(const std::string& nstr)
{
	T valor;

	std::stringstream stream(nstr);
	stream >> valor;
	if (stream.fail()) {
		return 0;
	}
	return valor;
}

template<typename T>
std::string ToString(const T& t) {
	std::stringstream strm;
	strm << t;
	return strm.str();
}

template<typename T>
T* Offset(T* p, int offset) {
	unsigned char* pt = reinterpret_cast<unsigned char*>(p);
	pt += offset;
	return reinterpret_cast<T*>(pt);
}

typedef unsigned char Byte;
typedef unsigned int u32;

#endif
