#ifndef YELLOWDOCS_UI_SPLIT_VIEW_H
#define YELLOWDOCS_UI_SPLIT_VIEW_H

#include <View.h>
#include <String.h>
#include <Rect.h>
#include <Point.h>
#include <Vector.h>

namespace YellowDocs {
namespace UI {

// Split view orientation
enum SplitViewOrientation {
	SPLIT_HORIZONTAL,
	SPLIT_VERTICAL
};

// Split view divider class
class SplitViewDivider {
public:
	SplitViewDivider();
	~SplitViewDivider();

	void SetPosition(float position);
	float Position() const;

	void SetSize(float size);
	float Size() const;

	void SetCollapsible(bool collapsible);
	bool IsCollapsible() const;

	void SetCollapsed(bool collapsed);
	bool IsCollapsed() const;

private:
	float fPosition;
	float fSize;
	bool fCollapsible;
	bool fCollapsed;
};

// Split view class
class SplitView : public BView {
public:
	SplitView(const BString& name = "SplitView", SplitViewOrientation orientation = SPLIT_HORIZONTAL);
	~SplitView();

	// Orientation
	void SetOrientation(SplitViewOrientation orientation);
	SplitViewOrientation Orientation() const;

	// View management
	bool AddView(BView* view, float size = 0.5f);
	bool RemoveView(BView* view);
	bool RemoveView(int32 index);
	
	int32 CountViews() const;
	BView* ViewAt(int32 index) const;
	
	void Clear();

	// Sizes
	void SetViewSize(int32 index, float size);
	float ViewSize(int32 index) const;

	void SetDividerSize(float size);
	float DividerSize() const;

	// Layout
	void Layout();

	// Drawing
	void Draw(BRect frame) override;

	// Mouse handling
	void MouseDown(BPoint point) override;
	void MouseUp(BPoint point) override;
	void MouseMoved(BPoint point, uint32 transit, const BMessage* dragMessage) override;

	// Size management
	BSize MinSize() override;
	BSize MaxSize() override;
	BSize PreferredSize() override;

	// Frame management
	void FrameResized(float width, float height) override;

private:
	SplitViewOrientation fOrientation;
	BVector<BView*> fViews;
	BVector<float> fViewSizes;
	float fDividerSize;
	
	// Divider tracking
	int32 fDraggingDividerIndex;
	bool fDraggingDivider;
	BPoint fDragStartPoint;
	BRect fDragStartFrame;
	
	// Helper methods
	void UpdateViewFrames();
	int32 FindDividerAt(BPoint point) const;
};

} // namespace UI
} // namespace YellowDocs

#endif // YELLOWDOCS_UI_SPLIT_VIEW_H
