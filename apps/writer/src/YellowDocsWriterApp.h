#ifndef YELLOWDOCS_WRITER_APP_H
#define YELLOWDOCS_WRITER_APP_H

#include <Application.h>
#include <String.h>
#include <Vector.h>

namespace YellowDocs {
namespace Writer {

// Forward declarations
class MainWindow;
class DocumentWindow;

// Application constants
const char* const kAppName = "YellowDocs Writer";
const char* const kAppSignature = "application/x-vnd.yellowdocs-writer-app";
const char* const kAppMIMEType = "application/x-vnd.yellowdocs-writer-app";
const char* const kDocumentMIMEType = "application/x-vnd.yellowdocs-writer";
const char* const kDocumentExtension = ".ywrite";

class YellowDocsWriterApp : public BApplication {
public:
	YellowDocsWriterApp();
	~YellowDocsWriterApp();

	// Application lifecycle
	void ReadyToRun() override;
	void AboutRequested() override;
	bool QuitRequested() override;
	void MessageReceived(BMessage* message) override;
	void RefReceived(BMessage* message) override;
	void ArgvReceived(int32 argc, char** argv) override;

	// Window management
	MainWindow* GetMainWindow() const;
	DocumentWindow* GetActiveDocumentWindow() const;
	void OpenNewDocument();
	void OpenDocument(const BString& path);
	void CloseAllDocuments();

	// Document management
	void NewDocument();
	bool SaveDocument(DocumentWindow* window);
	bool SaveDocumentAs(DocumentWindow* window, const BString& path);
	bool OpenDocumentFile(const BString& path);

	// Preferences
	void ShowPreferences();

	// Help
	void ShowHelp();
	void ShowAbout();

private:
	MainWindow* fMainWindow;
	BVector<DocumentWindow*> fDocumentWindows;
	BString fLastOpenPath;
	
	// Initialize application
	void Initialize();
	
	// Register MIME types
	void RegisterMIMETypes();
	
	// Create main window
	void CreateMainWindow();
	
	// Handle file opening
	void HandleOpenFile(const BMessage* message);
	void HandleOpenRef(const BEntry* entry);
	
	// Message constants
	enum {
		MSG_NEW_DOCUMENT = 'newd',
		MSG_OPEN_DOCUMENT = 'opnd',
		MSG_SAVE_DOCUMENT = 'sved',
		MSG_SAVE_AS_DOCUMENT = 'svad',
		MSG_CLOSE_DOCUMENT = 'clsd',
		MSG_EXIT = 'exit',
		MSG_ABOUT = 'abou',
		MSG_PREFERENCES = 'pref',
		MSG_HELP = 'help',
		MSG_OPEN_REF = 'oref'
	};
};

} // namespace Writer
} // namespace YellowDocs

#endif // YELLOWDOCS_WRITER_APP_H
