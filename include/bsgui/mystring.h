#ifndef MYSTRING_H_INCLUDED
#define MYSTRING_H_INCLUDED

#include <string>

namespace BSGUI
{

class MyString: public std::wstring
{
public:
	MyString(){};
	MyString(const char* text);
	MyString(const wchar_t* text);

	void Set(const std::string &text);
	void Set(const std::wstring &text);

	const char *GetAscii();
	const wchar_t *GetUnicode() const;

	MyString &operator=(const MyString &str) {Set(str.c_str()); return *this;}


protected:
	std::string ascii;

};




}


#endif // MYSTRING_H_INCLUDED
