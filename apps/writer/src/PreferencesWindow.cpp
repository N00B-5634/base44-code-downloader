#include "PreferencesWindow.h"

#include <Application.h>
#include <Box.h>
#include <Button.h>
#include <CheckBox.h>
#include <LayoutBuilder.h>
#include <ListView.h>
#include <Message.h>
#include <RadioButton.h>
#include <ScrollView.h>
#include <SeparatorView.h>
#include <StringView.h>
#include <TextControl.h>

#include "YellowDocsWriterApp.h"

using namespace YellowDocs::Core;
using namespace YellowDocs::UI;

namespace YellowDocs {
namespace Writer {

// PreferencesWindow implementation

PreferencesWindow::PreferencesWindow(BRect frame)
	: BWindow(frame, "Preferences - YellowDocs Writer", B_TITLED_WINDOW,
		B_ASYNCHRONOUS_CONTROLS | B_QUIT_ON_WINDOW_CLOSE | B_AUTO_UPDATE_SIZE_LIMITS),
	  fCurrentCategory(PREF_GENERAL)
{
	// Set window feel
	SetFeel(B_NORMAL_WINDOW_FEEL);
	
	// Create UI
	CreateUI();
	
	// Center window
	CenterOnScreen();
}

PreferencesWindow::~PreferencesWindow() {
}

// Window lifecycle

void PreferencesWindow::MessageReceived(BMessage* message) {
	switch (message->what) {
		case MSG_PREF_CATEGORY_CHANGED:
			// Category changed
			{
				int32 category;
				if (message->FindInt32("category", &category) == B_OK) {
					ShowCategory(static_cast<PreferencesCategory>(category));
				}
			}
			break;
		
		case MSG_PREF_SAVE:
			SavePreferences();
			PostMessage(B_QUIT_REQUESTED);
			break;
		
		case MSG_PREF_CANCEL:
			PostMessage(B_QUIT_REQUESTED);
			break;
		
		default:
			BWindow::MessageReceived(message);
			break;
	}
}

bool PreferencesWindow::QuitRequested() {
	// Hide instead of quit
	Hide();
	return false;
}

// Show specific category

void PreferencesWindow::ShowCategory(PreferencesCategory category) {
	fCurrentCategory = category;
	
	// Update UI based on category
	// This will be implemented when we have the full preferences UI
}

// Create UI

void PreferencesWindow::CreateUI() {
	// Create main layout
	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.SetInsets(10, 10, 10, 10)
		.AddGroup(B_HORIZONTAL, 10)
			.Add(CreateCategoryList())
			.AddGroup(B_VERTICAL, 10)
				.Add(CreateGeneralPreferences())
				.Add(CreateEditorPreferences())
				.Add(CreateAppearancePreferences())
				.Add(CreatePrintingPreferences())
				.End()
			.End()
			.AddGroup(B_HORIZONTAL, 10)
				.AddGlue()
				.Add(new BButton("save", "Save", new BMessage(MSG_PREF_SAVE)))
				.Add(new BButton("cancel", "Cancel", new BMessage(MSG_PREF_CANCEL)))
				.End()
		.End();
}

// Create category list

BView* PreferencesWindow::CreateCategoryList() {
	BListView* categoryList = new BListView("category_list", B_SINGLE_SELECTION_LIST);
	categoryList->SetSelectionMessage(new BMessage(MSG_PREF_CATEGORY_CHANGED));
	
	// Add categories
	categoryList->AddItem(new BStringItem("General"));
	categoryList->AddItem(new BStringItem("Editor"));
	categoryList->AddItem(new BStringItem("Appearance"));
	categoryList->AddItem(new BStringItem("Printing"));
	
	// Select first category
	categoryList->Select(0);
	
	return new BScrollView("category_scroll", categoryList, B_FRAME_EVENTS, false, true);
}

// Create general preferences

BView* PreferencesWindow::CreateGeneralPreferences() {
	BBox* generalBox = new BBox("general_box");
	generalBox->SetLabel("General");
	
	BLayoutBuilder::Group<>(generalBox, B_VERTICAL, 10)
		.Add(new BCheckBox("auto_save", "Auto save", new BMessage()))
		.Add(new BTextControl("default_path", "Default path:", ""))
		.End();
	
	return generalBox;
}

// Create editor preferences

BView* PreferencesWindow::CreateEditorPreferences() {
	BBox* editorBox = new BBox("editor_box");
	editorBox->SetLabel("Editor");
	
	BLayoutBuilder::Group<>(editorBox, B_VERTICAL, 10)
		.Add(new BCheckBox("word_wrap", "Word wrap", new BMessage()))
		.Add(new BCheckBox("show_line_numbers", "Show line numbers", new BMessage()))
		.Add(new BCheckBox("show_margin", "Show margin", new BMessage()))
		.Add(new BSlider("margin_size", "Margin size:", new BMessage(), 0, 100, 10))
		.End();
	
	return editorBox;
}

// Create appearance preferences

BView* PreferencesWindow::CreateAppearancePreferences() {
	BBox* appearanceBox = new BBox("appearance_box");
	appearanceBox->SetLabel("Appearance");
	
	BLayoutBuilder::Group<>(appearanceBox, B_VERTICAL, 10)
		.Add(new BRadioButton("theme_system", "System theme", new BMessage()))
		.Add(new BRadioButton("theme_light", "Light theme", new BMessage()))
		.Add(new BRadioButton("theme_dark", "Dark theme", new BMessage()))
		.Add(new BColorControl("accent_color", "Accent color:", new BMessage(), B_WILL_DRAW))
		.End();
	
	return appearanceBox;
}

// Create printing preferences

BView* PreferencesWindow::CreatePrintingPreferences() {
	BBox* printingBox = new BBox("printing_box");
	printingBox->SetLabel("Printing");
	
	BLayoutBuilder::Group<>(printingBox, B_VERTICAL, 10)
		.Add(new BCheckBox("print_header", "Print header", new BMessage()))
		.Add(new BCheckBox("print_footer", "Print footer", new BMessage()))
		.Add(new BCheckBox("print_page_numbers", "Print page numbers", new BMessage()))
		.End();
	
	return printingBox;
}

// Save preferences

void PreferencesWindow::SavePreferences() {
	// Save preferences to file
	// This will be implemented when we have the full preferences system
}

} // namespace Writer
} // namespace YellowDocs
