#ifndef YELLOWDOCS_UI_DOCUMENT_VIEW_H
#define YELLOWDOCS_UI_DOCUMENT_VIEW_H

#include <View.h>
#include <String.h>
#include <Point.h>
#include <Rect.h>
#include <Vector.h>

namespace YellowDocs {
namespace Core {
	class DocumentModel;
	class Selection;
	class TextStyle;
	class ParagraphStyle;
}

namespace UI {

// Forward declarations
class DocumentView;

// Document view delegate interface
class DocumentViewDelegate {
public:
	virtual ~DocumentViewDelegate();
	
	virtual void SelectionChanged(const Core::Selection& selection) = 0;
	virtual void TextChanged() = 0;
	virtual void StyleChanged() = 0;
	virtual void DocumentModified() = 0;
};

// Document view class
class DocumentView : public BView {
public:
	DocumentView(const BString& name = "DocumentView");
	~DocumentView();

	// Document model
	void SetDocumentModel(Core::DocumentModel* model);
	Core::DocumentModel* DocumentModel() const;

	// Delegate
	void SetDelegate(DocumentViewDelegate* delegate);
	DocumentViewDelegate* Delegate() const;

	// Selection
	void SetSelection(const Core::Selection& selection);
	Core::Selection Selection() const;

	void SelectAll();
	void SelectNone();

	// Text editing
	bool InsertText(const BString& text);
	bool DeleteSelection();
	bool ReplaceSelection(const BString& text);

	// Style
	void SetTextStyle(const Core::TextStyle& style);
	Core::TextStyle TextStyle() const;

	void SetParagraphStyle(const Core::ParagraphStyle& style);
	Core::ParagraphStyle ParagraphStyle() const;

	// View settings
	void SetShowMargin(bool show);
	bool ShowMargin() const;

	void SetMarginSize(float size);
	float MarginSize() const;

	void SetMarginColor(rgb_color color);
	rgb_color MarginColor() const;

	void SetLineSpacing(float spacing);
	float LineSpacing() const;

	// Drawing
	void Draw(BRect frame) override;
	
	// Mouse handling
	void MouseDown(BPoint point) override;
	void MouseUp(BPoint point) override;
	void MouseMoved(BPoint point, uint32 transit, const BMessage* dragMessage) override;

	// Keyboard handling
	void KeyDown(const char* bytes, int32 numBytes) override;
	void KeyUp(const char* bytes, int32 numBytes) override;

	// Focus handling
	void MakeFocus(bool focus = true) override;

	// Size management
	BSize MinSize() override;
	BSize MaxSize() override;
	BSize PreferredSize() override;

	// Scrolling
	void ScrollToSelection();
	void ScrollBy(float dx, float dy);

private:
	Core::DocumentModel* fDocumentModel;
	DocumentViewDelegate* fDelegate;
	Core::Selection fSelection;
	Core::TextStyle fTextStyle;
	Core::ParagraphStyle fParagraphStyle;
	
	bool fShowMargin;
	float fMarginSize;
	rgb_color fMarginColor;
	float fLineSpacing;
	
	// Mouse tracking
	BPoint fMouseDownPoint;
	bool fMouseTracking;
	
	// Helper methods
	BPoint TextPointToPosition(BPoint point) const;
	BPoint PositionToTextPoint(int32 position) const;
	
	void DrawText(BRect frame);
	void DrawSelection(BRect frame);
	void DrawMargin(BRect frame);
	
	void UpdateSelection(BPoint point);
	
	// Calculate layout
	void CalculateLayout();
};

// Inline delegate implementation
inline DocumentViewDelegate::~DocumentViewDelegate() {
}

} // namespace UI
} // namespace YellowDocs

#endif // YELLOWDOCS_UI_DOCUMENT_VIEW_H
