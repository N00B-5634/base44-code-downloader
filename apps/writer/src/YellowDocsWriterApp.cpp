#include "YellowDocsWriterApp.h"

#include <Alert.h>
#include <AboutWindow.h>
#include <Entry.h>
#include <File.h>
#include <FindDirectory.h>
#include <Message.h>
#include <Mime.h>
#include <Path.h>
#include <Roster.h>
#include <String.h>

#include "MainWindow.h"
#include "DocumentWindow.h"
#include "AboutWindow.h"

#include "../../libs/utils/src/MIMEUtils.h"

using namespace YellowDocs::Core;
using namespace YellowDocs::UI;

namespace YellowDocs {
namespace Writer {

// Application instance
static YellowDocsWriterApp* sAppInstance = nullptr;

YellowDocsWriterApp::YellowDocsWriterApp()
	: BApplication(kAppSignature),
	  fMainWindow(nullptr)
{
	sAppInstance = this;
	
	// Initialize application
	Initialize();
}

YellowDocsWriterApp::~YellowDocsWriterApp() {
	// Clean up windows
	CloseAllDocuments();
	
	delete fMainWindow;
	fMainWindow = nullptr;
	
	sAppInstance = nullptr;
}

// Application lifecycle

void YellowDocsWriterApp::ReadyToRun() {
	// Create main window
	CreateMainWindow();
	
	// Register MIME types
	RegisterMIMETypes();
	
	// Show main window
	if (fMainWindow) {
		fMainWindow->Show();
	}
	
	// Open a new document if no files were specified
	if (fDocumentWindows.CountItems() == 0) {
		NewDocument();
	}
}

void YellowDocsWriterApp::AboutRequested() {
	ShowAbout();
}

bool YellowDocsWriterApp::QuitRequested() {
	// Check if all documents can be closed
	for (int32 i = 0; i < fDocumentWindows.CountItems(); i++) {
		DocumentWindow* window = fDocumentWindows.ItemAt(i);
		if (window && window->IsModified()) {
			// Ask to save changes
			BAlert* alert = new BAlert("Save Changes",
				"You have unsaved changes. Save before quitting?",
				"Save", "Don't Save", "Cancel", B_WIDTH_AS_USUAL, B_OFFSET_HORIZONTAL);
			
			int32 choice = alert->Go();
			
			if (choice == 0) {
				// Save
				SaveDocument(window);
			} else if (choice == 2) {
				// Cancel
				return false;
			}
			// Don't Save - continue with quit
		}
	}
	
	return BApplication::QuitRequested();
}

void YellowDocsWriterApp::MessageReceived(BMessage* message) {
	switch (message->what) {
		case MSG_NEW_DOCUMENT:
			NewDocument();
			break;
		
		case MSG_OPEN_DOCUMENT:
			// Show file open panel
			if (fMainWindow) {
				fMainWindow->ShowOpenPanel();
			}
			break;
		
		case MSG_SAVE_DOCUMENT:
			// Save active document
			if (DocumentWindow* window = GetActiveDocumentWindow()) {
				SaveDocument(window);
			}
			break;
		
		case MSG_SAVE_AS_DOCUMENT:
			// Save active document as
			if (DocumentWindow* window = GetActiveDocumentWindow()) {
				// Show save panel
				window->ShowSavePanel();
			}
			break;
		
		case MSG_CLOSE_DOCUMENT:
			// Close active document
			if (DocumentWindow* window = GetActiveDocumentWindow()) {
				window->PostMessage(B_QUIT_REQUESTED);
			}
			break;
		
		case MSG_PREFERENCES:
			ShowPreferences();
			break;
		
		case MSG_HELP:
			ShowHelp();
			break;
		
		case MSG_ABOUT:
			ShowAbout();
			break;
		
		case B_OPEN:
			HandleOpenFile(message);
			break;
		
		default:
			BApplication::MessageReceived(message);
			break;
	}
}

void YellowDocsWriterApp::RefReceived(BMessage* message) {
	// Handle file refs (double-click on file in Tracker)
	HandleOpenRef(new BEntry(message));
}

void YellowDocsWriterApp::ArgvReceived(int32 argc, char** argv) {
	// Handle command line arguments
	for (int32 i = 1; i < argc; i++) {
		BString arg = argv[i];
		
		// Check if it's a file path
		BEntry entry(arg.String());
		if (entry.Exists() && entry.IsFile()) {
			OpenDocument(arg);
		}
	}
}

// Window management

MainWindow* YellowDocsWriterApp::GetMainWindow() const {
	return fMainWindow;
}

DocumentWindow* YellowDocsWriterApp::GetActiveDocumentWindow() const {
	// Return the frontmost document window
	for (int32 i = 0; i < fDocumentWindows.CountItems(); i++) {
		DocumentWindow* window = fDocumentWindows.ItemAt(i);
		if (window && window->IsActive()) {
			return window;
		}
	}
	
	// Return the first document window
	if (fDocumentWindows.CountItems() > 0) {
		return fDocumentWindows.ItemAt(0);
	}
	
	return nullptr;
}

void YellowDocsWriterApp::OpenNewDocument() {
	NewDocument();
}

void YellowDocsWriterApp::OpenDocument(const BString& path) {
	BEntry entry(path.String());
	if (entry.Exists() && entry.IsFile()) {
		OpenDocumentFile(path);
	}
}

void YellowDocsWriterApp::CloseAllDocuments() {
	// Close all document windows
	while (fDocumentWindows.CountItems() > 0) {
		DocumentWindow* window = fDocumentWindows.RemoveItemAt(0);
		if (window) {
			window->PostMessage(B_QUIT_REQUESTED);
			delete window;
		}
	}
}

// Document management

void YellowDocsWriterApp::NewDocument() {
	// Create new document window
	DocumentWindow* window = new DocumentWindow(BRect(100, 100, 600, 400));
	
	if (window) {
		window->Show();
		fDocumentWindows.Add(window);
	}
}

bool YellowDocsWriterApp::SaveDocument(DocumentWindow* window) {
	if (!window) {
		return false;
	}
	
	BString path = window->DocumentPath();
	
	if (path.IsEmpty()) {
		// No path, show save panel
		return window->ShowSavePanel();
	}
	
	return window->SaveDocument(path);
}

bool YellowDocsWriterApp::SaveDocumentAs(DocumentWindow* window, const BString& path) {
	if (!window) {
		return false;
	}
	
	return window->SaveDocument(path);
}

bool YellowDocsWriterApp::OpenDocumentFile(const BString& path) {
	// Check if file exists
	BEntry entry(path.String());
	if (!entry.Exists() || !entry.IsFile()) {
		return false;
	}
	
	// Check MIME type
	BAppFileInfo appInfo(&entry);
	char mimeType[B_MIME_TYPE_LENGTH];
	
	if (appInfo.GetType(mimeType) != B_OK) {
		// Try to open anyway
		DocumentWindow* window = new DocumentWindow(BRect(100, 100, 600, 400));
		
		if (window) {
			if (window->LoadDocument(path)) {
				window->Show();
				fDocumentWindows.Add(window);
				return true;
			} else {
				delete window;
			}
		}
		
		return false;
	}
	
	// Check if it's a YellowDocs Writer document
	if (strcmp(mimeType, kDocumentMIMEType) == 0 || 
		strcmp(mimeType, "text/plain") == 0) {
		
		DocumentWindow* window = new DocumentWindow(BRect(100, 100, 600, 400));
		
		if (window) {
			if (window->LoadDocument(path)) {
				window->Show();
				fDocumentWindows.Add(window);
				return true;
			} else {
				delete window;
			}
		}
	}
	
	return false;
}

// Preferences

void YellowDocsWriterApp::ShowPreferences() {
	// TODO: Implement preferences window
	BAlert* alert = new BAlert("Preferences",
		"Preferences window will be implemented in a future version.",
		"OK", nullptr, nullptr, B_WIDTH_AS_USUAL, B_OFFSET_HORIZONTAL);
	alert->Go();
}

// Help

void YellowDocsWriterApp::ShowHelp() {
	// TODO: Implement help
	BAlert* alert = new BAlert("Help",
		"Help will be available in a future version.",
		"OK", nullptr, nullptr, B_WIDTH_AS_USUAL, B_OFFSET_HORIZONTAL);
	alert->Go();
}

void YellowDocsWriterApp::ShowAbout() {
	// Create about window
	AboutWindow* about = new AboutWindow(kAppName, kAppSignature);
	
	if (about) {
		about->AddDescription("YellowDocs Writer - A native Haiku OS word processor");
		about->AddVersion("Version 1.0.0");
		about->AddCopyright("Copyright 2024 YellowDocs Contributors");
		about->AddAuthors("YellowDocs Team");
		
		// Add custom text
		BString aboutText;
		aboutText << "\n\n";
		aboutText << "YellowDocs Writer is part of the YellowDocs office suite.\n";
		aboutText << "It provides a native, lightweight word processor for Haiku OS.\n\n";
		aboutText << "Features:\n";
		aboutText << "- Rich text editing\n";
		aboutText << "- Native Haiku UI\n";
		aboutText << "- Fast and responsive\n";
		aboutText << "- Local-first design\n";
		
		about->AddText(aboutText);
		
		about->Show();
	}
}

// Initialize application

void YellowDocsWriterApp::Initialize() {
	// Set application info
	BString appName = kAppName;
	appName << " (" << kAppVersion << ")";
	
	// Initialize theme
	GetTheme().Initialize();
}

// Register MIME types

void YellowDocsWriterApp::RegisterMIMETypes() {
	// Register YellowDocs Writer MIME type
	Utils::RegisterYellowDocsWriterMIMEType();
	
	// Register with Tracker
	Utils::RegisterFileTypeWithTracker(
		kDocumentMIMEType,
		kDocumentExtension,
		"YellowDocs Writer Document",
		kAppSignature
	);
}

// Create main window

void YellowDocsWriterApp::CreateMainWindow() {
	// Create main window (hidden by default)
	fMainWindow = new MainWindow(BRect(0, 0, 400, 300));
	
	if (fMainWindow) {
		fMainWindow->Hide();
	}
}

// Handle file opening

void YellowDocsWriterApp::HandleOpenFile(const BMessage* message) {
	// Handle file open from menu
	entry_ref ref;
	
	for (int32 i = 0; message->FindRef("refs", i, &ref) == B_OK; i++) {
		BEntry entry(&ref);
		HandleOpenRef(&entry);
	}
}

void YellowDocsWriterApp::HandleOpenRef(const BEntry* entry) {
	if (!entry) {
		return;
	}
	
	BPath path;
	entry->GetPath(&path);
	
	if (path.Path()) {
		OpenDocument(path.Path());
	}
}

} // namespace Writer
} // namespace YellowDocs
