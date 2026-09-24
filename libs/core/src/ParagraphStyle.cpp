#include "ParagraphStyle.h"

namespace YellowDocs {
namespace Core {

ParagraphStyle::ParagraphStyle()
	: fAlignment(TEXT_ALIGN_LEFT),
	  fLeftIndent(0),
	  fRightIndent(0),
	  fFirstLineIndent(0),
	  fLineSpacing(1.0f),
	  fSpaceBefore(0),
	  fSpaceAfter(0),
	  fListStyle(LIST_STYLE_NONE),
	  fListLevel(0),
	  fHasBorder(false),
	  fBorderColor({0, 0, 0, 255}),
	  fBorderWidth(1.0f),
	  fBackgroundColor({0, 0, 0, 0})
{
}

ParagraphStyle::ParagraphStyle(const ParagraphStyle& other)
	: fAlignment(other.fAlignment),
	  fLeftIndent(other.fLeftIndent),
	  fRightIndent(other.fRightIndent),
	  fFirstLineIndent(other.fFirstLineIndent),
	  fLineSpacing(other.fLineSpacing),
	  fSpaceBefore(other.fSpaceBefore),
	  fSpaceAfter(other.fSpaceAfter),
	  fListStyle(other.fListStyle),
	  fListLevel(other.fListLevel),
	  fHasBorder(other.fHasBorder),
	  fBorderColor(other.fBorderColor),
	  fBorderWidth(other.fBorderWidth),
	  fBackgroundColor(other.fBackgroundColor)
{
}

ParagraphStyle& ParagraphStyle::operator=(const ParagraphStyle& other) {
	if (this != &other) {
		fAlignment = other.fAlignment;
		fLeftIndent = other.fLeftIndent;
		fRightIndent = other.fRightIndent;
		fFirstLineIndent = other.fFirstLineIndent;
		fLineSpacing = other.fLineSpacing;
		fSpaceBefore = other.fSpaceBefore;
		fSpaceAfter = other.fSpaceAfter;
		fListStyle = other.fListStyle;
		fListLevel = other.fListLevel;
		fHasBorder = other.fHasBorder;
		fBorderColor = other.fBorderColor;
		fBorderWidth = other.fBorderWidth;
		fBackgroundColor = other.fBackgroundColor;
	}
	return *this;
}

ParagraphStyle::~ParagraphStyle() {
}

// Alignment

void ParagraphStyle::SetAlignment(TextAlignment alignment) {
	fAlignment = alignment;
}

TextAlignment ParagraphStyle::Alignment() const {
	return fAlignment;
}

// Indentation

void ParagraphStyle::SetLeftIndent(float indent) {
	fLeftIndent = indent;
}

float ParagraphStyle::LeftIndent() const {
	return fLeftIndent;
}

void ParagraphStyle::SetRightIndent(float indent) {
	fRightIndent = indent;
}

float ParagraphStyle::RightIndent() const {
	return fRightIndent;
}

void ParagraphStyle::SetFirstLineIndent(float indent) {
	fFirstLineIndent = indent;
}

float ParagraphStyle::FirstLineIndent() const {
	return fFirstLineIndent;
}

// Spacing

void ParagraphStyle::SetLineSpacing(float spacing) {
	fLineSpacing = spacing;
}

float ParagraphStyle::LineSpacing() const {
	return fLineSpacing;
}

void ParagraphStyle::SetSpaceBefore(float space) {
	fSpaceBefore = space;
}

float ParagraphStyle::SpaceBefore() const {
	return fSpaceBefore;
}

void ParagraphStyle::SetSpaceAfter(float space) {
	fSpaceAfter = space;
}

float ParagraphStyle::SpaceAfter() const {
	return fSpaceAfter;
}

// List properties

void ParagraphStyle::SetListStyle(ListStyle style) {
	fListStyle = style;
}

ListStyle ParagraphStyle::ListStyle() const {
	return fListStyle;
}

void ParagraphStyle::SetListLevel(int level) {
	fListLevel = level;
}

int ParagraphStyle::ListLevel() const {
	return fListLevel;
}

// Borders

void ParagraphStyle::SetHasBorder(bool hasBorder) {
	fHasBorder = hasBorder;
}

bool ParagraphStyle::HasBorder() const {
	return fHasBorder;
}

void ParagraphStyle::SetBorderColor(rgb_color color) {
	fBorderColor = color;
}

rgb_color ParagraphStyle::BorderColor() const {
	return fBorderColor;
}

void ParagraphStyle::SetBorderWidth(float width) {
	fBorderWidth = width;
}

float ParagraphStyle::BorderWidth() const {
	return fBorderWidth;
}

// Background

void ParagraphStyle::SetBackgroundColor(rgb_color color) {
	fBackgroundColor = color;
}

rgb_color ParagraphStyle::BackgroundColor() const {
	return fBackgroundColor;
}

// Comparison

bool ParagraphStyle::operator==(const ParagraphStyle& other) const {
	return fAlignment == other.fAlignment &&
		fLeftIndent == other.fLeftIndent &&
		fRightIndent == other.fRightIndent &&
		fFirstLineIndent == other.fFirstLineIndent &&
		fLineSpacing == other.fLineSpacing &&
		fSpaceBefore == other.fSpaceBefore &&
		fSpaceAfter == other.fSpaceAfter &&
		fListStyle == other.fListStyle &&
		fListLevel == other.fListLevel &&
		fHasBorder == other.fHasBorder &&
		fBorderColor == other.fBorderColor &&
		fBorderWidth == other.fBorderWidth &&
		fBackgroundColor == other.fBackgroundColor;
}

bool ParagraphStyle::operator!=(const ParagraphStyle& other) const {
	return !(*this == other);
}

// Check if style is default

bool ParagraphStyle::IsDefault() const {
	ParagraphStyle defaultStyle;
	return *this == defaultStyle;
}

// Reset to default

void ParagraphStyle::Reset() {
	fAlignment = TEXT_ALIGN_LEFT;
	fLeftIndent = 0;
	fRightIndent = 0;
	fFirstLineIndent = 0;
	fLineSpacing = 1.0f;
	fSpaceBefore = 0;
	fSpaceAfter = 0;
	fListStyle = LIST_STYLE_NONE;
	fListLevel = 0;
	fHasBorder = false;
	fBorderColor = rgb_color{0, 0, 0, 255};
	fBorderWidth = 1.0f;
	fBackgroundColor = rgb_color{0, 0, 0, 0};
}

} // namespace Core
} // namespace YellowDocs
