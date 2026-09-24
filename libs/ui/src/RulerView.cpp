#include "RulerView.h"

#include <Font.h>
#include <String.h>
#include <math.h>

#include "Theme.h"

namespace YellowDocs {
namespace UI {

// RulerView implementation

RulerView::RulerView(const BString& name, RulerOrientation orientation)
	: BView(name.String(), B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE),
	  fOrientation(orientation),
	  fScale(1.0f),
	  fUnits("mm"),
	  fOrigin(0),
	  fSize(1000)
{
	SetViewColor(GetTheme().RulerBackgroundColor());
}

RulerView::~RulerView() {
	ClearMarkers();
}

// Orientation

void RulerView::SetOrientation(RulerOrientation orientation) {
	fOrientation = orientation;
	Invalidate();
}

RulerOrientation RulerView::Orientation() const {
	return fOrientation;
}

// Scale

void RulerView::SetScale(float scale) {
	fScale = scale;
	Invalidate();
}

float RulerView::Scale() const {
	return fScale;
}

// Units

void RulerView::SetUnits(const BString& units) {
	fUnits = units;
	Invalidate();
}

BString RulerView::Units() const {
	return fUnits;
}

// Origin

void RulerView::SetOrigin(float origin) {
	fOrigin = origin;
	Invalidate();
}

float RulerView::Origin() const {
	return fOrigin;
}

// Size

void RulerView::SetSize(float size) {
	fSize = size;
	Invalidate();
}

float RulerView::Size() const {
	return fSize;
}

// Markers

void RulerView::AddMarker(float position, const BString& label) {
	fMarkers.Add(position);
	fMarkerLabels.Add(label);
	Invalidate();
}

bool RulerView::RemoveMarker(float position) {
	for (int32 i = 0; i < fMarkers.CountItems(); i++) {
		if (fMarkers.ItemAt(i) == position) {
			fMarkers.RemoveItemAt(i);
			fMarkerLabels.RemoveItemAt(i);
			Invalidate();
			return true;
		}
	}
	return false;
}

void RulerView::ClearMarkers() {
	fMarkers.MakeEmpty();
	fMarkerLabels.MakeEmpty();
	Invalidate();
}

// Drawing

void RulerView::Draw(BRect frame) {
	Theme& theme = GetTheme();
	
	// Draw background
	SetHighColor(theme.RulerBackgroundColor());
	FillRect(frame);
	
	// Draw ruler based on orientation
	if (fOrientation == RULER_HORIZONTAL) {
		DrawHorizontalRuler(frame);
	} else {
		DrawVerticalRuler(frame);
	}
	
	// Draw border
	SetHighColor(theme.BorderColor());
	StrokeRect(frame);
}

// Mouse handling

void RulerView::MouseDown(BPoint point) {
	// Handle marker creation or dragging
	BView::MouseDown(point);
}

void RulerView::MouseUp(BPoint point) {
	BView::MouseUp(point);
}

void RulerView::MouseMoved(BPoint point, uint32 transit, const BMessage* dragMessage) {
	BView::MouseMoved(point, transit, dragMessage);
}

// Size management

BSize RulerView::MinSize() {
	if (fOrientation == RULER_HORIZONTAL) {
		return BSize(100, 20);
	} else {
		return BSize(20, 100);
	}
}

BSize RulerView::MaxSize() {
	if (fOrientation == RULER_HORIZONTAL) {
		return BSize(B_SIZE_UNLIMITED, 20);
	} else {
		return BSize(20, B_SIZE_UNLIMITED);
	}
}

BSize RulerView::PreferredSize() {
	if (fOrientation == RULER_HORIZONTAL) {
		return BSize(600, 20);
	} else {
		return BSize(20, 400);
	}
}

// Helper methods

BString RulerView::FormatValue(float value) const {
	BString result;
	
	// Convert to appropriate units
	if (fUnits == "mm") {
		result << static_cast<int>(value);
	} else if (fUnits == "cm") {
		result << static_cast<int>(value / 10);
	} else if (fUnits == "in") {
		float inches = value / 25.4f;
		result << inches;
	} else {
		result << static_cast<int>(value);
	}
	
	return result;
}

void RulerView::DrawHorizontalRuler(BRect frame) {
	Theme& theme = GetTheme();
	
	// Draw scale marks
	SetHighColor(theme.RulerTextColor());
	
	BFont font;
	GetFont(&font);
	
	font_height fontHeight;
	font.GetHeight(&fontHeight);
	
	float textY = frame.bottom - fontHeight.descent - 2;
	
	// Calculate mark spacing based on scale
	float majorInterval = 50.0f * fScale; // 50mm intervals
	float minorInterval = 10.0f * fScale; // 10mm intervals
	float tinyInterval = 1.0f * fScale;  // 1mm intervals
	
	// Draw major marks
	for (float pos = fOrigin; pos <= fSize; pos += majorInterval) {
		float x = frame.left + (pos - fOrigin) / fScale;
		
		if (x > frame.right) {
			break;
		}
		
		// Draw major mark line
		SetHighColor(theme.RulerMarkColor());
		StrokeLine(BPoint(x, frame.top + 5), BPoint(x, frame.bottom - 5));
		
		// Draw label
		SetHighColor(theme.RulerTextColor());
		BString label = FormatValue(pos);
		float textWidth = font.StringWidth(label.String());
		
		DrawString(label.String(), BPoint(x - textWidth / 2, textY));
	}
	
	// Draw minor marks
	SetHighColor(theme.RulerMarkColor());
	for (float pos = fOrigin; pos <= fSize; pos += minorInterval) {
		float x = frame.left + (pos - fOrigin) / fScale;
		
		if (x > frame.right) {
			break;
		}
		
		// Skip if it's a major mark
		if (fmod(pos, majorInterval) == 0) {
			continue;
		}
		
		StrokeLine(BPoint(x, frame.top + 8), BPoint(x, frame.bottom - 5));
	}
	
	// Draw tiny marks
	for (float pos = fOrigin; pos <= fSize; pos += tinyInterval) {
		float x = frame.left + (pos - fOrigin) / fScale;
		
		if (x > frame.right) {
			break;
		}
		
		// Skip if it's a minor or major mark
		if (fmod(pos, minorInterval) == 0) {
			continue;
		}
		
		StrokeLine(BPoint(x, frame.top + 10), BPoint(x, frame.bottom - 5));
	}
	
	// Draw markers
	for (int32 i = 0; i < fMarkers.CountItems(); i++) {
		float markerPos = fMarkers.ItemAt(i);
		float x = frame.left + (markerPos - fOrigin) / fScale;
		
		if (x >= frame.left && x <= frame.right) {
			// Draw marker triangle
			SetHighColor(theme.AccentColor());
			BPoint points[3];
			points[0] = BPoint(x, frame.top + 2);
			points[1] = BPoint(x - 4, frame.top + 8);
			points[2] = BPoint(x + 4, frame.top + 8);
			FillPolygon(points, 3);
			
			// Draw marker label if available
			BString label = fMarkerLabels.ItemAt(i);
			if (!label.IsEmpty()) {
				SetHighColor(theme.RulerTextColor());
				float textWidth = font.StringWidth(label.String());
				DrawString(label.String(), BPoint(x - textWidth / 2, frame.top + 12));
			}
		}
	}
	
	// Draw origin indicator
	SetHighColor(theme.AccentColor());
	float originX = frame.left + (0 - fOrigin) / fScale;
	if (originX >= frame.left && originX <= frame.right) {
		StrokeLine(BPoint(originX, frame.top + 2), BPoint(originX, frame.bottom - 2));
	}
}

void RulerView::DrawVerticalRuler(BRect frame) {
	Theme& theme = GetTheme();
	
	// Draw scale marks
	SetHighColor(theme.RulerTextColor());
	
	BFont font;
	GetFont(&font);
	
	font_height fontHeight;
	font.GetHeight(&fontHeight);
	
	float textX = frame.right - font.StringWidth("000") - 2;
	
	// Calculate mark spacing based on scale
	float majorInterval = 50.0f * fScale; // 50mm intervals
	float minorInterval = 10.0f * fScale; // 10mm intervals
	float tinyInterval = 1.0f * fScale;  // 1mm intervals
	
	// Draw major marks
	for (float pos = fOrigin; pos <= fSize; pos += majorInterval) {
		float y = frame.top + (pos - fOrigin) / fScale;
		
		if (y > frame.bottom) {
			break;
		}
		
		// Draw major mark line
		SetHighColor(theme.RulerMarkColor());
		StrokeLine(BPoint(frame.left + 5, y), BPoint(frame.right - 5, y));
		
		// Draw label
		SetHighColor(theme.RulerTextColor());
		BString label = FormatValue(pos);
		DrawString(label.String(), BPoint(textX, y + fontHeight.ascent / 2));
	}
	
	// Draw minor marks
	SetHighColor(theme.RulerMarkColor());
	for (float pos = fOrigin; pos <= fSize; pos += minorInterval) {
		float y = frame.top + (pos - fOrigin) / fScale;
		
		if (y > frame.bottom) {
			break;
		}
		
		// Skip if it's a major mark
		if (fmod(pos, majorInterval) == 0) {
			continue;
		}
		
		StrokeLine(BPoint(frame.left + 8, y), BPoint(frame.right - 5, y));
	}
	
	// Draw tiny marks
	for (float pos = fOrigin; pos <= fSize; pos += tinyInterval) {
		float y = frame.top + (pos - fOrigin) / fScale;
		
		if (y > frame.bottom) {
			break;
		}
		
		// Skip if it's a minor or major mark
		if (fmod(pos, minorInterval) == 0) {
			continue;
		}
		
		StrokeLine(BPoint(frame.left + 10, y), BPoint(frame.right - 5, y));
	}
	
	// Draw markers
	for (int32 i = 0; i < fMarkers.CountItems(); i++) {
		float markerPos = fMarkers.ItemAt(i);
		float y = frame.top + (markerPos - fOrigin) / fScale;
		
		if (y >= frame.top && y <= frame.bottom) {
			// Draw marker triangle
			SetHighColor(theme.AccentColor());
			BPoint points[3];
			points[0] = BPoint(frame.left + 2, y);
			points[1] = BPoint(frame.left + 8, y - 4);
			points[2] = BPoint(frame.left + 8, y + 4);
			FillPolygon(points, 3);
			
			// Draw marker label if available
			BString label = fMarkerLabels.ItemAt(i);
			if (!label.IsEmpty()) {
				SetHighColor(theme.RulerTextColor());
				DrawString(label.String(), BPoint(frame.left + 12, y + fontHeight.ascent / 2));
			}
		}
	}
	
	// Draw origin indicator
	SetHighColor(theme.AccentColor());
	float originY = frame.top + (0 - fOrigin) / fScale;
	if (originY >= frame.top && originY <= frame.bottom) {
		StrokeLine(BPoint(frame.left + 2, originY), BPoint(frame.right - 2, originY));
	}
}

} // namespace UI
} // namespace YellowDocs
