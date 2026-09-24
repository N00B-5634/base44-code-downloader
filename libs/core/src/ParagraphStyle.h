#ifndef YELLOWDOCS_CORE_PARAGRAPH_STYLE_H
#define YELLOWDOCS_CORE_PARAGRAPH_STYLE_H

#include <SupportDefs.h>
#include "DocumentTypes.h"

namespace YellowDocs {
namespace Core {

class ParagraphStyle {
public:
	ParagraphStyle();
	ParagraphStyle(const ParagraphStyle& other);
	ParagraphStyle& operator=(const ParagraphStyle& other);
	~ParagraphStyle();

	// Alignment
	void SetAlignment(TextAlignment alignment);
	TextAlignment Alignment() const;

	// Indentation
	void SetLeftIndent(float indent);
	float LeftIndent() const;

	void SetRightIndent(float indent);
	float RightIndent() const;

	void SetFirstLineIndent(float indent);
	float FirstLineIndent() const;

	// Spacing
	void SetLineSpacing(float spacing);
	float LineSpacing() const;

	void SetSpaceBefore(float space);
	float SpaceBefore() const;

	void SetSpaceAfter(float space);
	float SpaceAfter() const;

	// List properties
	void SetListStyle(ListStyle style);
	ListStyle ListStyle() const;

	void SetListLevel(int level);
	int ListLevel() const;

	// Borders
	void SetHasBorder(bool hasBorder);
	bool HasBorder() const;

	void SetBorderColor(rgb_color color);
	rgb_color BorderColor() const;

	void SetBorderWidth(float width);
	float BorderWidth() const;

	// Background
	void SetBackgroundColor(rgb_color color);
	rgb_color BackgroundColor() const;

	// Comparison
	bool operator==(const ParagraphStyle& other) const;
	bool operator!=(const ParagraphStyle& other) const;

	// Check if style is default
	bool IsDefault() const;

	// Reset to default
	void Reset();

private:
	TextAlignment fAlignment;
	float fLeftIndent;
	float fRightIndent;
	float fFirstLineIndent;
	float fLineSpacing;
	float fSpaceBefore;
	float fSpaceAfter;
	ListStyle fListStyle;
	int fListLevel;
	bool fHasBorder;
	rgb_color fBorderColor;
	float fBorderWidth;
	rgb_color fBackgroundColor;
};

} // namespace Core
} // namespace YellowDocs

#endif // YELLOWDOCS_CORE_PARAGRAPH_STYLE_H
