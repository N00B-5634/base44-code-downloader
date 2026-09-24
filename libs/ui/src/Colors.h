#ifndef YELLOWDOCS_UI_COLORS_H
#define YELLOWDOCS_UI_COLORS_H

#include <SupportDefs.h>
#include <GraphicsDefs.h>

namespace YellowDocs {
namespace UI {

// Predefined colors for YellowDocs

// Accent colors (warm yellow/amber theme)
const rgb_color kAccentYellow = {255, 191, 0, 255};    // #FFBF00 - Golden yellow
const rgb_color kAccentAmber = {255, 152, 0, 255};     // #FF9800 - Amber
const rgb_color kAccentGold = {255, 215, 0, 255};     // #FFD700 - Gold
const rgb_color kAccentLightYellow = {255, 223, 100, 255}; // #FFDF64
const rgb_color kAccentDarkYellow = {200, 140, 0, 255};  // #C88C00

// UI element colors
const rgb_color kColorText = {0, 0, 0, 255};
const rgb_color kColorTextLight = {50, 50, 50, 255};
const rgb_color kColorTextDark = {200, 200, 200, 255};

const rgb_color kColorBackground = {255, 255, 255, 255};
const rgb_color kColorBackgroundLight = {245, 245, 245, 255};
const rgb_color kColorBackgroundDark = {40, 40, 40, 255};

const rgb_color kColorSelection = {50, 100, 200, 255};
const rgb_color kColorSelectionText = {255, 255, 255, 255};

const rgb_color kColorBorder = {200, 200, 200, 255};
const rgb_color kColorBorderLight = {220, 220, 220, 255};
const rgb_color kColorBorderDark = {80, 80, 80, 255};

const rgb_color kColorHighlight = {255, 255, 0, 128};
const rgb_color kColorShadow = {0, 0, 0, 64};

// Status colors
const rgb_color kColorSuccess = {0, 180, 0, 255};
const rgb_color kColorWarning = {255, 165, 0, 255};
const rgb_color kColorError = {255, 0, 0, 255};
const rgb_color kColorInfo = {0, 100, 255, 255};

// Document colors
const rgb_color kColorDocumentBackground = {255, 255, 255, 255};
const rgb_color kColorDocumentMargin = {240, 240, 240, 255};
const rgb_color kColorDocumentRuler = {230, 230, 230, 255};

// Function to blend colors
rgb_color BlendColors(rgb_color a, rgb_color b, float ratio);

// Function to lighten color
rgb_color LightenColor(rgb_color color, float factor);

// Function to darken color
rgb_color DarkenColor(rgb_color color, float factor);

// Function to get grayscale value
float GetColorBrightness(rgb_color color);

// Function to check if color is light
bool IsLightColor(rgb_color color);

// Function to get contrasting text color
rgb_color GetContrastingTextColor(rgb_color background);

} // namespace UI
} // namespace YellowDocs

#endif // YELLOWDOCS_UI_COLORS_H
