#ifndef YELLOWDOCS_WRITER_REPLACE_PANEL_H
#define YELLOWDOCS_WRITER_REPLACE_PANEL_H

#include <Window.h>
#include <String.h>

namespace YellowDocs {
namespace Writer {

// Forward declaration
class EditorView;

// Replace panel for YellowDocs Writer
class ReplacePanel : public BWindow {
public:
	ReplacePanel(BRect frame, EditorView* editor);
	~ReplacePanel();

	// Window lifecycle
	void MessageReceived(BMessage* message) override;
	bool QuitRequested() override;

	// Replace operations
	void Replace();
	void ReplaceAll();

	// Set search text
	void SetSearchText(const BString& text);
	BString SearchText() const;

	// Set replace text
	void SetReplaceText(const BString& text);
	BString ReplaceText() const;

private:
	EditorView* fEditor;
	BString fSearchText;
	BString fReplaceText;
	
	// Create UI
	void CreateUI();
	
	// Message constants
	enum {
		MSG_REPLACE_REPLACE = 'rplr',
		MSG_REPLACE_REPLACE_ALL = 'rpla',
		MSG_REPLACE_CLOSE = 'rplc',
		MSG_REPLACE_TEXT_CHANGED = 'rplt'
	};
};

} // namespace Writer
} // namespace YellowDocs

#endif // YELLOWDOCS_WRITER_REPLACE_PANEL_H
