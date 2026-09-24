#include "Colors.h"

#include <algorithm>

namespace YellowDocs {
namespace UI {

// Function to blend colors

rgb_color BlendColors(rgb_color a, rgb_color b, float ratio) {
	if (ratio <= 0.0f) return a;
	if (ratio >= 1.0f) return b;
	
	rgb_color result;
	result.red = static_cast<uint8>(a.red + (b.red - a.red) * ratio);
	result.green = static_cast<uint8>(a.green + (b.green - a.green) * ratio);
	result.blue = static_cast<uint8>(a.blue + (b.blue - a.blue) * ratio);
	result.alpha = static_cast<uint8>(a.alpha + (b.alpha - a.alpha) * ratio);
	
	return result;
}

// Function to lighten color

rgb_color LightenColor(rgb_color color, float factor) {
	if (factor <= 0.0f) return color;
	
	float r = static_cast<float>(color.red) / 255.0f;
	float g = static_cast<float>(color.green) / 255.0f;
	float b = static_cast<float>(color.blue) / 255.0f;
	
	// Lighten using HSL-like approach (simplified)
	r = std::min(r * (1.0f + factor), 1.0f);
	g = std::min(g * (1.0f + factor), 1.0f);
	b = std::min(b * (1.0f + factor), 1.0f);
	
	rgb_color result;
	result.red = static_cast<uint8>(r * 255.0f);
	result.green = static_cast<uint8>(g * 255.0f);
	result.blue = static_cast<uint8>(b * 255.0f);
	result.alpha = color.alpha;
	
	return result;
}

// Function to darken color

rgb_color DarkenColor(rgb_color color, float factor) {
	if (factor <= 0.0f) return color;
	
	float r = static_cast<float>(color.red) / 255.0f;
	float g = static_cast<float>(color.green) / 255.0f;
	float b = static_cast<float>(color.blue) / 255.0f;
	
	// Darken using HSL-like approach (simplified)
	r = std::max(r * (1.0f - factor), 0.0f);
	g = std::max(g * (1.0f - factor), 0.0f);
	b = std::max(b * (1.0f - factor), 0.0f);
	
	rgb_color result;
	result.red = static_cast<uint8>(r * 255.0f);
	result.green = static_cast<uint8>(g * 255.0f);
	result.blue = static_cast<uint8>(b * 255.0f);
	result.alpha = color.alpha;
	
	return result;
}

// Function to get grayscale value

float GetColorBrightness(rgb_color color) {
	// Calculate relative luminance (per ITU-R BT.709)
	return (color.red * 0.2126f + color.green * 0.7152f + color.blue * 0.0722f) / 255.0f;
}

// Function to check if color is light

bool IsLightColor(rgb_color color) {
	return GetColorBrightness(color) > 0.5f;
}

// Function to get contrasting text color

rgb_color GetContrastingTextColor(rgb_color background) {
	// Use black text on light backgrounds, white text on dark backgrounds
	return IsLightColor(background) ? kColorText : kColorTextDark;
}

} // namespace UI
} // namespace YellowDocs
