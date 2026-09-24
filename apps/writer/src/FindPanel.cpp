#include "FindPanel.h"

#include <Application.h>
#include <Box.h>
#include <Button.h>
#include <CheckBox.h>
#include <LayoutBuilder.h>
#include <Message.h>
#include <TextControl.h>

#include "EditorView.h"

using namespace YellowDocs::Core;
using namespace YellowDocs::UI;

namespace YellowDocs {
namespace Writer {

// FindPanel implementation

FindPanel::FindPanel(BRect frame, EditorView* editor)
	: BWindow(frame, "Find - YellowDocs Writer", B_TITLED_WINDOW,
		B_ASYNCHRONOUS_CONTROLS | B_QUIT_ON_WINDOW_CLOSE | B_NOT_RESIZABLE),
	  fEditor(editor),
	  fCaseSensitive(false),
	  fWrapAround(true),
	  fWholeWords(false)
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

FindPanel::~FindPanel() {
}

// Window lifecycle

void FindPanel::MessageReceived(BMessage* message) {
	switch (message->what) {
		case MSG_FIND_NEXT:
			FindNext();
			break;
		
		case MSG_FIND_PREVIOUS:
			FindPrevious();
			break;
		
		case MSG_FIND_REPLACE:
			Replace();
			break;
		
		case MSG_FIND_REPLACE_ALL:
			ReplaceAll();
			break;
		
		case MSG_FIND_CLOSE:
			PostMessage(B_QUIT_REQUESTED);
			break;
		
		case MSG_FIND_TEXT_CHANGED:
			// Update search text
			{
				const char* text;
				if (message->FindString("text", &text) == B_OK) {
					SetSearchText(text);
				}
			}
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

bool FindPanel::QuitRequested() {
	// Hide instead of quit
	Hide();
	return false;
}

// Find operations

void FindPanel::FindNext() {
	if (!fEditor || fSearchText.IsEmpty()) {
		return;
	}
	
	// TODO: Implement find next
	// This would search for the text in the editor and select it
}

void FindPanel::FindPrevious() {
	if (!fEditor || fSearchText.IsEmpty()) {
		return;
	}
	
	// TODO: Implement find previous
}

void FindPanel::Replace() {
	if (!fEditor || fSearchText.IsEmpty()) {
		return;
	}
	
	// TODO: Implement replace
	// This would replace the selected text with the replace text
}

void FindPanel::ReplaceAll() {
	if (!fEditor || fSearchText.IsEmpty()) {
		return;
	}
	
	// TODO: Implement replace all
	// This would replace all occurrences of search text with replace text
}

// Set search text

void FindPanel::SetSearchText(const BString& text) {
	fSearchText = text;
}

BString FindPanel::SearchText() const {
	return fSearchText;
}

// Set replace text

void FindPanel::SetReplaceText(const BString& text) {
	fReplaceText = text;
}

BString FindPanel::ReplaceText() const {
	return fReplaceText;
}

// Options

void FindPanel::SetCaseSensitive(bool sensitive) {
	fCaseSensitive = sensitive;
}

bool FindPanel::IsCaseSensitive() const {
	return fCaseSensitive;
}

void FindPanel::SetWrapAround(bool wrap) {
	fWrapAround = wrap;
}

bool FindPanel::IsWrapAround() const {
	return fWrapAround;
}

void FindPanel::SetWholeWords(bool wholeWords) {
	fWholeWords = wholeWords;
}

bool FindPanel::IsWholeWords() const {
	return fWholeWords;
}

// Create UI

void FindPanel::CreateUI() {
	BLayoutBuilder::Group<>(this, B_VERTICAL, 10)
		.SetInsets(10, 10, 10, 10)
		.AddGroup(B_VERTICAL, 5)
			.Add(new BStringView("find_label", "Find:"))
			.Add(new BTextControl("find_text", "", new BMessage(MSG_FIND_TEXT_CHANGED)))
			.Add(new BStringView("replace_label", "Replace with:"))
			.Add(new BTextControl("replace_text", "", new BMessage(MSG_REPLACE_TEXT_CHANGED)))
			.End()
			.AddGroup(B_HORIZONTAL, 10)
				.Add(new BCheckBox("case_sensitive", "Case sensitive", new BMessage()))
				.Add(new BCheckBox("whole_words", "Whole words", new BMessage()))
				.Add(new BCheckBox("wrap_around", "Wrap around", new BMessage()))
				.End()
			.AddGroup(B_HORIZONTAL, 10)
				.Add(new BButton("find_previous", "Previous", new BMessage(MSG_FIND_PREVIOUS)))
				.Add(new BButton("find_next", "Next", new BMessage(MSG_FIND_NEXT)))
				.Add(new BButton("replace", "Replace", new BMessage(MSG_FIND_REPLACE)))
				.Add(new BButton("replace_all", "Replace All", new BMessage(MSG_FIND_REPLACE_ALL)))
				.Add(new BButton("close", "Close", new BMessage(MSG_FIND_CLOSE)))
				.End()
		.End();
}

} // namespace Writer
} // namespace YellowDocs
