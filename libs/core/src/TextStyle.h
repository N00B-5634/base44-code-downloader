#ifndef YELLOWDOCS_CORE_TEXT_STYLE_H
#define YELLOWDOCS_CORE_TEXT_STYLE_H

#include <SupportDefs.h>
#include <String.h>

namespace YellowDocs {
namespace Core {

class TextStyle {
public:
	TextStyle();
	TextStyle(const TextStyle& other);
	TextStyle& operator=(const TextStyle& other);
	~TextStyle();

	// Font properties
	void SetFontFamily(const BString& family);
	BString FontFamily() const;

	void SetFontSize(float size);
	float FontSize() const;

	void SetBold(bool bold);
	bool IsBold() const;

	void SetItalic(bool italic);
	bool IsItalic() const;

	void SetUnderline(bool underline);
	bool IsUnderline() const;

	void SetStrikeout(bool strikeout);
	bool IsStrikeout() const;

	// Color properties
	void SetTextColor(rgb_color color);
	rgb_color TextColor() const;

	void SetBackgroundColor(rgb_color color);
	rgb_color BackgroundColor() const;

	// Effect properties
	void SetSubscript(bool subscript);
	bool IsSubscript() const;

	void SetSuperscript(bool superscript);
	bool IsSuperscript() const;

	void SetSmallCaps(bool smallCaps);
	bool IsSmallCaps() const;

	// Comparison
	bool operator==(const TextStyle& other) const;
	bool operator!=(const TextStyle& other) const;

	// Check if style is default
	bool IsDefault() const;

	// Reset to default
	void Reset();

private:
	BString fFontFamily;
	float fFontSize;
	bool fBold;
	bool fItalic;
	bool fUnderline;
	bool fStrikeout;
	rgb_color fTextColor;
	rgb_color fBackgroundColor;
	bool fSubscript;
	bool fSuperscript;
	bool fSmallCaps;
};

} // namespace Core
} // namespace YellowDocs

#endif // YELLOWDOCS_CORE_TEXT_STYLE_H
