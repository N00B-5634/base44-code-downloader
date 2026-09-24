#ifndef YELLOWDOCS_WRITER_DOCUMENT_WINDOW_H
#define YELLOWDOCS_WRITER_DOCUMENT_WINDOW_H

#include <Window.h>
#include <String.h>
#include <FilePanel.h>

namespace YellowDocs {
namespace Core {
	class DocumentModel;
}

namespace Writer {

// Forward declarations
class EditorView;
class ToolbarView;
class StatusView;

class DocumentWindow : public BWindow {
public:
	DocumentWindow(BRect frame);
	~DocumentWindow();

	// Window lifecycle
	void MessageReceived(BMessage* message) override;
	bool QuitRequested() override;
	void FrameResized(float width, float height) override;

	// Document management
	bool LoadDocument(const BString& path);
	bool SaveDocument(const BString& path);
	bool SaveDocument();
	
	// File operations
	void ShowSavePanel();
	
	// Document properties
	BString DocumentPath() const;
	void SetDocumentPath(const BString& path);
	
	bool IsModified() const;
	void SetModified(bool modified);
	
	BString DocumentTitle() const;
	void SetDocumentTitle(const BString& title);
	
	// Document model access
	Core::DocumentModel* DocumentModel() const;
	
private:
	// UI components
	ToolbarView* fToolbarView;
	EditorView* fEditorView;
	StatusView* fStatusView;
	
	// Document state
	Core::DocumentModel* fDocumentModel;
	BString fDocumentPath;
	BString fDocumentTitle;
	bool fModified;
	
	// File panel
	BFilePanel* fSavePanel;
	
	// Create UI
	void CreateUI();
	
	// Create menu
	void CreateMenu();
	
	// Update title
	void UpdateTitle();
	
	// Update UI state
	void UpdateUIState();
	
	// Message constants
	enum {
		MSG_DOCUMENT_SAVE = 'dcs '
		MSG_DOCUMENT_SAVE_AS = 'dcsa',
		MSG_DOCUMENT_CLOSE = 'dcls',
		MSG_DOCUMENT_MODIFIED = 'dcmd',
		MSG_TEXT_CHANGED = 'txtc',
		MSG_SELECTION_CHANGED = 'slcc',
		MSG_STYLE_CHANGED = 'styc'
	};
};

} // namespace Writer
} // namespace YellowDocs

#endif // YELLOWDOCS_WRITER_DOCUMENT_WINDOW_H
