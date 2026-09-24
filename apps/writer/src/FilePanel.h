#ifndef YELLOWDOCS_WRITER_FILE_PANEL_H
#define YELLOWDOCS_WRITER_FILE_PANEL_H

#include <FilePanel.h>
#include <String.h>

namespace YellowDocs {
namespace Writer {

// Custom file panel for YellowDocs Writer
class FilePanel : public BFilePanel {
public:
	FilePanel(const BString& title, const BString& defaultPath = "",
		uint32 mode = B_OPEN_PANEL, BMessenger* target = nullptr);
	~FilePanel();

	// Set default directory
	void SetDefaultDirectory(const BString& path);
	
	// Set default filename
	void SetDefaultFilename(const BString& filename);
	
	// Set file filters
	void SetFileFilters(const BVector<BString>& extensions);
	
	// Set MIME type filter
	void SetMIMETypeFilter(const BString& mimeType);

private:
	BString fDefaultPath;
	BString fDefaultFilename;
};

} // namespace Writer
} // namespace YellowDocs

#endif // YELLOWDOCS_WRITER_FILE_PANEL_H
