#include "EditorView.h"

#include <Application.h>
#include <Clipboard.h>
#include <Font.h>
#include <Message.h>
#include <ScrollView.h>
#include <String.h>
#include <Window.h>

#include "DocumentWindow.h"
#include "YellowDocsWriterApp.h"

#include "../../libs/core/src/DocumentModel.h"
#include "../../libs/ui/src/Theme.h"

using namespace YellowDocs::Core;
using namespace YellowDocs::UI;

namespace YellowDocs {
namespace Writer {

// EditorView implementation

EditorView::EditorView(const BString& name)
	: DocumentView(name),
	  fWindow(nullptr)
{
	// Set view properties
	SetViewColor(GetTheme().DocumentBackgroundColor());
}

EditorView::~EditorView() {
}

// Document model

void EditorView::SetDocumentModel(Core::DocumentModel* model) {
	DocumentView::SetDocumentModel(model);
	
	// Set up delegate to handle changes
	// Note: We're not setting a delegate here, but the window could be notified
	// through other means
}

Core::DocumentModel* EditorView::DocumentModel() const {
	return DocumentView::DocumentModel();
}

// Window reference

void EditorView::SetWindow(DocumentWindow* window) {
	fWindow = window;
}

DocumentWindow* EditorView::Window() const {
	return fWindow;
}

// Keyboard handling

void EditorView::KeyDown(const char* bytes, int32 numBytes) {
	// Handle command keys first
	uint32 modifiers = Window()->CurrentMessage()->GetInt32("modifiers", 0);
	
	if (modifiers & B_COMMAND_KEY) {
		HandleCommandKey(bytes[0], modifiers);
		return;
	}
	
	// Handle regular text input
	if (numBytes > 0) {
		BString text(bytes, numBytes);
		HandleTextInput(text);
	}
	
	// Let base class handle it
	DocumentView::KeyDown(bytes, numBytes);
}

void EditorView::KeyUp(const char* bytes, int32 numBytes) {
	DocumentView::KeyUp(bytes, numBytes);
}

// Mouse handling

void EditorView::MouseDown(BPoint point) {
	// Set focus to this view
	MakeFocus(true);
	
	// Let base class handle mouse down
	DocumentView::MouseDown(point);
	
	UpdateWindowState();
}

void EditorView::MouseUp(BPoint point) {
	DocumentView::MouseUp(point);
	UpdateWindowState();
}

void EditorView::MouseMoved(BPoint point, uint32 transit, const BMessage* dragMessage) {
	DocumentView::MouseMoved(point, transit, dragMessage);
	UpdateWindowState();
}

// Focus handling

void EditorView::MakeFocus(bool focus) {
	DocumentView::MakeFocus(focus);
	
	if (focus) {
		// Scroll to make selection visible when focused
		ScrollToSelection();
	}
	
	UpdateWindowState();
}

// Drawing

void EditorView::Draw(BRect frame) {
	DocumentView::Draw(frame);
}

// Size management

BSize EditorView::MinSize() {
	return BSize(200, 200);
}

BSize EditorView::MaxSize() {
	return BSize(B_SIZE_UNLIMITED, B_SIZE_UNLIMITED);
}

BSize EditorView::PreferredSize() {
	return BSize(600, 400);
}

// Helper methods

void EditorView::HandleTextInput(const BString& text) {
	// Insert text at current position
	if (InsertText(text)) {
		// Notify window that document was modified
		if (fWindow) {
			fWindow->PostMessage(MSG_DOCUMENT_MODIFIED);
		}
		
		// Update window state
		UpdateWindowState();
	}
}

void EditorView::HandleCommandKey(int32 key, int32 modifiers) {
	switch (key) {
		case 'C': // Copy
			// Copy selected text to clipboard
			if (Selection().IsValid()) {
				BString selectedText = GetSelectedText();
				if (!selectedText.IsEmpty()) {
					be_clipboard->Clear();
					BMessage message(B_MIME_DATA);
					message.AddString("text/plain", selectedText.String());
					be_clipboard->Commit(&message);
				}
			}
			break;
		
		case 'X': // Cut
			// Cut selected text to clipboard
			if (Selection().IsValid()) {
				BString selectedText = GetSelectedText();
				if (!selectedText.IsEmpty()) {
					be_clipboard->Clear();
					BMessage message(B_MIME_DATA);
					message.AddString("text/plain", selectedText.String());
					be_clipboard->Commit(&message);
					
					// Delete selection
					DeleteSelection();
					
					if (fWindow) {
						fWindow->PostMessage(MSG_DOCUMENT_MODIFIED);
					}
				}
			}
			break;
		
		case 'V': // Paste
			// Paste from clipboard
			{
				BMessage* clipboardData = be_clipboard->Data();
				if (clipboardData) {
					const char* text;
					if (clipboardData->FindString("text/plain", &text) == B_OK) {
						BString pasteText = text;
						if (!pasteText.IsEmpty()) {
							ReplaceSelection(pasteText);
							
							if (fWindow) {
								fWindow->PostMessage(MSG_DOCUMENT_MODIFIED);
							}
						}
					}
				}
			}
			break;
		
		case 'A': // Select All
			SelectAll();
			break;
		
		case 'Z': // Undo
			// TODO: Implement undo
			break;
		
		case 'Y': // Redo
			// TODO: Implement redo
			break;
		
		case 'S': // Save
			if (fWindow) {
				fWindow->PostMessage(MSG_DOCUMENT_SAVE);
			}
			break;
		
		case 'F': // Find
			// TODO: Show find panel
			break;
		
		default:
			// Let base class handle it
			BView::KeyDown(&key, 1);
			break;
	}
}

void EditorView::UpdateWindowState() {
	// Notify window about state changes
	if (fWindow) {
		// Update selection state
		fWindow->PostMessage(MSG_SELECTION_CHANGED);
		
		// Update text state
		fWindow->PostMessage(MSG_TEXT_CHANGED);
	}
}

} // namespace Writer
} // namespace YellowDocs
