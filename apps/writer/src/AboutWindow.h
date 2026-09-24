#ifndef YELLOWDOCS_WRITER_ABOUT_WINDOW_H
#define YELLOWDOCS_WRITER_ABOUT_WINDOW_H

#include <AboutWindow.h>
#include <String.h>

namespace YellowDocs {
namespace Writer {

// Custom about window for YellowDocs Writer
class AboutWindow : public BAboutWindow {
public:
	AboutWindow(const char* appName, const char* appSignature);
	~AboutWindow();

	// Add custom content
	void AddDescription(const BString& description);
	void AddVersion(const BString& version);
	void AddCopyright(const BString& copyright);
	void AddAuthors(const BString& authors);
	void AddText(const BString& text);

private:
	BString fDescription;
	BString fVersion;
	BString fCopyright;
	BString fAuthors;
	BString fExtraText;
};

} // namespace Writer
} // namespace YellowDocs

#endif // YELLOWDOCS_WRITER_ABOUT_WINDOW_H
