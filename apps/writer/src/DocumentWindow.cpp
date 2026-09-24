#include "DocumentWindow.h"

#include <Application.h>
#include <Box.h>
#include <Button.h>
#include <FilePanel.h>
#include <LayoutBuilder.h>
#include <Menu.h>
#include <MenuBar.h>
#include <MenuItem.h>
#include <Message.h>
#include <ScrollView.h>
#include <SeparatorView.h>
#include <StatusBar.h>
#include <StringView.h>

#include "EditorView.h"
#include "ToolbarView.h"
#include "StatusView.h"
#include "YellowDocsWriterApp.h"

#include "../../libs/core/src/DocumentModel.h"

using namespace YellowDocs::Core;
using namespace YellowDocs::UI;

namespace YellowDocs {
namespace Writer {

// DocumentWindow implementation

DocumentWindow::DocumentWindow(BRect frame)
	: BWindow(frame, "Untitled - YellowDocs Writer", B_TITLED_WINDOW,
		B_ASYNCHRONOUS_CONTROLS | B_QUIT_ON_WINDOW_CLOSE | B_AUTO_UPDATE_SIZE_LIMITS),
	  fToolbarView(nullptr),
	  fEditorView(nullptr),
	  fStatusView(nullptr),
	  fDocumentModel(nullptr),
	  fSavePanel(nullptr),
	  fModified(false)
{
	// Set window feel
	SetFeel(B_NORMAL_WINDOW_FEEL);
	
	// Create document model
	fDocumentModel = new DocumentModel();
	
	// Set default document title
	fDocumentTitle = "Untitled";
	
	// Create UI
	CreateUI();
	
	// Update title
	UpdateTitle();
	
	// Center window
	CenterOnScreen();
}

DocumentWindow::~DocumentWindow() {
	// Clean up file panel
	if (fSavePanel) {
		fSavePanel->Hide();
		delete fSavePanel;
	}
	
	// Clean up document model
	delete fDocumentModel;
}

// Window lifecycle

void DocumentWindow::MessageReceived(BMessage* message) {
	switch (message->what) {
		case MSG_DOCUMENT_SAVE:
			SaveDocument();
			break;
		
		case MSG_DOCUMENT_SAVE_AS:
			ShowSavePanel();
			break;
		
		case MSG_DOCUMENT_CLOSE:
			PostMessage(B_QUIT_REQUESTED);
			break;
		
		case MSG_DOCUMENT_MODIFIED:
			SetModified(true);
			UpdateTitle();
			UpdateUIState();
			break;
		
		case MSG_TEXT_CHANGED:
			// Text changed in editor
			UpdateUIState();
			break;
		
		case MSG_SELECTION_CHANGED:
			// Selection changed in editor
			UpdateUIState();
			break;
		
		case MSG_STYLE_CHANGED:
			// Style changed in editor
			UpdateUIState();
			break;
		
		case B_SAVE_REQUESTED:
			// Handle save from file panel
			if (fSavePanel) {
				entry_ref ref;
				if (message->FindRef("directory", &ref) == B_OK) {
					BEntry entry(&ref);
					BPath path;
					entry.GetPath(&path);
					
					BString filename;
					if (message->FindString("name", &filename) == B_OK) {
						path.Append(filename.String());
					}
					
					SaveDocument(path.Path());
					fSavePanel->Hide();
				}
			}
			break;
		
		case B_CANCEL:
			// Cancel save panel
			if (fSavePanel) {
				fSavePanel->Hide();
			}
			break;
		
		default:
			BWindow::MessageReceived(message);
			break;
	}
}

bool DocumentWindow::QuitRequested() {
	// Check if document is modified
	if (fModified) {
		BAlert* alert = new BAlert("Save Changes",
			"You have unsaved changes. Save before closing?",
			"Save", "Don't Save", "Cancel", B_WIDTH_AS_USUAL, B_OFFSET_HORIZONTAL);
		
		int32 choice = alert->Go();
		
		if (choice == 0) {
			// Save
			if (!SaveDocument()) {
				// Save failed, don't close
				return false;
			}
		} else if (choice == 2) {
			// Cancel
			return false;
		}
		// Don't Save - continue with quit
	}
	
	// Remove from application's document windows list
	if (YellowDocsWriterApp* app = dynamic_cast<YellowDocsWriterApp*>(be_app)) {
		app->CloseAllDocuments(); // This will close all, but we'll be removed anyway
	}
	
	return true;
}

void DocumentWindow::FrameResized(float width, float height) {
	BWindow::FrameResized(width, height);
	
	// Resize views
	if (fEditorView) {
		fEditorView->ResizeTo(width, height - 60); // Account for toolbar and status
	}
}

// Document management

bool DocumentWindow::LoadDocument(const BString& path) {
	// TODO: Implement document loading
	// For now, just set the path and update title
	
	fDocumentPath = path;
	fDocumentTitle = GetFileName(path);
	SetModified(false);
	
	UpdateTitle();
	UpdateUIState();
	
	return true;
}

bool DocumentWindow::SaveDocument(const BString& path) {
	// TODO: Implement document saving
	// For now, just set the path and update title
	
	fDocumentPath = path;
	if (fDocumentTitle.IsEmpty()) {
		fDocumentTitle = GetFileName(path);
	}
	SetModified(false);
	
	UpdateTitle();
	UpdateUIState();
	
	return true;
}

bool DocumentWindow::SaveDocument() {
	if (fDocumentPath.IsEmpty()) {
		return SaveDocumentAs();
	}
	
	return SaveDocument(fDocumentPath);
}

// File operations

void DocumentWindow::ShowSavePanel() {
	// Create save panel if it doesn't exist
	if (!fSavePanel) {
		fSavePanel = new BFilePanel(B_SAVE_PANEL, this, nullptr, B_FILE_PANEL_SAVE);
		fSavePanel->SetTitle("Save Document");
		fSavePanel->SetSaveText("Untitled.ywrite");
		
		// Set default directory
		BPath path;
		find_directory(B_USER_DOCUMENTS_DIRECTORY, &path);
		fSavePanel->SetPanelDirectory(path.Path());
	}
	
	// Show save panel
	fSavePanel->Show();
}

bool DocumentWindow::SaveDocumentAs() {
	ShowSavePanel();
	return false; // Will be handled by message
}

// Document properties

BString DocumentWindow::DocumentPath() const {
	return fDocumentPath;
}

void DocumentWindow::SetDocumentPath(const BString& path) {
	fDocumentPath = path;
	UpdateTitle();
}

bool DocumentWindow::IsModified() const {
	return fModified;
}

void DocumentWindow::SetModified(bool modified) {
	fModified = modified;
	UpdateTitle();
	UpdateUIState();
}

BString DocumentWindow::DocumentTitle() const {
	return fDocumentTitle;
}

void DocumentWindow::SetDocumentTitle(const BString& title) {
	fDocumentTitle = title;
	UpdateTitle();
}

// Document model access

Core::DocumentModel* DocumentWindow::DocumentModel() const {
	return fDocumentModel;
}

// Create UI

void DocumentWindow::CreateUI() {
	// Create main layout
	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.SetInsets(0, 0, 0, 0)
		.AddGroup(B_VERTICAL, 0)
			.Add(fToolbarView = new ToolbarView("toolbar"))
			.Add(fEditorView = new EditorView("editor"))
			.Add(fStatusView = new StatusView("status"))
			.End()
		.End();
	
	// Set editor view document model
	if (fEditorView && fDocumentModel) {
		fEditorView->SetDocumentModel(fDocumentModel);
	}
	
	// Create menu
	CreateMenu();
}

// Create menu

void DocumentWindow::CreateMenu() {
	// Create menu bar
	BMenuBar* menuBar = new BMenuBar("menu_bar");
	
	// File menu
	BMenu* fileMenu = new BMenu("File");
	
	fileMenu->AddItem(new BMenuItem("Save", new BMessage(MSG_DOCUMENT_SAVE), 'S'));
	fileMenu->AddItem(new BMenuItem("Save As...", new BMessage(MSG_DOCUMENT_SAVE_AS)));
	fileMenu->AddItem(new BSeparatorItem());
	fileMenu->AddItem(new BMenuItem("Close", new BMessage(MSG_DOCUMENT_CLOSE), 'W'));
	
	menuBar->AddItem(fileMenu);
	
	// Edit menu
	BMenu* editMenu = new BMenu("Edit");
	
	editMenu->AddItem(new BMenuItem("Undo", new BMessage(B_UNDO), 'Z'));
	editMenu->AddItem(new BMenuItem("Redo", new BMessage(B_REDO), 'Y'));
	editMenu->AddItem(new BSeparatorItem());
	editMenu->AddItem(new BMenuItem("Cut", new BMessage(B_CUT), 'X'));
	editMenu->AddItem(new BMenuItem("Copy", new BMessage(B_COPY), 'C'));
	editMenu->AddItem(new BMenuItem("Paste", new BMessage(B_PASTE), 'V'));
	editMenu->AddItem(new BMenuItem("Delete", new BMessage(B_DELETE), B_DELETE));
	editMenu->AddItem(new BSeparatorItem());
	editMenu->AddItem(new BMenuItem("Select All", new BMessage(B_SELECT_ALL), 'A'));
	
	menuBar->AddItem(editMenu);
	
	// Format menu
	BMenu* formatMenu = new BMenu("Format");
	
	BMenu* fontMenu = new BMenu("Font");
	fontMenu->AddItem(new BMenuItem("Bold", new BMessage(MSG_FORMAT_BOLD)));
	fontMenu->AddItem(new BMenuItem("Italic", new BMessage(MSG_FORMAT_ITALIC)));
	fontMenu->AddItem(new BMenuItem("Underline", new BMessage(MSG_FORMAT_UNDERLINE)));
	formatMenu->AddItem(fontMenu);
	
	BMenu* alignMenu = new BMenu("Alignment");
	alignMenu->AddItem(new BMenuItem("Left", new BMessage(MSG_FORMAT_ALIGN_LEFT)));
	alignMenu->AddItem(new BMenuItem("Center", new BMessage(MSG_FORMAT_ALIGN_CENTER)));
	alignMenu->AddItem(new BMenuItem("Right", new BMessage(MSG_FORMAT_ALIGN_RIGHT)));
	alignMenu->AddItem(new BMenuItem("Justify", new BMessage(MSG_FORMAT_ALIGN_JUSTIFY)));
	formatMenu->AddItem(alignMenu);
	
	menuBar->AddItem(formatMenu);
	
	// Add menu bar to window
	AddChild(menuBar);
}

// Update title

void DocumentWindow::UpdateTitle() {
	BString title;
	
	if (!fDocumentTitle.IsEmpty()) {
		title = fDocumentTitle;
	} else if (!fDocumentPath.IsEmpty()) {
		title = GetFileName(fDocumentPath);
	} else {
		title = "Untitled";
	}
	
	if (fModified) {
		title << " *";
	}
	
	title << " - YellowDocs Writer";
	
	SetTitle(title.String());
}

// Update UI state

void DocumentWindow::UpdateUIState() {
	// Update status view
	if (fStatusView) {
		BString status;
		
		if (fDocumentModel) {
			status << "Characters: " << fDocumentModel->GetAllText().Length();
		}
		
		fStatusView->SetText(status.String());
	}
}

} // namespace Writer
} // namespace YellowDocs
