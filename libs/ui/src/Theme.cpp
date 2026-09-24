#include "Theme.h"

#include <InterfaceDefs.h>
#include <Screen.h>
#include <View.h>

namespace YellowDocs {
namespace UI {

// YellowDocs accent color (warm yellow/amber)
const rgb_color kYellowDocsAccentColor = {
	255, 191, 0, 255  // RGB: #FFBF00 (Golden yellow)
};

const rgb_color kYellowDocsAccentLightColor = {
	255, 215, 100, 255  // RGB: #FFD764 (Light yellow)
};

const rgb_color kYellowDocsAccentDarkColor = {
	200, 140, 0, 255  // RGB: #C88C00 (Dark yellow/amber)
};

// Static theme instance
static Theme* sTheme = nullptr;

Theme& GetTheme() {
	if (!sTheme) {
		sTheme = new Theme();
		sTheme->Initialize();
	}
	return *sTheme;
}

Theme::Theme()
	: fDarkTheme(false),
	  fInitialized(false)
{
	// Initialize with default colors
	fBackgroundColor = {240, 240, 240, 255};
	fTextColor = {0, 0, 0, 255};
	fSelectionColor = {50, 100, 200, 255};
	fSelectionTextColor = {255, 255, 255, 255};
	fBorderColor = {150, 150, 150, 255};
	fAccentColor = kYellowDocsAccentColor;
	fAccentLightColor = kYellowDocsAccentLightColor;
	fAccentDarkColor = kYellowDocsAccentDarkColor;
	fHighlightColor = {255, 255, 0, 255};
	fShadowColor = {100, 100, 100, 128};
	fDisabledColor = {180, 180, 180, 255};

	fToolbarBackgroundColor = {230, 230, 230, 255};
	fToolbarTextColor = {0, 0, 0, 255};
	fToolbarButtonColor = {240, 240, 240, 255};
	fToolbarButtonHoverColor = {200, 200, 200, 255};
	fToolbarButtonActiveColor = {180, 180, 180, 255};

	fStatusBarBackgroundColor = {220, 220, 220, 255};
	fStatusBarTextColor = {0, 0, 0, 255};

	fRulerBackgroundColor = {245, 245, 245, 255};
	fRulerTextColor = {0, 0, 0, 255};
	fRulerMarkColor = {0, 0, 0, 255};

	fDocumentBackgroundColor = {255, 255, 255, 255};
	fDocumentTextColor = {0, 0, 0, 255};
	fDocumentMarginColor = {240, 240, 240, 255};

	fScrollbarBackgroundColor = {230, 230, 230, 255};
	fScrollbarThumbColor = {180, 180, 180, 255};
	fScrollbarThumbHoverColor = {150, 150, 150, 255};
}

Theme::~Theme() {
}

// Initialize theme from system settings

void Theme::Initialize() {
	if (fInitialized) {
		return;
	}
	
	UpdateFromSystemTheme();
	fInitialized = true;
}

// Color accessors

rgb_color Theme::BackgroundColor() const {
	return fBackgroundColor;
}

rgb_color Theme::TextColor() const {
	return fTextColor;
}

rgb_color Theme::SelectionColor() const {
	return fSelectionColor;
}

rgb_color Theme::SelectionTextColor() const {
	return fSelectionTextColor;
}

rgb_color Theme::BorderColor() const {
	return fBorderColor;
}

rgb_color Theme::AccentColor() const {
	return fAccentColor;
}

rgb_color Theme::AccentLightColor() const {
	return fAccentLightColor;
}

rgb_color Theme::AccentDarkColor() const {
	return fAccentDarkColor;
}

rgb_color Theme::HighlightColor() const {
	return fHighlightColor;
}

rgb_color Theme::ShadowColor() const {
	return fShadowColor;
}

rgb_color Theme::DisabledColor() const {
	return fDisabledColor;
}

rgb_color Theme::ToolbarBackgroundColor() const {
	return fToolbarBackgroundColor;
}

rgb_color Theme::ToolbarTextColor() const {
	return fToolbarTextColor;
}

rgb_color Theme::ToolbarButtonColor() const {
	return fToolbarButtonColor;
}

rgb_color Theme::ToolbarButtonHoverColor() const {
	return fToolbarButtonHoverColor;
}

rgb_color Theme::ToolbarButtonActiveColor() const {
	return fToolbarButtonActiveColor;
}

rgb_color Theme::StatusBarBackgroundColor() const {
	return fStatusBarBackgroundColor;
}

rgb_color Theme::StatusBarTextColor() const {
	return fStatusBarTextColor;
}

rgb_color Theme::RulerBackgroundColor() const {
	return fRulerBackgroundColor;
}

rgb_color Theme::RulerTextColor() const {
	return fRulerTextColor;
}

rgb_color Theme::RulerMarkColor() const {
	return fRulerMarkColor;
}

rgb_color Theme::DocumentBackgroundColor() const {
	return fDocumentBackgroundColor;
}

rgb_color Theme::DocumentTextColor() const {
	return fDocumentTextColor;
}

rgb_color Theme::DocumentMarginColor() const {
	return fDocumentMarginColor;
}

rgb_color Theme::ScrollbarBackgroundColor() const {
	return fScrollbarBackgroundColor;
}

rgb_color Theme::ScrollbarThumbColor() const {
	return fScrollbarThumbColor;
}

rgb_color Theme::ScrollbarThumbHoverColor() const {
	return fScrollbarThumbHoverColor;
}

// Get color by name

rgb_color Theme::GetColor(const BString& name) const {
	if (name == "background") return fBackgroundColor;
	if (name == "text") return fTextColor;
	if (name == "selection") return fSelectionColor;
	if (name == "selection_text") return fSelectionTextColor;
	if (name == "border") return fBorderColor;
	if (name == "accent") return fAccentColor;
	if (name == "accent_light") return fAccentLightColor;
	if (name == "accent_dark") return fAccentDarkColor;
	if (name == "highlight") return fHighlightColor;
	if (name == "shadow") return fShadowColor;
	if (name == "disabled") return fDisabledColor;
	
	if (name == "toolbar_background") return fToolbarBackgroundColor;
	if (name == "toolbar_text") return fToolbarTextColor;
	if (name == "toolbar_button") return fToolbarButtonColor;
	if (name == "toolbar_button_hover") return fToolbarButtonHoverColor;
	if (name == "toolbar_button_active") return fToolbarButtonActiveColor;
	
	if (name == "statusbar_background") return fStatusBarBackgroundColor;
	if (name == "statusbar_text") return fStatusBarTextColor;
	
	if (name == "ruler_background") return fRulerBackgroundColor;
	if (name == "ruler_text") return fRulerTextColor;
	if (name == "ruler_mark") return fRulerMarkColor;
	
	if (name == "document_background") return fDocumentBackgroundColor;
	if (name == "document_text") return fDocumentTextColor;
	if (name == "document_margin") return fDocumentMarginColor;
	
	if (name == "scrollbar_background") return fScrollbarBackgroundColor;
	if (name == "scrollbar_thumb") return fScrollbarThumbColor;
	if (name == "scrollbar_thumb_hover") return fScrollbarThumbHoverColor;
	
	// Default to background color
	return fBackgroundColor;
}

// Check if dark theme is active

bool Theme::IsDarkTheme() const {
	return fDarkTheme;
}

// Get system color

rgb_color Theme::GetSystemColor(const BString& name) const {
	// Get color from system UI settings
	// This is a simplified implementation
	
	if (name == "panel_background") {
		return ui_color(B_PANEL_BACKGROUND_COLOR);
	}
	if (name == "panel_text") {
		return ui_color(B_PANEL_TEXT_COLOR);
	}
	if (name == "control_background") {
		return ui_color(B_CONTROL_BACKGROUND_COLOR);
	}
	if (name == "control_text") {
		return ui_color(B_CONTROL_TEXT_COLOR);
	}
	if (name == "control_border") {
		return ui_color(B_CONTROL_BORDER_COLOR);
	}
	if (name == "control_highlight") {
		return ui_color(B_CONTROL_HIGHLIGHT_COLOR);
	}
	if (name == "control_shadow") {
		return ui_color(B_CONTROL_SHADOW_COLOR);
	}
	if (name == "disabled_control_text") {
		return ui_color(B_DISABLED_CONTROL_TEXT_COLOR);
	}
	if (name == "document_background") {
		return ui_color(B_DOCUMENT_BACKGROUND_COLOR);
	}
	if (name == "document_text") {
		return ui_color(B_DOCUMENT_TEXT_COLOR);
	}
	if (name == "tooltip_background") {
		return ui_color(B_TOOLTIP_BACKGROUND_COLOR);
	}
	if (name == "tooltip_text") {
		return ui_color(B_TOOLTIP_TEXT_COLOR);
	}
	if (name == "menu_background") {
		return ui_color(B_MENU_BACKGROUND_COLOR);
	}
	if (name == "menu_text") {
		return ui_color(B_MENU_TEXT_COLOR);
	}
	if (name == "menu_selected_background") {
		return ui_color(B_MENU_SELECTED_BACKGROUND_COLOR);
	}
	if (name == "menu_selected_text") {
		return ui_color(B_MENU_SELECTED_TEXT_COLOR);
	}
	
	return {255, 255, 255, 255};
}

// Initialize colors

void Theme::InitializeColors() {
	// Get system colors and adapt them
	fBackgroundColor = GetSystemColor("panel_background");
	fTextColor = GetSystemColor("panel_text");
	
	// Determine if dark theme
	// Simple heuristic: if background is dark, it's a dark theme
	float brightness = (fBackgroundColor.red * 0.299f + 
		fBackgroundColor.green * 0.587f + 
		fBackgroundColor.blue * 0.114f) / 255.0f;
	
	fDarkTheme = brightness < 0.5f;
	
	// Adjust colors based on theme
	if (fDarkTheme) {
		// Dark theme adjustments
		fSelectionColor = {100, 150, 255, 255};
		fSelectionTextColor = {255, 255, 255, 255};
		fBorderColor = {80, 80, 80, 255};
		fHighlightColor = {255, 200, 0, 255};
		fShadowColor = {0, 0, 0, 128};
		fDisabledColor = {120, 120, 120, 255};
		
		fToolbarBackgroundColor = {60, 60, 60, 255};
		fToolbarTextColor = {220, 220, 220, 255};
		fToolbarButtonColor = {70, 70, 70, 255};
		fToolbarButtonHoverColor = {90, 90, 90, 255};
		fToolbarButtonActiveColor = {110, 110, 110, 255};
		
		fStatusBarBackgroundColor = {50, 50, 50, 255};
		fStatusBarTextColor = {200, 200, 200, 255};
		
		fRulerBackgroundColor = {55, 55, 55, 255};
		fRulerTextColor = {200, 200, 200, 255};
		fRulerMarkColor = {150, 150, 150, 255};
		
		fDocumentBackgroundColor = {40, 40, 40, 255};
		fDocumentTextColor = {220, 220, 220, 255};
		fDocumentMarginColor = {50, 50, 50, 255};
		
		fScrollbarBackgroundColor = {60, 60, 60, 255};
		fScrollbarThumbColor = {100, 100, 100, 255};
		fScrollbarThumbHoverColor = {120, 120, 120, 255};
	} else {
		// Light theme adjustments
		fSelectionColor = {50, 100, 200, 255};
		fSelectionTextColor = {255, 255, 255, 255};
		fBorderColor = {150, 150, 150, 255};
		fHighlightColor = {255, 255, 0, 255};
		fShadowColor = {100, 100, 100, 128};
		fDisabledColor = {180, 180, 180, 255};
		
		fToolbarBackgroundColor = {230, 230, 230, 255};
		fToolbarTextColor = {0, 0, 0, 255};
		fToolbarButtonColor = {240, 240, 240, 255};
		fToolbarButtonHoverColor = {200, 200, 200, 255};
		fToolbarButtonActiveColor = {180, 180, 180, 255};
		
		fStatusBarBackgroundColor = {220, 220, 220, 255};
		fStatusBarTextColor = {0, 0, 0, 255};
		
		fRulerBackgroundColor = {245, 245, 245, 255};
		fRulerTextColor = {0, 0, 0, 255};
		fRulerMarkColor = {0, 0, 0, 255};
		
		fDocumentBackgroundColor = {255, 255, 255, 255};
		fDocumentTextColor = {0, 0, 0, 255};
		fDocumentMarginColor = {240, 240, 240, 255};
		
		fScrollbarBackgroundColor = {230, 230, 230, 255};
		fScrollbarThumbColor = {180, 180, 180, 255};
		fScrollbarThumbHoverColor = {150, 150, 150, 255};
	}
}

// Update from system theme

void Theme::UpdateFromSystemTheme() {
	InitializeColors();
}

} // namespace UI
} // namespace YellowDocs
