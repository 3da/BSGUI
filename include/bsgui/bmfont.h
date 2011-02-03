#ifndef BMFONT_H_INCLUDED
#define BMFONT_H_INCLUDED

#include "font.h"

namespace BSGUI
{

class BMFont : public Font
{
public:
	//! Defines BMFont related constants.
	enum BMF_CONSTANTS
	{
		MAX_FIELD_LENGTH = 64,
		MAX_FILENAME_LENGTH = 128,
		PADDING_UP = 0,
		PADDING_RIGHT,
		PADDING_DOWN,
		PADDING_LEFT
	};

	struct Info
	{
		char face[MAX_FIELD_LENGTH];
		char charset[MAX_FIELD_LENGTH];
		unsigned int size;
		int stretchH;
		int padding[4];
		int horizSpacing;
		int vertSpacing;
		int outline;
		char aa;
		bool bold;
		bool italic;
		bool unicode;
		bool smooth;
	};

	struct Common
	{
		unsigned int lineHeight;
		int base;
		int scaleW;
		int scaleH;
		unsigned int pages;
		unsigned char alphaChnl;
		unsigned char redChnl;
		unsigned char greenChnl;
		unsigned char blueChnl;
		bool packed;
	};

	struct Page
	{
		Page();
		~Page();
		unsigned int id;
		char file[MAX_FILENAME_LENGTH];
		Image *fontImage;
	};

	struct Char
	{
		unsigned long id;
		int firstKernIndex;
		unsigned int x, y;
		unsigned int width, height;
		int xoffset, yoffset;
		int xadvance;
		unsigned int page;
		unsigned char chnl;
	};

	struct Kerning
	{
		long first, second;
		int amount;
	};

	struct FontData
	{
		FontData();
		~FontData();
		void Reset();

		Info info;
		Common common;
		Page *pages;
		long kerningCount;
		Kerning *kernings;
		long charCount;
		Char *chars;
	};

	BMFont(const char *descriptionFile);
	virtual ~BMFont();

	// From Font:

	virtual bool Init();
	virtual bool Reload();

	virtual void SetCharacterSpacing(long spacing) {
		mSpacing = spacing;
	}
	virtual long GetCharacterSpacing() {
		return mSpacing;
	}
	virtual long GetFontHeight();
	virtual float GetFontHeight(float scale);
	virtual bool HasCharacter(unsigned long character);
	virtual float GetStringWidth(const wchar_t *string, float scale, unsigned long substrLength = 0);

	virtual float DrawString(float x, float y, const wchar_t *string,
							unsigned long flags = IF_ALIGN_TOP_LEFT, float scale = 1, unsigned long substrLength = 0);

	// Local:

	const char *GetFontName() const;

private:

	long FindChar(unsigned long ch);
	long FindKern(long first, long second);
	long FindKern(long first, long second, long firstCharIndex);

	long mSpacing;
	char mFileName[MAX_FILENAME_LENGTH];
	FontData mFontData;

};
}

#endif // BMFONT_H_INCLUDED
