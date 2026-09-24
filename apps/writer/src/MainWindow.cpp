#include "MainWindow.h"

#include <Application.h>
#include <Box.h>
#include <Button.h>
#include <FilePanel.h>
#include <LayoutBuilder.h>
#include <Menu.h>
#include <MenuBar.h>
#include <MenuItem.h>
#include <Message.h>
#include <SeparatorView.h>
#include <StatusBar.h>
#include <StringView.h>
#include <TextView.h>

#include "YellowDocsWriterApp.h"
#include "DocumentWindow.h"
#include "AboutWindow.h"

using namespace YellowDocs::Core;
using namespace YellowDocs::UI;

namespace YellowDocs {
namespace Writer {

// MainWindow implementation

MainWindow::MainWindow(BRect frame)
	: BWindow(frame, "YellowDocs Writer", B_TITLED_WINDOW,
		B_ASYNCHRONOUS_CONTROLS | B_QUIT_ON_WINDOW_CLOSE)
{
	// Set window feel
	SetFeel(B_NORMAL_WINDOW_FEEL);
	
	// Create menu bar
	CreateMenuBar();
	
	// Create toolbar
	CreateToolbar();
	
	// Create status bar
	CreateStatusBar();
	
	// Center window
	CenterOnScreen();
}

MainWindow::~MainWindow() {
}

// Window lifecycle

void MainWindow::MessageReceived(BMessage* message) {
	switch (message->what) {
		case MSG_FILE_NEW:
			NewDocument();
			break;
		
		case MSG_FILE_OPEN:
			ShowOpenPanel();
			break;
		
		case MSG_FILE_SAVE:
			// Save active document
			if (YellowDocsWriterApp* app = dynamic_cast<YellowDocsWriterApp*>(be_app)) {
				if (DocumentWindow* window = app->GetActiveDocumentWindow()) {
					app->SaveDocument(window);
				}
			}
			break;
		
		case MSG_FILE_SAVE_AS:
			// Save active document as
			if (YellowDocsWriterApp* app = dynamic_cast<YellowDocsWriterApp*>(be_app)) {
				if (DocumentWindow* window = app->GetActiveDocumentWindow()) {
					window->ShowSavePanel();
				}
			}
			break;
		
		case MSG_FILE_CLOSE:
			// Close active document
			if (YellowDocsWriterApp* app = dynamic_cast<YellowDocsWriterApp*>(be_app)) {
				if (DocumentWindow* window = app->GetActiveDocumentWindow()) {
					window->PostMessage(B_QUIT_REQUESTED);
				}
			}
			break;
		
		case MSG_FILE_QUIT:
			PostMessage(B_QUIT_REQUESTED);
			break;
		
		case MSG_EDIT_UNDO:
			// TODO: Implement undo
			break;
		
		case MSG_EDIT_REDO:
			// TODO: Implement redo
			break;
		
		case MSG_EDIT_CUT:
			// TODO: Implement cut
			break;
		
		case MSG_EDIT_COPY:
			// TODO: Implement copy
			break;
		
		case MSG_EDIT_PASTE:
			// TODO: Implement paste
			break;
		
		case MSG_EDIT_SELECT_ALL:
			// TODO: Implement select all
			break;
		
		case MSG_EDIT_FIND:
			// TODO: Implement find
			break;
		
		case MSG_EDIT_REPLACE:
			// TODO: Implement replace
			break;
		
		case MSG_FORMAT_BOLD:
			// TODO: Implement bold formatting
			break;
		
		case MSG_FORMAT_ITALIC:
			// TODO: Implement italic formatting
			break;
		
		case MSG_FORMAT_UNDERLINE:
			// TODO: Implement underline formatting
			break;
		
		case MSG_FORMAT_ALIGN_LEFT:
			// TODO: Implement left alignment
			break;
		
		case MSG_FORMAT_ALIGN_CENTER:
			// TODO: Implement center alignment
			break;
		
		case MSG_FORMAT_ALIGN_RIGHT:
			// TODO: Implement right alignment
			break;
		
		case MSG_FORMAT_ALIGN_JUSTIFY:
			// TODO: Implement justify alignment
			break;
		
		case MSG_VIEW_ZOOM_IN:
			// TODO: Implement zoom in
			break;
		
		case MSG_VIEW_ZOOM_OUT:
			// TODO: Implement zoom out
			break;
		
		case MSG_VIEW_ZOOM_RESET:
			// TODO: Implement zoom reset
			break;
		
		case MSG_HELP_ABOUT:
			ShowAbout();
			break;
		
		case MSG_HELP_HELP:
			ShowHelp();
			break;
		
		default:
			BWindow::MessageReceived(message);
			break;
	}
}

bool MainWindow::QuitRequested() {
	// Hide instead of quit
	Hide();
	return false;
}

// File operations

void MainWindow::ShowOpenPanel() {
	// Create file open panel
	BFilePanel* panel = new BFilePanel(B_OPEN_PANEL, nullptr, nullptr, B_FILE_PANEL_OPEN);
	
	if (panel) {
		// Set panel properties
		panel->SetTitle("Open Document");
		
		// Filter for YellowDocs Writer files
		BMessage message(B_SET_PROPERTY);
		message.AddString("text", "YellowDocs Writer Documents");
		message.AddString("mimetype", "application/x-vnd.yellowdocs-writer");
		
		// Show panel
		panel->Show();
	}
}

void MainWindow::ShowSavePanel() {
	// This should be handled by DocumentWindow
	if (YellowDocsWriterApp* app = dynamic_cast<YellowDocsWriterApp*>(be_app)) {
		if (DocumentWindow* window = app->GetActiveDocumentWindow()) {
			window->ShowSavePanel();
		}
	}
}

void MainWindow::ShowPreferences() {
	if (YellowDocsWriterApp* app = dynamic_cast<YellowDocsWriterApp*>(be_app)) {
		app->ShowPreferences();
	}
}

void MainWindow::ShowAbout() {
	if (YellowDocsWriterApp* app = dynamic_cast<YellowDocsWriterApp*>(be_app)) {
		app->ShowAbout();
	}
}

void MainWindow::ShowHelp() {
	if (YellowDocsWriterApp* app = dynamic_cast<YellowDocsWriterApp*>(be_app)) {
		app->ShowHelp();
	}
}

// Document management

void MainWindow::NewDocument() {
	if (YellowDocsWriterApp* app = dynamic_cast<YellowDocsWriterApp*>(be_app)) {
		app->NewDocument();
	}
}

void MainWindow::OpenDocument(const BString& path) {
	if (YellowDocsWriterApp* app = dynamic_cast<YellowDocsWriterApp*>(be_app)) {
		app->OpenDocument(path);
	}
}

// Create menu bar

void MainWindow::CreateMenuBar() {
	// Create menu bar
	BMenuBar* menuBar = new BMenuBar("menu_bar");
	
	// File menu
	BMenu* fileMenu = new BMenu("File");
	
	fileMenu->AddItem(new BMenuItem("New", new BMessage(MSG_FILE_NEW), 'N'));
	fileMenu->AddItem(new BMenuItem("Open...", new BMessage(MSG_FILE_OPEN), 'O'));
	fileMenu->AddItem(new BMenuItem("Save", new BMessage(MSG_FILE_SAVE), 'S'));
	fileMenu->AddItem(new BMenuItem("Save As...", new BMessage(MSG_FILE_SAVE_AS)));
	fileMenu->AddItem(new BSeparatorItem());
	fileMenu->AddItem(new BMenuItem("Close", new BMessage(MSG_FILE_CLOSE), 'W'));
	fileMenu->AddItem(new BSeparatorItem());
	fileMenu->AddItem(new BMenuItem("Quit", new BMessage(MSG_FILE_QUIT), 'Q'));
	
	menuBar->AddItem(fileMenu);
	
	// Edit menu
	BMenu* editMenu = new BMenu("Edit");
	
	editMenu->AddItem(new BMenuItem("Undo", new BMessage(MSG_EDIT_UNDO), 'Z'));
	editMenu->AddItem(new BMenuItem("Redo", new BMessage(MSG_EDIT_REDO), 'Y'));
	editMenu->AddItem(new BSeparatorItem());
	editMenu->AddItem(new BMenuItem("Cut", new BMessage(MSG_EDIT_CUT), 'X'));
	editMenu->AddItem(new BMenuItem("Copy", new BMessage(MSG_EDIT_COPY), 'C'));
	editMenu->AddItem(new BMenuItem("Paste", new BMessage(MSG_EDIT_PASTE), 'V'));
	editMenu->AddItem(new BMenuItem("Delete", new BMessage(B_DELETE), B_DELETE));
	editMenu->AddItem(new BSeparatorItem());
	editMenu->AddItem(new BMenuItem("Select All", new BMessage(MSG_EDIT_SELECT_ALL), 'A'));
	editMenu->AddItem(new BSeparatorItem());
	editMenu->AddItem(new BMenuItem("Find...", new BMessage(MSG_EDIT_FIND), 'F'));
	editMenu->AddItem(new BMenuItem("Replace...", new BMessage(MSG_EDIT_REPLACE), 'H'));
	
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
	
	// View menu
	BMenu* viewMenu = new BMenu("View");
	
	viewMenu->AddItem(new BMenuItem("Zoom In", new BMessage(MSG_VIEW_ZOOM_IN)));
	viewMenu->AddItem(new BMenuItem("Zoom Out", new BMessage(MSG_VIEW_ZOOM_OUT)));
	viewMenu->AddItem(new BMenuItem("Reset Zoom", new BMessage(MSG_VIEW_ZOOM_RESET)));
	
	menuBar->AddItem(viewMenu);
	
	// Help menu
	BMenu* helpMenu = new BMenu("Help");
	
	helpMenu->AddItem(new BMenuItem("Help", new BMessage(MSG_HELP_HELP)));
	helpMenu->AddItem(new BMenuItem("About YellowDocs Writer...", new BMessage(MSG_HELP_ABOUT)));
	
	menuBar->AddItem(helpMenu);
	
	// Add menu bar to window
	AddChild(menuBar);
}

// Create toolbar

void MainWindow::CreateToolbar() {
	// Create toolbar box
	BBox* toolbarBox = new BBox("toolbar_box");
	toolbarBox->SetBorder(B_NO_BORDER);
	
	// Create toolbar view
	// TODO: Use custom Toolbar class from UI library
	
	// Add toolbar to window
	BLayoutBuilder::Group<>(this, B_VERTICAL)
		.Add(menuBar)
		.Add(toolbarBox)
		.End();
}

// Create status bar

void MainWindow::CreateStatusBar() {
	// Create status bar
	BStatusBar* statusBar = new BStatusBar("status_bar", "Ready");
	
	// Add status bar to window
	BLayoutBuilder::Group<>(this, B_VERTICAL)
		.Add(statusBar)
		.End();
}

} // namespace Writer
} // namespace YellowDocs
