#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#include <string>

#ifdef UNICODE
	#define str_t std::wstring
	#define ch_t wchar_t
#else
	#define str_t std::string
	#define ch_t char
#endif

#endif // CONFIG_H_INCLUDED
