#include "SplitView.h"

#include <Message.h>
#include <Window.h>

#include "Theme.h"

namespace YellowDocs {
namespace UI {

// SplitViewDivider implementation

SplitViewDivider::SplitViewDivider()
	: fPosition(0.5f),
	  fSize(8.0f),
	  fCollapsible(false),
	  fCollapsed(false)
{
}

SplitViewDivider::~SplitViewDivider() {
}

void SplitViewDivider::SetPosition(float position) {
	fPosition = position;
}

float SplitViewDivider::Position() const {
	return fPosition;
}

void SplitViewDivider::SetSize(float size) {
	fSize = size;
}

float SplitViewDivider::Size() const {
	return fSize;
}

void SplitViewDivider::SetCollapsible(bool collapsible) {
	fCollapsible = collapsible;
}

bool SplitViewDivider::IsCollapsible() const {
	return fCollapsible;
}

void SplitViewDivider::SetCollapsed(bool collapsed) {
	fCollapsed = collapsed;
}

bool SplitViewDivider::IsCollapsed() const {
	return fCollapsed;
}

// SplitView implementation

SplitView::SplitView(const BString& name, SplitViewOrientation orientation)
	: BView(name.String(), B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE | B_FRAME_EVENTS),
	  fOrientation(orientation),
	  fDividerSize(8.0f),
	  fDraggingDividerIndex(-1),
	  fDraggingDivider(false)
{
	SetViewColor(GetTheme().BackgroundColor());
}

SplitView::~SplitView() {
	Clear();
}

// Orientation

void SplitView::SetOrientation(SplitViewOrientation orientation) {
	fOrientation = orientation;
	Layout();
}

SplitViewOrientation SplitView::Orientation() const {
	return fOrientation;
}

// View management

bool SplitView::AddView(BView* view, float size) {
	if (!view) {
		return false;
	}
	
	fViews.Add(view);
	fViewSizes.Add(size);
	
	// Add view to hierarchy
	AddChild(view);
	
	Layout();
	return true;
}

bool SplitView::RemoveView(BView* view) {
	for (int32 i = 0; i < fViews.CountItems(); i++) {
		if (fViews.ItemAt(i) == view) {
			RemoveChild(view);
			fViews.RemoveItemAt(i);
			fViewSizes.RemoveItemAt(i);
			Layout();
			return true;
		}
	}
	return false;
}

bool SplitView::RemoveView(int32 index) {
	if (index >= 0 && index < fViews.CountItems()) {
		BView* view = fViews.RemoveItemAt(index);
		fViewSizes.RemoveItemAt(index);
		RemoveChild(view);
		Layout();
		return true;
	}
	return false;
}

int32 SplitView::CountViews() const {
	return fViews.CountItems();
}

BView* SplitView::ViewAt(int32 index) const {
	if (index >= 0 && index < fViews.CountItems()) {
		return fViews.ItemAt(index);
	}
	return nullptr;
}

void SplitView::Clear() {
	for (int32 i = 0; i < fViews.CountItems(); i++) {
		RemoveChild(fViews.ItemAt(i));
	}
	fViews.MakeEmpty();
	fViewSizes.MakeEmpty();
	Layout();
}

// Sizes

void SplitView::SetViewSize(int32 index, float size) {
	if (index >= 0 && index < fViewSizes.CountItems()) {
		fViewSizes.ReplaceItemAt(index, size);
		Layout();
	}
}

float SplitView::ViewSize(int32 index) const {
	if (index >= 0 && index < fViewSizes.CountItems()) {
		return fViewSizes.ItemAt(index);
	}
	return 0;
}

void SplitView::SetDividerSize(float size) {
	fDividerSize = size;
	Layout();
}

float SplitView::DividerSize() const {
	return fDividerSize;
}

// Layout

void SplitView::Layout() {
	UpdateViewFrames();
}

// Drawing

void SplitView::Draw(BRect frame) {
	Theme& theme = GetTheme();
	
	// Draw background
	SetHighColor(theme.BackgroundColor());
	FillRect(frame);
	
	// Draw dividers
	if (fOrientation == SPLIT_HORIZONTAL) {
		// Horizontal split: vertical dividers
		float x = frame.left;
		
		for (int32 i = 0; i < fViews.CountItems() - 1; i++) {
			float viewWidth = frame.Width() * fViewSizes.ItemAt(i);
			x += viewWidth;
			
			// Draw divider
			BRect dividerRect(x, frame.top, x + fDividerSize, frame.bottom);
			SetHighColor(theme.BorderColor());
			FillRect(dividerRect);
			
			x += fDividerSize;
		}
	} else {
		// Vertical split: horizontal dividers
		float y = frame.top;
		
		for (int32 i = 0; i < fViews.CountItems() - 1; i++) {
			float viewHeight = frame.Height() * fViewSizes.ItemAt(i);
			y += viewHeight;
			
			// Draw divider
			BRect dividerRect(frame.left, y, frame.right, y + fDividerSize);
			SetHighColor(theme.BorderColor());
			FillRect(dividerRect);
			
			y += fDividerSize;
		}
	}
}

// Mouse handling

void SplitView::MouseDown(BPoint point) {
	int32 dividerIndex = FindDividerAt(point);
	
	if (dividerIndex >= 0) {
		fDraggingDividerIndex = dividerIndex;
		fDraggingDivider = true;
		fDragStartPoint = point;
		fDragStartFrame = Frame();
		
		// Capture mouse
		Window()->Activate(true);
	}
}

void SplitView::MouseUp(BPoint point) {
	if (fDraggingDivider) {
		fDraggingDivider = false;
		fDraggingDividerIndex = -1;
		Layout();
	}
}

void SplitView::MouseMoved(BPoint point, uint32 transit, const BMessage* dragMessage) {
	if (fDraggingDivider) {
		// Calculate new divider position
		BRect frame = Bounds();
		
		if (fOrientation == SPLIT_HORIZONTAL) {
			// Horizontal split: move vertically
			float delta = point.x - fDragStartPoint.x;
			
			// Calculate which divider we're dragging
			float totalSize = 0;
			for (int32 i = 0; i < fDraggingDividerIndex; i++) {
				totalSize += fViewSizes.ItemAt(i);
			}
			
			// Calculate new size for the view before the divider
			float newSize = fViewSizes.ItemAt(fDraggingDividerIndex) + delta / frame.Width();
			
			// Clamp to reasonable values
			if (newSize > 0.1f && newSize < 0.9f) {
				fViewSizes.ReplaceItemAt(fDraggingDividerIndex, newSize);
				
				// Adjust the next view's size
				float nextSize = fViewSizes.ItemAt(fDraggingDividerIndex + 1) - delta / frame.Width();
				if (nextSize > 0.1f) {
					fViewSizes.ReplaceItemAt(fDraggingDividerIndex + 1, nextSize);
				}
				
				Layout();
			}
		} else {
			// Vertical split: move horizontally
			float delta = point.y - fDragStartPoint.y;
			
			// Calculate which divider we're dragging
			float totalSize = 0;
			for (int32 i = 0; i < fDraggingDividerIndex; i++) {
				totalSize += fViewSizes.ItemAt(i);
			}
			
			// Calculate new size for the view before the divider
			float newSize = fViewSizes.ItemAt(fDraggingDividerIndex) + delta / frame.Height();
			
			// Clamp to reasonable values
			if (newSize > 0.1f && newSize < 0.9f) {
				fViewSizes.ReplaceItemAt(fDraggingDividerIndex, newSize);
				
				// Adjust the next view's size
				float nextSize = fViewSizes.ItemAt(fDraggingDividerIndex + 1) - delta / frame.Height();
				if (nextSize > 0.1f) {
					fViewSizes.ReplaceItemAt(fDraggingDividerIndex + 1, nextSize);
				}
				
				Layout();
			}
		}
	}
}

// Size management

BSize SplitView::MinSize() {
	BSize minSize(0, 0);
	
	for (int32 i = 0; i < fViews.CountItems(); i++) {
		BView* view = fViews.ItemAt(i);
		BSize viewMin = view->MinSize();
		
		if (fOrientation == SPLIT_HORIZONTAL) {
			minSize.width += viewMin.width;
			minSize.height = std::max(minSize.height, viewMin.height);
		} else {
			minSize.width = std::max(minSize.width, viewMin.width);
			minSize.height += viewMin.height;
		}
	}
	
	// Add divider sizes
	if (fOrientation == SPLIT_HORIZONTAL) {
		minSize.width += fDividerSize * (fViews.CountItems() - 1);
	} else {
		minSize.height += fDividerSize * (fViews.CountItems() - 1);
	}
	
	return minSize;
}

BSize SplitView::MaxSize() {
	BSize maxSize(B_SIZE_UNLIMITED, B_SIZE_UNLIMITED);
	
	for (int32 i = 0; i < fViews.CountItems(); i++) {
		BView* view = fViews.ItemAt(i);
		BSize viewMax = view->MaxSize();
		
		if (fOrientation == SPLIT_HORIZONTAL) {
			if (viewMax.width < B_SIZE_UNLIMITED) {
				maxSize.width = std::min(maxSize.width, viewMax.width);
			}
			if (viewMax.height < B_SIZE_UNLIMITED) {
				maxSize.height = std::min(maxSize.height, viewMax.height);
			}
		} else {
			if (viewMax.width < B_SIZE_UNLIMITED) {
				maxSize.width = std::min(maxSize.width, viewMax.width);
			}
			if (viewMax.height < B_SIZE_UNLIMITED) {
				maxSize.height = std::min(maxSize.height, viewMax.height);
			}
		}
	}
	
	return maxSize;
}

BSize SplitView::PreferredSize() {
	BSize preferredSize(0, 0);
	
	for (int32 i = 0; i < fViews.CountItems(); i++) {
		BView* view = fViews.ItemAt(i);
		BSize viewPreferred = view->PreferredSize();
		
		if (fOrientation == SPLIT_HORIZONTAL) {
			preferredSize.width += viewPreferred.width * fViewSizes.ItemAt(i);
			preferredSize.height = std::max(preferredSize.height, viewPreferred.height);
		} else {
			preferredSize.width = std::max(preferredSize.width, viewPreferred.width);
			preferredSize.height += viewPreferred.height * fViewSizes.ItemAt(i);
		}
	}
	
	// Add divider sizes
	if (fOrientation == SPLIT_HORIZONTAL) {
		preferredSize.width += fDividerSize * (fViews.CountItems() - 1);
	} else {
		preferredSize.height += fDividerSize * (fViews.CountItems() - 1);
	}
	
	return preferredSize;
}

// Frame management

void SplitView::FrameResized(float width, float height) {
	BView::FrameResized(width, height);
	Layout();
}

// Helper methods

void SplitView::UpdateViewFrames() {
	BRect frame = Bounds();
	
	if (fOrientation == SPLIT_HORIZONTAL) {
		// Horizontal split: arrange views side by side
		float x = frame.left;
		
		for (int32 i = 0; i < fViews.CountItems(); i++) {
			float viewWidth = frame.Width() * fViewSizes.ItemAt(i);
			
			BRect viewFrame(x, frame.top, x + viewWidth, frame.bottom);
			
			BView* view = fViews.ItemAt(i);
			view->MoveTo(viewFrame.LeftTop());
			view->ResizeTo(viewFrame.Width(), viewFrame.Height());
			
			x += viewWidth + fDividerSize;
		}
	} else {
		// Vertical split: arrange views top to bottom
		float y = frame.top;
		
		for (int32 i = 0; i < fViews.CountItems(); i++) {
			float viewHeight = frame.Height() * fViewSizes.ItemAt(i);
			
			BRect viewFrame(frame.left, y, frame.right, y + viewHeight);
			
			BView* view = fViews.ItemAt(i);
			view->MoveTo(viewFrame.LeftTop());
			view->ResizeTo(viewFrame.Width(), viewFrame.Height());
			
			y += viewHeight + fDividerSize;
		}
	}
}

int32 SplitView::FindDividerAt(BPoint point) const {
	BRect frame = Bounds();
	
	if (fOrientation == SPLIT_HORIZONTAL) {
		// Horizontal split: check vertical dividers
		float x = frame.left;
		
		for (int32 i = 0; i < fViews.CountItems() - 1; i++) {
			float viewWidth = frame.Width() * fViewSizes.ItemAt(i);
			x += viewWidth;
			
			BRect dividerRect(x, frame.top, x + fDividerSize, frame.bottom);
			if (dividerRect.Contains(point)) {
				return i;
			}
			
			x += fDividerSize;
		}
	} else {
		// Vertical split: check horizontal dividers
		float y = frame.top;
		
		for (int32 i = 0; i < fViews.CountItems() - 1; i++) {
			float viewHeight = frame.Height() * fViewSizes.ItemAt(i);
			y += viewHeight;
			
			BRect dividerRect(frame.left, y, frame.right, y + fDividerSize);
			if (dividerRect.Contains(point)) {
				return i;
			}
			
			y += fDividerSize;
		}
	}
	
	return -1;
}

} // namespace UI
} // namespace YellowDocs
