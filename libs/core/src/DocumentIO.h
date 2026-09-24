#ifndef YELLOWDOCS_CORE_DOCUMENT_IO_H
#define YELLOWDOCS_CORE_DOCUMENT_IO_H

#include <SupportDefs.h>
#include <String.h>
#include <File.h>
#include <DataIO.h>
#include <GraphicsDefs.h>
#include "DocumentModel.h"
#include "TextStyle.h"
#include "ParagraphStyle.h"

namespace YellowDocs {
namespace Core {

// Forward declaration
class DocumentModel;

class DocumentIO {
public:
	DocumentIO();
	~DocumentIO();

	// Load document from file
	status_t LoadFromFile(DocumentModel* document, const BString& path);
	
	// Save document to file
	status_t SaveToFile(DocumentModel* document, const BString& path);
	
	// Load document from data
	status_t LoadFromData(DocumentModel* document, const uint8* data, size_t size);
	status_t LoadFromData(DocumentModel* document, BDataIO* data);
	
	// Save document to data
	status_t SaveToData(DocumentModel* document, uint8** data, size_t* size);
	status_t SaveToData(DocumentModel* document, BDataIO* data);
	
	// Check if file is valid YellowDocs Writer document
	bool IsValidDocumentFile(const BString& path);
	
	// Get document format version
	uint32 GetFormatVersion() const;
	
private:
	// File format constants
	static const uint32 kMagicNumber = 0x59445752; // "YDWR" in hex
	static const uint32 kFormatVersion = 1;
	
	// Helper methods
	status_t ReadHeader(BDataIO* data, uint32* magic, uint32* version);
	status_t WriteHeader(BDataIO* data);
	
	status_t ReadDocumentInfo(BDataIO* data, DocumentModel* document);
	status_t WriteDocumentInfo(BDataIO* data, DocumentModel* document);
	
	status_t ReadPageSettings(BDataIO* data, DocumentModel* document);
	status_t WritePageSettings(BDataIO* data, DocumentModel* document);
	
	status_t ReadElements(BDataIO* data, DocumentModel* document);
	status_t WriteElements(BDataIO* data, DocumentModel* document);
	
	status_t ReadTextElement(BDataIO* data, DocumentModel* document);
	status_t WriteTextElement(BDataIO* data, DocumentElement* element);
	
	status_t ReadParagraphElement(BDataIO* data, DocumentModel* document);
	status_t WriteParagraphElement(BDataIO* data, DocumentElement* element);
	
	status_t ReadImageElement(BDataIO* data, DocumentModel* document);
	status_t WriteImageElement(BDataIO* data, DocumentElement* element);
	
	status_t ReadTableElement(BDataIO* data, DocumentModel* document);
	status_t WriteTableElement(BDataIO* data, DocumentElement* element);
	
	// Read/write style
	status_t ReadTextStyle(BDataIO* data, TextStyle* style);
	status_t WriteTextStyle(BDataIO* data, const TextStyle* style);
	
	status_t ReadParagraphStyle(BDataIO* data, ParagraphStyle* style);
	status_t WriteParagraphStyle(BDataIO* data, const ParagraphStyle* style);
	
	// Read/write color
	status_t ReadColor(BDataIO* data, rgb_color* color);
	status_t WriteColor(BDataIO* data, rgb_color color);
	
	// Read/write string
	status_t ReadString(BDataIO* data, BString* string);
	status_t WriteString(BDataIO* data, const BString& string);
};

} // namespace Core
} // namespace YellowDocs

#endif // YELLOWDOCS_CORE_DOCUMENT_IO_H
