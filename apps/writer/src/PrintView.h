#ifndef YELLOWDOCS_WRITER_PRINT_VIEW_H
#define YELLOWDOCS_WRITER_PRINT_VIEW_H

#include <View.h>
#include <String.h>
#include <PrintJob.h>

namespace YellowDocs {
namespace Core {
	class DocumentModel;
}

namespace Writer {

// Print view for YellowDocs Writer
class PrintView : public BView {
public:
	PrintView(BRect frame, const BString& name, Core::DocumentModel* model);
	~PrintView();

	// Drawing
	void Draw(BRect frame) override;

	// Print
	void Print(BPrintJob* printJob);

private:
	Core::DocumentModel* fDocumentModel;
	
	// Helper methods
	void DrawPage(BRect frame, int32 pageNumber);
	int32 CountPages();
};

} // namespace Writer
} // namespace YellowDocs

#endif // YELLOWDOCS_WRITER_PRINT_VIEW_H
