#ifndef FONT_H_INCLUDED
#define FONT_H_INCLUDED

#include "image.h"

namespace BSGUI
{

class Font
{
public:

	class StringRowParser
	{
	public:
		StringRowParser(Font *font, const wchar_t *string, float scale = 1, float wrapAreaWidth = 0, unsigned long substrLength = 0);
		//! Resets row parser. Returns true if successful, or false otherwise.
		bool Reset(Font *font, const wchar_t *string, float scale = 1, float wrapAreaWidth = 0, unsigned long substrLength = 0);
		//! Returns true if there was a row to return. Row info is written to arguments.
		bool GetNextRow(const wchar_t *&oRowStart, unsigned long &oRowLength, float *oRowWidth);
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
		float mScale, mWrapAreaWidth;
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

	enum FONT_FLAG
	{
		FF_HANDLE_BACKSLASH_ESCAPES = 0x00000001,
		FF_DEFAULT_FLAGS = FF_HANDLE_BACKSLASH_ESCAPES
	};

	Font() : mLoaded(false), mFlags(FF_DEFAULT_FLAGS) {}
	virtual ~Font() {}

	//! Sets new flags.
	void SetFlags(unsigned long flags) {
		mFlags = flags;
	}

	//! Returns current flags. \sa FONT_FLAG
	unsigned long GetFlags() const {
		return mFlags;
	}

	//! Does any initialization the object needs before it is ready for use.
	/*!
	 * \return TRUE if successful or FALSE otherwise.
	 */
	virtual bool Init() = 0;

	//! Reloads the font.
	/*!
	 * Sets the internal loaded state to true if loading was successful
	 * and FALSE otherwise.
	 * \return true if successful or false otherwise.
	 */
	virtual bool Reload() = 0;

	//! Returns TRUE if the font is loaded or FALSE otherwise.
	bool IsLoaded() const {
		return mLoaded;
	}

	//! Sets horizontal character spacing.
	virtual void SetCharacterSpacing(long spacing) = 0;
	//! Returns horizontal character spacing.
	virtual long GetCharacterSpacing() = 0;
	//! Returns height of the font.
	virtual long GetFontHeight() = 0;
	//! Returns height of the font with given scale.
	virtual float GetFontHeight(float scale) = 0;
	//! Returns true if font contains the given character.
	virtual bool HasCharacter(unsigned long character) = 0;

	//! Returns width of given (sub-)string in given scale.
	virtual float GetStringWidth(const wchar_t *string, float scale, unsigned long substrLength = 0) = 0;

	//! Finds out metrics of given text (sub-)string in given scale. Returns number of rows in text.
	virtual long GetStringMetrics(float &oWidth, float &oHeight, const wchar_t *string,
								  float scale = 1, float wrapAreaWidth = 0, unsigned long substrLength = 0);

	//! Sets draw mode for glyphs. \sa IMAGE_DRAW_MODE
	/*void SetDrawMode(IMAGE_DRAW_MODE drawMode) {
		mDrawMode = drawMode;
	}
	//! Returns draw mode.
	IMAGE_DRAW_MODE GetDrawMode() const {
		return mDrawMode;
	}*/

	//! Draws a string.
	/*!
	 * \param x X-coordinate of the origin for the string.
	 * \param y Y-coordinate of the origin for the string.
	 * \param string string to draw.
	 * \param flags flags to control how the string is positioned and
	 *              drawn. \sa IF_ALIGN flags in IMAGE_FLAG.
	 * \param scale relative scale to draw with.
	 * \param color color and alpha to use.
	 * \param substrLength length of substring to draw, or 0 to draw whole string.
	 * \return string width.
	 */
	virtual float DrawString(float x, float y, const wchar_t *string,
							 unsigned long flags = IF_ALIGN_TOP_LEFT, float scale = 1, unsigned long substrLength = 0) = 0;

	//! Draws a text block.
	/*! Processes newlines and supports word wrapping to given width.
	 * \param x X-coordinate of the origin for the text.
	 * \param y Y-coordinate of the origin for the text.
	 * \param text text to draw.
	 * \param flags flags to control how text is positioned and drawn. Horizontal flags
	 *              are handled per row and vertical flags for whole block. \sa IF_ALIGN flags in IMAGE_FLAG.
	 * \param scale relative scale to draw with.
	 * \param color color and alpha to use.
	 * \param substrLength length of substring to draw, or 0 to draw whole text.
	 * \return number of rows in drawn text.
	 */
	virtual long DrawText(float x, float y, const wchar_t *text,
						  unsigned long flags = IF_ALIGN_TOP_LEFT, float scale = 1, float wrapAreaWidth = 0, unsigned long substrLength = 0);

protected:

	bool mLoaded;
	unsigned long mFlags;

}; // class Font
}

#endif // FONT_H_INCLUDED
