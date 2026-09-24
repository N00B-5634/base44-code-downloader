#include "AboutWindow.h"

#include <AboutWindow.h>
#include <Bitmap.h>
#include <Box.h>
#include <Button.h>
#include <LayoutBuilder.h>
#include <Picture.h>
#include <StringView.h>
#include <TextView.h>

namespace YellowDocs {
namespace Writer {

// AboutWindow implementation

AboutWindow::AboutWindow(const char* appName, const char* appSignature)
	: BAboutWindow(appName, appSignature)
{
}

AboutWindow::~AboutWindow() {
}

// Add custom content

void AboutWindow::AddDescription(const BString& description) {
	fDescription = description;
	
	// Add description to about window
	BStringView* descView = new BStringView("description", description.String());
	AddChild(descView);
}

void AboutWindow::AddVersion(const BString& version) {
	fVersion = version;
	
	// Add version to about window
	BStringView* versionView = new BStringView("version", version.String());
	AddChild(versionView);
}

void AboutWindow::AddCopyright(const BString& copyright) {
	fCopyright = copyright;
	
	// Add copyright to about window
	BStringView* copyrightView = new BStringView("copyright", copyright.String());
	AddChild(copyrightView);
}

void AboutWindow::AddAuthors(const BString& authors) {
	fAuthors = authors;
	
	// Add authors to about window
	BStringView* authorsView = new BStringView("authors", authors.String());
	AddChild(authorsView);
}

void AboutWindow::AddText(const BString& text) {
	fExtraText = text;
	
	// Add text to about window
	BTextView* textView = new BTextView("extra_text", text.String(), B_WORD_WRAP);
	textView->MakeEditable(false);
	textView->MakeSelectable(false);
	textView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	
	BLayoutBuilder::Group<>(this, B_VERTICAL)
		.Add(textView)
		.End();
}

} // namespace Writer
} // namespace YellowDocs
