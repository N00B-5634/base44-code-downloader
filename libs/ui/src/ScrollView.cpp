#include "ScrollView.h"

#include <Message.h>
#include <Window.h>

namespace YellowDocs {
namespace UI {

// ScrollView implementation

ScrollView::ScrollView(const BString& name, BView* target, uint32 resizingMode,
		uint32 flags, bool horizontal, bool vertical, border_style border)
	: BScrollView(name.String(), target, resizingMode, flags, horizontal, vertical, border),
	  fSmoothScrolling(false),
	  fZoom(1.0f),
	  fScrolling(false)
{
}

ScrollView::~ScrollView() {
}

// Scrolling behavior

void ScrollView::SetSmoothScrolling(bool smooth) {
	fSmoothScrolling = smooth;
}

bool ScrollView::SmoothScrolling() const {
	return fSmoothScrolling;
}

// Scroll to position with animation

void ScrollView::ScrollToSmooth(BPoint point, bigtime_t duration) {
	if (!fSmoothScrolling) {
		ScrollTo(point);
		return;
	}
	
	fTargetScroll = point;
	fStartScroll = Target();
	fScrollStartTime = system_time();
	fScrollDuration = duration;
	fScrolling = true;
	
	// Start animation
	// Note: In a real implementation, this would use BMessageRunner or similar
	// For now, we'll just scroll directly
	ScrollTo(point);
	fScrolling = false;
}

// Scroll by amount with animation

void ScrollView::ScrollBySmooth(float dx, float dy, bigtime_t duration) {
	BPoint current = Target();
	ScrollToSmooth(BPoint(current.x + dx, current.y + dy), duration);
}

// Zoom

void ScrollView::SetZoom(float zoom) {
	fZoom = zoom;
	Invalidate();
}

float ScrollView::Zoom() const {
	return fZoom;
}

// Scrollbar visibility

void ScrollView::SetHorizontalScrollBarVisible(bool visible) {
	SetBorder(visible ? B_PLAIN_BORDER : B_NO_BORDER);
}

void ScrollView::SetVerticalScrollBarVisible(bool visible) {
	// Note: BScrollView doesn't have separate control for vertical scrollbar
	// This would require custom implementation
}

// Drawing

void ScrollView::Draw(BRect frame) {
	BScrollView::Draw(frame);
	
	// Custom drawing for scroll view background
	// This can be used to add custom styling
}

// Helper methods

void ScrollView::UpdateScroll() {
	if (!fScrolling) {
		return;
	}
	
	bigtime_t now = system_time();
	bigtime_t elapsed = now - fScrollStartTime;
	
	if (elapsed >= fScrollDuration) {
		ScrollTo(fTargetScroll);
		fScrolling = false;
	} else {
		float ratio = static_cast<float>(elapsed) / fScrollDuration;
		BPoint current;
		current.x = fStartScroll.x + (fTargetScroll.x - fStartScroll.x) * ratio;
		current.y = fStartScroll.y + (fTargetScroll.y - fStartScroll.y) * ratio;
		ScrollTo(current);
	}
}

} // namespace UI
} // namespace YellowDocs
