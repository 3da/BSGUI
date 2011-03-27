#ifndef FONT_H_INCLUDED
#define FONT_H_INCLUDED

#include  <Drawing.h>

namespace BSGFX
{

class Font
{
public:

	class StringRowParser
	{
	public:
		StringRowParser(Font *font, const wchar_t *string, float scale = 1, unsigned int wrapAreaWidth = 0, unsigned long substrLength = 0);
		//! Resets row parser. Returns true if successful, or false otherwise.
		bool Reset(Font *font, const wchar_t *string, float scale = 1, unsigned int wrapAreaWidth = 0, unsigned long substrLength = 0);
		//! Returns true if there was a row to return. Row info is written to arguments.
		bool GetNextRow(const wchar_t *&oRowStart, unsigned long &oRowLength, unsigned int *oRowWidth);
		//! Returns true if whole string has been parsed, or false otherwise.
		bool Finished() const {
			return mDone;
		}
		//! Returns amount of rows returned so far.
		long GetResultRowCount() const {
			return mResultRowCount;
		}




	protected:
		Font *mFont;
		const wchar_t *mString;
		float mScale;
		unsigned int mWrapAreaWidth;
		long mMaxLength;
		bool mWordWrapping;
		float mSpacing, mSpaceWidth;
		float mResultWidth;
		long mResultRowCount;
		long mTotalChars;
		const wchar_t *mCurrentRowStart, *mCurrentWordStart;
		long mCurrentRowLength, mCurrentWordLength;
		float mCurrentRowWidth;
		bool mDone;
	};

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
		Texture *fontImage;
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

	Font(const char *descriptionFile);
	~Font();

	static Font *Load(const char *descriptionFile);

	// From Font:

	bool Init();
	bool Reload();

	void SetCharacterSpacing(long spacing) {
		mSpacing = spacing;
	}
	long GetCharacterSpacing() {
		return mSpacing;
	}
	long GetFontHeight();
	unsigned int GetFontHeight(float scale);
	bool HasCharacter(unsigned long character);
	unsigned int GetStringWidth(const wchar_t *string, float scale, unsigned long substrLength = 0);

	int DrawString(int x, int y, const wchar_t *string,
				   unsigned long flags = A_TOP_LEFT, float scale = 1, unsigned long substrLength = 0);

	// Local:

	const char *GetFontName() const;




	bool IsLoaded() const {
		return mLoaded;
	}

	long GetStringMetrics(unsigned int &oWidth, unsigned int &oHeight, const wchar_t *string,
						  float scale = 1, unsigned int wrapAreaWidth = 0, unsigned long substrLength = 0);

	long DrawText(int x, int y, const wchar_t *text,
				  unsigned long flags = A_TOP_LEFT, float scale = 1, unsigned int wrapAreaWidth = 0, unsigned long substrLength = 0);

protected:

	bool mLoaded;
	unsigned long mFlags;

	long FindChar(unsigned long ch);
	long FindKern(long first, long second);
	long FindKern(long first, long second, long firstCharIndex);

	long mSpacing;
	char mFileName[MAX_FILENAME_LENGTH];
	FontData mFontData;

}; // class Font



}

#endif // FONT_H_INCLUDED
