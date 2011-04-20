#ifndef MYSTRING_H_INCLUDED
#define MYSTRING_H_INCLUDED

#include <string>

namespace BSGUI
{

class MyString: public std::wstring
{
public:
	MyString()
	{
	}

	MyString(const std::string &text)
	{
		Set(text);
	}

	MyString(const std::wstring &text)
	{
		Set(text);
	}

	MyString(const MyString &text)
	{
		Set(text);
	}

	MyString(const char *text)
	{
		Set(text);
	}

	MyString(const wchar_t *text)
	{
		Set(text);
	}


	void Set(const std::string &text);
	void Set(const std::wstring &text);

	const char *GetAscii();
	const wchar_t *GetUnicode() const {return c_str();}

	MyString &operator=(const MyString &str) {Set(str); return *this;}


protected:
	std::string ascii;

};




}


#endif // MYSTRING_H_INCLUDED
