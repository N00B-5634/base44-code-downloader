#ifndef YELLOWDOCS_WRITER_MAIN_WINDOW_H
#define YELLOWDOCS_WRITER_MAIN_WINDOW_H

#include <Window.h>
#include <String.h>

namespace YellowDocs {
namespace Writer {

// Forward declarations
class DocumentWindow;

class MainWindow : public BWindow {
public:
	MainWindow(BRect frame);
	~MainWindow();

	// Window lifecycle
	void MessageReceived(BMessage* message) override;
	bool QuitRequested() override;

	// File operations
	void ShowOpenPanel();
	void ShowSavePanel();
	void ShowPreferences();
	void ShowAbout();
	void ShowHelp();

	// Document management
	void NewDocument();
	void OpenDocument(const BString& path);
	
private:
	// Create menu bar
	void CreateMenuBar();
	
	// Create toolbar
	void CreateToolbar();
	
	// Create status bar
	void CreateStatusBar();
	
	// Message constants
	enum {
		MSG_FILE_NEW = 'fnw '
		MSG_FILE_OPEN = 'fopn',
		MSG_FILE_SAVE = 'fsv '
		MSG_FILE_SAVE_AS = 'fsva',
		MSG_FILE_CLOSE = 'fcls',
		MSG_FILE_QUIT = 'fqu '
		MSG_EDIT_UNDO = 'eund',
		MSG_EDIT_REDO = 'erd ', 
		MSG_EDIT_CUT = 'ecut',
		MSG_EDIT_COPY = 'ecpy',
		MSG_EDIT_PASTE = 'epst',
		MSG_EDIT_SELECT_ALL = 'esal',
		MSG_EDIT_FIND = 'efnd',
		MSG_EDIT_REPLACE = 'erpl',
		MSG_FORMAT_BOLD = 'fmtb',
		MSG_FORMAT_ITALIC = 'fmti',
		MSG_FORMAT_UNDERLINE = 'ftul',
		MSG_FORMAT_ALIGN_LEFT = 'faln',
		MSG_FORMAT_ALIGN_CENTER = 'facn',
		MSG_FORMAT_ALIGN_RIGHT = 'farn',
		MSG_FORMAT_ALIGN_JUSTIFY = 'fajn',
		MSG_VIEW_ZOOM_IN = 'vzmi',
		MSG_VIEW_ZOOM_OUT = 'vzmo',
		MSG_VIEW_ZOOM_RESET = 'vzmr',
		MSG_HELP_ABOUT = 'habt',
		MSG_HELP_HELP = 'hlp '
	};
};

} // namespace Writer
} // namespace YellowDocs

#endif // YELLOWDOCS_WRITER_MAIN_WINDOW_H
