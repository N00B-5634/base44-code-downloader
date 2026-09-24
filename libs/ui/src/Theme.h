#ifndef YELLOWDOCS_UI_THEME_H
#define YELLOWDOCS_UI_THEME_H

#include <SupportDefs.h>
#include <GraphicsDefs.h>
#include <String.h>

namespace YellowDocs {
namespace UI {

// YellowDocs theme colors
class Theme {
public:
	Theme();
	~Theme();

	// Initialize theme from system settings
	void Initialize();

	// Color scheme
	rgb_color BackgroundColor() const;
	rgb_color TextColor() const;
	rgb_color SelectionColor() const;
	rgb_color SelectionTextColor() const;
	rgb_color BorderColor() const;
	rgb_color AccentColor() const;
	rgb_color AccentLightColor() const;
	rgb_color AccentDarkColor() const;
	rgb_color HighlightColor() const;
	rgb_color ShadowColor() const;
	rgb_color DisabledColor() const;

	// Toolbar colors
	rgb_color ToolbarBackgroundColor() const;
	rgb_color ToolbarTextColor() const;
	rgb_color ToolbarButtonColor() const;
	rgb_color ToolbarButtonHoverColor() const;
	rgb_color ToolbarButtonActiveColor() const;

	// Status bar colors
	rgb_color StatusBarBackgroundColor() const;
	rgb_color StatusBarTextColor() const;

	// Ruler colors
	rgb_color RulerBackgroundColor() const;
	rgb_color RulerTextColor() const;
	rgb_color RulerMarkColor() const;

	// Document view colors
	rgb_color DocumentBackgroundColor() const;
	rgb_color DocumentTextColor() const;
	rgb_color DocumentMarginColor() const;

	// Scrollbar colors
	rgb_color ScrollbarBackgroundColor() const;
	rgb_color ScrollbarThumbColor() const;
	rgb_color ScrollbarThumbHoverColor() const;

	// Get color by name
	rgb_color GetColor(const BString& name) const;

	// Check if dark theme is active
	bool IsDarkTheme() const;

	// Get system color
	rgb_color GetSystemColor(const BString& name) const;

private:
	// Color storage
	rgb_color fBackgroundColor;
	rgb_color fTextColor;
	rgb_color fSelectionColor;
	rgb_color fSelectionTextColor;
	rgb_color fBorderColor;
	rgb_color fAccentColor;
	rgb_color fAccentLightColor;
	rgb_color fAccentDarkColor;
	rgb_color fHighlightColor;
	rgb_color fShadowColor;
	rgb_color fDisabledColor;

	rgb_color fToolbarBackgroundColor;
	rgb_color fToolbarTextColor;
	rgb_color fToolbarButtonColor;
	rgb_color fToolbarButtonHoverColor;
	rgb_color fToolbarButtonActiveColor;

	rgb_color fStatusBarBackgroundColor;
	rgb_color fStatusBarTextColor;

	rgb_color fRulerBackgroundColor;
	rgb_color fRulerTextColor;
	rgb_color fRulerMarkColor;

	rgb_color fDocumentBackgroundColor;
	rgb_color fDocumentTextColor;
	rgb_color fDocumentMarginColor;

	rgb_color fScrollbarBackgroundColor;
	rgb_color fScrollbarThumbColor;
	rgb_color fScrollbarThumbHoverColor;

	bool fDarkTheme;
	bool fInitialized;

	// Initialize colors
	void InitializeColors();
	
	// Update from system theme
	void UpdateFromSystemTheme();
};

// Global theme access
Theme& GetTheme();

} // namespace UI
} // namespace YellowDocs

#endif // YELLOWDOCS_UI_THEME_H
