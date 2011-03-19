#include "bsgui/mystring.h"

#include <locale>
#include <sstream>

namespace BSGUI
{

std::wstring Widen( const std::string& str )
{
	std::wostringstream wstm;
	const std::ctype<wchar_t>& ctfacet = std::use_facet< std::ctype<wchar_t> >( wstm.getloc() );
	for( size_t i=0 ; i<str.size() ; ++i )
		wstm << ctfacet.widen( str[i] ) ;
	return wstm.str() ;
}

std::string Narrow( const std::wstring& str )
{
	std::ostringstream stm;
	const std::ctype<char>& ctfacet = std::use_facet< std::ctype<char> >( stm.getloc() ) ;
	for( size_t i=0 ; i<str.size() ; ++i )
	stm << ctfacet.narrow( str[i], 0 ) ;
	return stm.str() ;
}

void MyString::Set(const std::string &text)
{
	ascii = text;
	assign(Widen(text));
}

void MyString::Set(const std::wstring &text)
{
	assign(text);
}

const char *MyString::GetAscii()
{
    ascii = Narrow(*this);
	return ascii.c_str();
}



}
