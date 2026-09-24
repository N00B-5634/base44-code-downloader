#include "FilePanel.h"

#include <Message.h>
#include <Path.h>
#include <String.h>

namespace YellowDocs {
namespace Writer {

// FilePanel implementation

FilePanel::FilePanel(const BString& title, const BString& defaultPath,
		uint32 mode, BMessenger* target)
	: BFilePanel(mode, target, nullptr, B_FILE_PANEL_OPEN, false),
	  fDefaultPath(defaultPath),
	  fDefaultFilename("")
{
	SetTitle(title.String());
	
	if (!defaultPath.IsEmpty()) {
		BPath path(defaultPath.String());
		BEntry entry(path.Path());
		if (entry.Exists() && entry.IsDirectory()) {
			SetPanelDirectory(path.Path());
		}
	}
}

FilePanel::~FilePanel() {
}

// Set default directory

void FilePanel::SetDefaultDirectory(const BString& path) {
	fDefaultPath = path;
	
	BPath bpath(path.String());
	BEntry entry(bpath.Path());
	if (entry.Exists() && entry.IsDirectory()) {
		SetPanelDirectory(bpath.Path());
	}
}

// Set default filename

void FilePanel::SetDefaultFilename(const BString& filename) {
	fDefaultFilename = filename;
	SetSaveText(filename.String());
}

// Set file filters

void FilePanel::SetFileFilters(const BVector<BString>& extensions) {
	// Create filter string
	BString filter;
	
	for (int32 i = 0; i < extensions.CountItems(); i++) {
		if (i > 0) {
			filter << ";";
		}
		filter << "*" << extensions.ItemAt(i);
	}
	
	// Set file types
	BMessage message(B_SET_PROPERTY);
	message.AddString("text", "YellowDocs Writer Documents");
	message.AddString("filetypes", filter.String());
	
	// Note: BFilePanel doesn't have a direct way to set file filters
	// This would need to be handled through the ref_filter mechanism
}

// Set MIME type filter

void FilePanel::SetMIMETypeFilter(const BString& mimeType) {
	// Set MIME type filter
	BMessage message(B_SET_PROPERTY);
	message.AddString("mimetype", mimeType.String());
	
	// Note: BFilePanel doesn't have a direct way to set MIME type filters
	// This would need to be handled through the ref_filter mechanism
}

} // namespace Writer
} // namespace YellowDocs
