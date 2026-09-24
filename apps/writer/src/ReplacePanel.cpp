#include "ReplacePanel.h"

#include <Application.h>
#include <Box.h>
#include <Button.h>
#include <LayoutBuilder.h>
#include <Message.h>
#include <TextControl.h>

#include "EditorView.h"

using namespace YellowDocs::Core;
using namespace YellowDocs::UI;

namespace YellowDocs {
namespace Writer {

// ReplacePanel implementation

ReplacePanel::ReplacePanel(BRect frame, EditorView* editor)
	: BWindow(frame, "Replace - YellowDocs Writer", B_TITLED_WINDOW,
		B_ASYNCHRONOUS_CONTROLS | B_QUIT_ON_WINDOW_CLOSE | B_NOT_RESIZABLE),
	  fEditor(editor)
{
	// Set window feel
	SetFeel(B_FLOATING_APP_WINDOW_FEEL);
	
	// Create UI
	CreateUI();
	
	// Center window relative to editor
	if (fEditor && fEditor->Window()) {
		BRect editorFrame = fEditor->Window()->Frame();
		BRect panelFrame = Frame();
		
		float x = editorFrame.left + (editorFrame.Width() - panelFrame.Width()) / 2;
		float y = editorFrame.top + (editorFrame.Height() - panelFrame.Height()) / 2;
		
		MoveTo(x, y);
	}
}

ReplacePanel::~ReplacePanel() {
}

// Window lifecycle

void ReplacePanel::MessageReceived(BMessage* message) {
	switch (message->what) {
		case MSG_REPLACE_REPLACE:
			Replace();
			break;
		
		case MSG_REPLACE_REPLACE_ALL:
			ReplaceAll();
			break;
		
		case MSG_REPLACE_CLOSE:
			PostMessage(B_QUIT_REQUESTED);
			break;
		
		case MSG_REPLACE_TEXT_CHANGED:
			// Update replace text
			{
				const char* text;
				if (message->FindString("text", &text) == B_OK) {
					SetReplaceText(text);
				}
			}
			break;
		
		default:
			BWindow::MessageReceived(message);
			break;
	}
}

bool ReplacePanel::QuitRequested() {
	// Hide instead of quit
	Hide();
	return false;
}

// Replace operations

void ReplacePanel::Replace() {
	if (!fEditor || fSearchText.IsEmpty()) {
		return;
	}
	
	// TODO: Implement replace
	// This would replace the selected text with the replace text
}

void ReplacePanel::ReplaceAll() {
	if (!fEditor || fSearchText.IsEmpty()) {
		return;
	}
	
	// TODO: Implement replace all
	// This would replace all occurrences of search text with replace text
}

// Set search text

void ReplacePanel::SetSearchText(const BString& text) {
	fSearchText = text;
}

BString ReplacePanel::SearchText() const {
	return fSearchText;
}

// Set replace text

void ReplacePanel::SetReplaceText(const BString& text) {
	fReplaceText = text;
}

BString ReplacePanel::ReplaceText() const {
	return fReplaceText;
}

// Create UI

void ReplacePanel::CreateUI() {
	BLayoutBuilder::Group<>(this, B_VERTICAL, 10)
		.SetInsets(10, 10, 10, 10)
		.AddGroup(B_VERTICAL, 5)
			.Add(new BStringView("replace_label", "Replace with:"))
			.Add(new BTextControl("replace_text", "", new BMessage(MSG_REPLACE_TEXT_CHANGED)))
			.End()
			.AddGroup(B_HORIZONTAL, 10)
				.Add(new BButton("replace", "Replace", new BMessage(MSG_REPLACE_REPLACE)))
				.Add(new BButton("replace_all", "Replace All", new BMessage(MSG_REPLACE_REPLACE_ALL)))
				.Add(new BButton("close", "Close", new BMessage(MSG_REPLACE_CLOSE)))
				.End()
		.End();
}

} // namespace Writer
} // namespace YellowDocs
