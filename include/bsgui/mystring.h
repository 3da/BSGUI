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
		actual = true;
	}

	MyString(const std::string &text)
	{
		actual = false;
		Set(text);
	}

	MyString(const std::wstring &text)
	{
		actual = false;
		Set(text);
	}

	MyString(const MyString &text)
	{
		actual = false;
		Set(text);
	}

	MyString(const char *text)
	{
		actual = false;
		Set(text);
	}

	MyString(const wchar_t *text)
	{
		actual = false;
		Set(text);
	}


	void Set(const std::string &text);
	void Set(const std::wstring &text);
	//void Set(const MyString &text);


	const char *GetAscii();
	const wchar_t *GetUnicode() const {return c_str();}

	MyString &operator=(const MyString &str) {Set(str); return *this;}


protected:
	std::string ascii;
	bool actual;

};




}


#endif // MYSTRING_H_INCLUDED
