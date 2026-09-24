#include "PrintView.h"

#include <Font.h>
#include <PrintJob.h>
#include <String.h>

#include "../../libs/core/src/DocumentModel.h"

using namespace YellowDocs::Core;

namespace YellowDocs {
namespace Writer {

// PrintView implementation

PrintView::PrintView(BRect frame, const BString& name, Core::DocumentModel* model)
	: BView(frame, name.String(), B_WILL_DRAW),
	  fDocumentModel(model)
{
}

PrintView::~PrintView() {
}

// Drawing

void PrintView::Draw(BRect frame) {
	// Draw document for printing
	if (!fDocumentModel) {
		return;
	}
	
	// Set up for printing
	SetDrawingMode(B_OP_COPY);
	SetHighColor(0, 0, 0, 255);
	
	// Draw all pages
	int32 pageCount = CountPages();
	
	for (int32 page = 0; page < pageCount; page++) {
		DrawPage(frame, page);
	}
}

// Print

void PrintView::Print(BPrintJob* printJob) {
	if (!printJob || !fDocumentModel) {
		return;
	}
	
	// Set up print job
	printJob->SetScale(1.0f);
	
	// Print document
	printJob->BeginJob();
	
	// Draw to print job
	BRect pageRect = printJob->PrintableRect();
	
	// Draw each page
	int32 pageCount = CountPages();
	
	for (int32 page = 0; page < pageCount; page++) {
		if (page > 0) {
			printJob->NextPage();
		}
		
		DrawPage(pageRect, page);
	}
	
	printJob->EndJob();
}

// Helper methods

void PrintView::DrawPage(BRect frame, int32 pageNumber) {
	if (!fDocumentModel) {
		return;
	}
	
	// Draw document content for this page
	BFont font;
	GetFont(&font);
	
	// Draw text content
	BString text = fDocumentModel->GetAllText();
	
	if (!text.IsEmpty()) {
		// Draw text at origin
		BPoint point(frame.left, frame.top + font.Size() + 2);
		
		// Simple text drawing - in reality this would be more sophisticated
		DrawString(text.String(), point);
	}
}

int32 PrintView::CountPages() {
	// Calculate number of pages needed
	// This is a simplified implementation
	
	if (!fDocumentModel) {
		return 1;
	}
	
	// For now, return 1 page
	// In reality, this would calculate based on document content and page size
	return 1;
}

} // namespace Writer
} // namespace YellowDocs
