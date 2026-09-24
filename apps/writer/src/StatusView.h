#ifndef YELLOWDOCS_WRITER_STATUS_VIEW_H
#define YELLOWDOCS_WRITER_STATUS_VIEW_H

#include <View.h>
#include <String.h>

#include "../../libs/ui/src/StatusBar.h"

namespace YellowDocs {
namespace Writer {

// Status view for YellowDocs Writer
class StatusView : public YellowDocs::UI::StatusBar {
public:
	StatusView(const BString& name = "StatusView");
	~StatusView();

	// Update status
	void UpdateStatus();

	// Set status text
	void SetStatusText(const BString& text);
	
	// Set document info
	void SetDocumentInfo(const BString& info);
	
	// Set cursor position
	void SetCursorPosition(int32 line, int32 column);
	
	// Set selection info
	void SetSelectionInfo(int32 start, int32 end);

private:
	BString fStatusText;
	BString fDocumentInfo;
	int32 fCursorLine;
	int32 fCursorColumn;
	int32 fSelectionStart;
	int32 fSelectionEnd;
};

} // namespace Writer
} // namespace YellowDocs

#endif // YELLOWDOCS_WRITER_STATUS_VIEW_H
