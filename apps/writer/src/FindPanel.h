#ifndef YELLOWDOCS_WRITER_FIND_PANEL_H
#define YELLOWDOCS_WRITER_FIND_PANEL_H

#include <Window.h>
#include <String.h>

namespace YellowDocs {
namespace Writer {

// Forward declaration
class EditorView;

// Find panel for YellowDocs Writer
class FindPanel : public BWindow {
public:
	FindPanel(BRect frame, EditorView* editor);
	~FindPanel();

	// Window lifecycle
	void MessageReceived(BMessage* message) override;
	bool QuitRequested() override;

	// Find operations
	void FindNext();
	void FindPrevious();
	void Replace();
	void ReplaceAll();

	// Set search text
	void SetSearchText(const BString& text);
	BString SearchText() const;

	// Set replace text
	void SetReplaceText(const BString& text);
	BString ReplaceText() const;

	// Options
	void SetCaseSensitive(bool sensitive);
	bool IsCaseSensitive() const;

	void SetWrapAround(bool wrap);
	bool IsWrapAround() const;

	void SetWholeWords(bool wholeWords);
	bool IsWholeWords() const;

private:
	EditorView* fEditor;
	BString fSearchText;
	BString fReplaceText;
	bool fCaseSensitive;
	bool fWrapAround;
	bool fWholeWords;
	
	// Create UI
	void CreateUI();
	
	// Message constants
	enum {
		MSG_FIND_NEXT = 'fndn',
		MSG_FIND_PREVIOUS = 'fndp',
		MSG_FIND_REPLACE = 'fndr',
		MSG_FIND_REPLACE_ALL = 'fnda',
		MSG_FIND_CLOSE = 'fndc',
		MSG_FIND_TEXT_CHANGED = 'fndt',
		MSG_REPLACE_TEXT_CHANGED = 'rplt'
	};
};

} // namespace Writer
} // namespace YellowDocs

#endif // YELLOWDOCS_WRITER_FIND_PANEL_H
