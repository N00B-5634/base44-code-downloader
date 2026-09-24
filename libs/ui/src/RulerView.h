#ifndef YELLOWDOCS_UI_RULER_VIEW_H
#define YELLOWDOCS_UI_RULER_VIEW_H

#include <View.h>
#include <String.h>
#include <Rect.h>
#include <Point.h>
#include <Vector.h>

namespace YellowDocs {
namespace UI {

// Ruler view orientation
enum RulerOrientation {
	RULER_HORIZONTAL,
	RULER_VERTICAL
};

// Ruler view class
class RulerView : public BView {
public:
	RulerView(const BString& name = "RulerView", RulerOrientation orientation = RULER_HORIZONTAL);
	~RulerView();

	// Orientation
	void SetOrientation(RulerOrientation orientation);
	RulerOrientation Orientation() const;

	// Scale
	void SetScale(float scale);
	float Scale() const;

	// Units
	void SetUnits(const BString& units);
	BString Units() const;

	// Origin
	void SetOrigin(float origin);
	float Origin() const;

	// Size
	void SetSize(float size);
	float Size() const;

	// Markers
	void AddMarker(float position, const BString& label = "");
	bool RemoveMarker(float position);
	void ClearMarkers();
	
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

private:
	RulerOrientation fOrientation;
	float fScale;
	BString fUnits;
	float fOrigin;
	float fSize;
	
	BVector<float> fMarkers;
	BVector<BString> fMarkerLabels;
	
	// Helper methods
	BString FormatValue(float value) const;
	void DrawHorizontalRuler(BRect frame);
	void DrawVerticalRuler(BRect frame);
};

} // namespace UI
} // namespace YellowDocs

#endif // YELLOWDOCS_UI_RULER_VIEW_H
