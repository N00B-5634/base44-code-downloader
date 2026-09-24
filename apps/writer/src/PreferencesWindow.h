#ifndef YELLOWDOCS_WRITER_PREFERENCES_WINDOW_H
#define YELLOWDOCS_WRITER_PREFERENCES_WINDOW_H

#include <Window.h>
#include <String.h>

namespace YellowDocs {
namespace Writer {

// Preferences window for YellowDocs Writer
class PreferencesWindow : public BWindow {
public:
	PreferencesWindow(BRect frame);
	~PreferencesWindow();

	// Window lifecycle
	void MessageReceived(BMessage* message) override;
	bool QuitRequested() override;

	// Preferences categories
	enum PreferencesCategory {
		PREF_GENERAL,
		PREF_EDITOR,
		PREF_APPEARANCE,
		PREF_PRINTING
	};

	// Show specific category
	void ShowCategory(PreferencesCategory category);

private:
	PreferencesCategory fCurrentCategory;
	
	// Create UI
	void CreateUI();
	
	// Create category list
	void CreateCategoryList();
	
	// Create general preferences
	void CreateGeneralPreferences();
	
	// Create editor preferences
	void CreateEditorPreferences();
	
	// Create appearance preferences
	void CreateAppearancePreferences();
	
	// Create printing preferences
	void CreatePrintingPreferences();
	
	// Save preferences
	void SavePreferences();
	
	// Message constants
	enum {
		MSG_PREF_CATEGORY_CHANGED = 'pcca',
		MSG_PREF_SAVE = 'psav',
		MSG_PREF_CANCEL = 'pcan'
	};
};

} // namespace Writer
} // namespace YellowDocs

#endif // YELLOWDOCS_WRITER_PREFERENCES_WINDOW_H
