#include "ToolbarView.h"

#include <Bitmap.h>
#include <Message.h>

#include "../../libs/ui/src/Theme.h"

using namespace YellowDocs::UI;

namespace YellowDocs {
namespace Writer {

// ToolbarView implementation

ToolbarView::ToolbarView(const BString& name)
	: Toolbar(name)
{
	// Initialize toolbar
	Initialize();
}

ToolbarView::~ToolbarView() {
}

// Initialize toolbar

void ToolbarView::Initialize() {
	// Set message target to this view
	SetMessageTarget(this);
	
	// Create toolbar items
	CreateItems();
	
	// Update state
	UpdateState();
}

// Update toolbar state

void ToolbarView::UpdateState() {
	// Update button states based on document state
	// This will be implemented when we have the document model connected
}

// Message handling

void ToolbarView::MessageReceived(BMessage* message) {
	switch (message->what) {
		case MSG_TOOLBAR_NEW:
			// New document
			// Forward to application
			if (YellowDocsWriterApp* app = dynamic_cast<YellowDocsWriterApp*>(be_app)) {
				app->NewDocument();
			}
			break;
		
		case MSG_TOOLBAR_OPEN:
			// Open document
			if (YellowDocsWriterApp* app = dynamic_cast<YellowDocsWriterApp*>(be_app)) {
				app->ShowOpenPanel();
			}
			break;
		
		case MSG_TOOLBAR_SAVE:
			// Save document
			if (YellowDocsWriterApp* app = dynamic_cast<YellowDocsWriterApp*>(be_app)) {
				if (DocumentWindow* window = app->GetActiveDocumentWindow()) {
					app->SaveDocument(window);
				}
			}
			break;
		
		case MSG_TOOLBAR_BOLD:
			// Toggle bold
			// TODO: Implement bold formatting
			break;
		
		case MSG_TOOLBAR_ITALIC:
			// Toggle italic
			// TODO: Implement italic formatting
			break;
		
		case MSG_TOOLBAR_UNDERLINE:
			// Toggle underline
			// TODO: Implement underline formatting
			break;
		
		case MSG_TOOLBAR_ALIGN_LEFT:
			// Left align
			// TODO: Implement left alignment
			break;
		
		case MSG_TOOLBAR_ALIGN_CENTER:
			// Center align
			// TODO: Implement center alignment
			break;
		
		case MSG_TOOLBAR_ALIGN_RIGHT:
			// Right align
			// TODO: Implement right alignment
			break;
		
		case MSG_TOOLBAR_ALIGN_JUSTIFY:
			// Justify align
			// TODO: Implement justify alignment
			break;
		
		default:
			Toolbar::MessageReceived(message);
			break;
	}
}

// Create toolbar items

void ToolbarView::CreateItems() {
	// File operations
	AddButton("new", "New", "Create a new document", MSG_TOOLBAR_NEW);
	AddButton("open", "Open", "Open an existing document", MSG_TOOLBAR_OPEN);
	AddButton("save", "Save", "Save the current document", MSG_TOOLBAR_SAVE);
	
	// Separator
	AddSeparator();
	
	// Formatting
	AddButton("bold", "Bold", "Toggle bold formatting", MSG_TOOLBAR_BOLD);
	AddButton("italic", "Italic", "Toggle italic formatting", MSG_TOOLBAR_ITALIC);
	AddButton("underline", "Underline", "Toggle underline formatting", MSG_TOOLBAR_UNDERLINE);
	
	// Separator
	AddSeparator();
	
	// Alignment
	AddButton("align_left", "Left", "Left align text", MSG_TOOLBAR_ALIGN_LEFT);
	AddButton("align_center", "Center", "Center align text", MSG_TOOLBAR_ALIGN_CENTER);
	AddButton("align_right", "Right", "Right align text", MSG_TOOLBAR_ALIGN_RIGHT);
	AddButton("align_justify", "Justify", "Justify text", MSG_TOOLBAR_ALIGN_JUSTIFY);
}

} // namespace Writer
} // namespace YellowDocs
