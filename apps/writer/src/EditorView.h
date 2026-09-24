#ifndef YELLOWDOCS_WRITER_EDITOR_VIEW_H
#define YELLOWDOCS_WRITER_EDITOR_VIEW_H

#include <View.h>
#include <String.h>
#include <Point.h>
#include <Rect.h>

#include "../../libs/ui/src/DocumentView.h"

namespace YellowDocs {
namespace Core {
	class DocumentModel;
	class Selection;
	class TextStyle;
	class ParagraphStyle;
}

namespace Writer {

// Forward declaration
class DocumentWindow;

// Editor view class - extends UI::DocumentView with application-specific functionality
class EditorView : public YellowDocs::UI::DocumentView {
public:
	EditorView(const BString& name = "EditorView");
	~EditorView();

	// Document model
	void SetDocumentModel(Core::DocumentModel* model);
	Core::DocumentModel* DocumentModel() const;

	// Window reference
	void SetWindow(DocumentWindow* window);
	DocumentWindow* Window() const;

	// Keyboard handling
	void KeyDown(const char* bytes, int32 numBytes) override;
	void KeyUp(const char* bytes, int32 numBytes) override;

	// Mouse handling
	void MouseDown(BPoint point) override;
	void MouseUp(BPoint point) override;
	void MouseMoved(BPoint point, uint32 transit, const BMessage* dragMessage) override;

	// Focus handling
	void MakeFocus(bool focus = true) override;

	// Drawing
	void Draw(BRect frame) override;

	// Size management
	BSize MinSize() override;
	BSize MaxSize() override;
	BSize PreferredSize() override;

private:
	DocumentWindow* fWindow;
	
	// Helper methods
	void HandleTextInput(const BString& text);
	void HandleCommandKey(int32 key, int32 modifiers);
	void UpdateWindowState();
};

} // namespace Writer
} // namespace YellowDocs

#endif // YELLOWDOCS_WRITER_EDITOR_VIEW_H
