#include "StatusView.h"

#include <String.h>

#include "../../libs/ui/src/Theme.h"

using namespace YellowDocs::UI;

namespace YellowDocs {
namespace Writer {

// StatusView implementation

StatusView::StatusView(const BString& name)
	: StatusBar(name),
	  fCursorLine(1),
	  fCursorColumn(1),
	  fSelectionStart(0),
	  fSelectionEnd(0)
{
	// Initialize status fields
	AddField("status", "Ready", STATUS_ALIGN_LEFT, 200);
	AddField("document", "Untitled", STATUS_ALIGN_CENTER, 200);
	AddField("position", "Line: 1, Column: 1", STATUS_ALIGN_RIGHT, 200);
}

StatusView::~StatusView() {
}

// Update status

void StatusView::UpdateStatus() {
	// Update all status fields
	SetFieldText("status", fStatusText);
	SetFieldText("document", fDocumentInfo);
	
	BString position;
	position << "Line: " << fCursorLine << ", Column: " << fCursorColumn;
	
	if (fSelectionStart != fSelectionEnd) {
		position << " | Selected: " << (fSelectionEnd - fSelectionStart) << " chars";
	}
	
	SetFieldText("position", position);
	
	Invalidate();
}

// Set status text

void StatusView::SetStatusText(const BString& text) {
	fStatusText = text;
	UpdateStatus();
}

// Set document info

void StatusView::SetDocumentInfo(const BString& info) {
	fDocumentInfo = info;
	UpdateStatus();
}

// Set cursor position

void StatusView::SetCursorPosition(int32 line, int32 column) {
	fCursorLine = line;
	fCursorColumn = column;
	UpdateStatus();
}

// Set selection info

void StatusView::SetSelectionInfo(int32 start, int32 end) {
	fSelectionStart = start;
	fSelectionEnd = end;
	UpdateStatus();
}

} // namespace Writer
} // namespace YellowDocs
