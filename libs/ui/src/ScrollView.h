#ifndef YELLOWDOCS_UI_SCROLL_VIEW_H
#define YELLOWDOCS_UI_SCROLL_VIEW_H

#include <ScrollView.h>
#include <View.h>

namespace YellowDocs {
namespace UI {

// Enhanced scroll view with additional features
class ScrollView : public BScrollView {
public:
	ScrollView(const BString& name, BView* target, uint32 resizingMode = B_FOLLOW_ALL,
		uint32 flags = B_WILL_DRAW | B_FRAME_EVENTS, bool horizontal = true, bool vertical = true,
		border_style border = B_PLAIN_BORDER);
	~ScrollView();

	// Scrolling behavior
	void SetSmoothScrolling(bool smooth);
	bool SmoothScrolling() const;

	// Scroll to position with animation
	void ScrollToSmooth(BPoint point, bigtime_t duration = 200000);

	// Scroll by amount with animation
	void ScrollBySmooth(float dx, float dy, bigtime_t duration = 200000);

	// Zoom
	void SetZoom(float zoom);
	float Zoom() const;

	// Scrollbar visibility
	void SetHorizontalScrollBarVisible(bool visible);
	void SetVerticalScrollBarVisible(bool visible);

	// Drawing
	void Draw(BRect frame) override;

private:
	bool fSmoothScrolling;
	float fZoom;
	
	// Animation state
	BPoint fTargetScroll;
	BPoint fStartScroll;
	bigtime_t fScrollStartTime;
	bigtime_t fScrollDuration;
	bool fScrolling;
	
	// Helper methods
	void UpdateScroll();
};

} // namespace UI
} // namespace YellowDocs

#endif // YELLOWDOCS_UI_SCROLL_VIEW_H
