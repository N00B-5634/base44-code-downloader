#include "TextStyle.h"

#include <new>

namespace YellowDocs {
namespace Core {

TextStyle::TextStyle()
	: fFontSize(12.0f),
	  fBold(false),
	  fItalic(false),
	  fUnderline(false),
	  fStrikeout(false),
	  fTextColor({255, 255, 255, 255}), // White (system default)
	  fBackgroundColor({0, 0, 0, 0}), // Transparent
	  fSubscript(false),
	  fSuperscript(false),
	  fSmallCaps(false)
{
	fFontFamily = "System";
}

TextStyle::TextStyle(const TextStyle& other)
	: fFontFamily(other.fFontFamily),
	  fFontSize(other.fFontSize),
	  fBold(other.fBold),
	  fItalic(other.fItalic),
	  fUnderline(other.fUnderline),
	  fStrikeout(other.fStrikeout),
	  fTextColor(other.fTextColor),
	  fBackgroundColor(other.fBackgroundColor),
	  fSubscript(other.fSubscript),
	  fSuperscript(other.fSuperscript),
	  fSmallCaps(other.fSmallCaps)
{
}

TextStyle& TextStyle::operator=(const TextStyle& other) {
	if (this != &other) {
		fFontFamily = other.fFontFamily;
		fFontSize = other.fFontSize;
		fBold = other.fBold;
		fItalic = other.fItalic;
		fUnderline = other.fUnderline;
		fStrikeout = other.fStrikeout;
		fTextColor = other.fTextColor;
		fBackgroundColor = other.fBackgroundColor;
		fSubscript = other.fSubscript;
		fSuperscript = other.fSuperscript;
		fSmallCaps = other.fSmallCaps;
	}
	return *this;
}

TextStyle::~TextStyle() {
}

// Font properties

void TextStyle::SetFontFamily(const BString& family) {
	fFontFamily = family;
}

BString TextStyle::FontFamily() const {
	return fFontFamily;
}

void TextStyle::SetFontSize(float size) {
	fFontSize = size;
}

float TextStyle::FontSize() const {
	return fFontSize;
}

void TextStyle::SetBold(bool bold) {
	fBold = bold;
}

bool TextStyle::IsBold() const {
	return fBold;
}

void TextStyle::SetItalic(bool italic) {
	fItalic = italic;
}

bool TextStyle::IsItalic() const {
	return fItalic;
}

void TextStyle::SetUnderline(bool underline) {
	fUnderline = underline;
}

bool TextStyle::IsUnderline() const {
	return fUnderline;
}

void TextStyle::SetStrikeout(bool strikeout) {
	fStrikeout = strikeout;
}

bool TextStyle::IsStrikeout() const {
	return fStrikeout;
}

// Color properties

void TextStyle::SetTextColor(rgb_color color) {
	fTextColor = color;
}

rgb_color TextStyle::TextColor() const {
	return fTextColor;
}

void TextStyle::SetBackgroundColor(rgb_color color) {
	fBackgroundColor = color;
}

rgb_color TextStyle::BackgroundColor() const {
	return fBackgroundColor;
}

// Effect properties

void TextStyle::SetSubscript(bool subscript) {
	fSubscript = subscript;
}

bool TextStyle::IsSubscript() const {
	return fSubscript;
}

void TextStyle::SetSuperscript(bool superscript) {
	fSuperscript = superscript;
}

bool TextStyle::IsSuperscript() const {
	return fSuperscript;
}

void TextStyle::SetSmallCaps(bool smallCaps) {
	fSmallCaps = smallCaps;
}

bool TextStyle::IsSmallCaps() const {
	return fSmallCaps;
}

// Comparison

bool TextStyle::operator==(const TextStyle& other) const {
	return fFontFamily == other.fFontFamily &&
		fFontSize == other.fFontSize &&
		fBold == other.fBold &&
		fItalic == other.fItalic &&
		fUnderline == other.fUnderline &&
		fStrikeout == other.fStrikeout &&
		fTextColor == other.fTextColor &&
		fBackgroundColor == other.fBackgroundColor &&
		fSubscript == other.fSubscript &&
		fSuperscript == other.fSuperscript &&
		fSmallCaps == other.fSmallCaps;
}

bool TextStyle::operator!=(const TextStyle& other) const {
	return !(*this == other);
}

// Check if style is default

bool TextStyle::IsDefault() const {
	TextStyle defaultStyle;
	return *this == defaultStyle;
}

// Reset to default

void TextStyle::Reset() {
	fFontFamily = "System";
	fFontSize = 12.0f;
	fBold = false;
	fItalic = false;
	fUnderline = false;
	fStrikeout = false;
	fTextColor = rgb_color{255, 255, 255, 255};
	fBackgroundColor = rgb_color{0, 0, 0, 0};
	fSubscript = false;
	fSuperscript = false;
	fSmallCaps = false;
}

} // namespace Core
} // namespace YellowDocs
