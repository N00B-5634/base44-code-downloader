#include "DocumentIO.h"

#include <new>
#include <string.h>

#include "DocumentModel.h"
#include "TextStyle.h"
#include "ParagraphStyle.h"
#include "ImageData.h"

namespace YellowDocs {
namespace Core {

// DocumentIO implementation

DocumentIO::DocumentIO() {
}

DocumentIO::~DocumentIO() {
}

// Load document from file

status_t DocumentIO::LoadFromFile(DocumentModel* document, const BString& path) {
	if (!document) {
		return B_BAD_VALUE;
	}
	
	BFile file(path.String(), B_READ_ONLY);
	if (file.InitCheck() != B_OK) {
		return file.InitCheck();
	}
	
	BDataIO dataIO(&file, B_READ_ONLY);
	return LoadFromData(document, dataIO);
}

// Save document to file

status_t DocumentIO::SaveToFile(DocumentModel* document, const BString& path) {
	if (!document) {
		return B_BAD_VALUE;
	}
	
	BFile file(path.String(), B_WRITE_ONLY | B_CREATE_FILE | B_ERASE_FILE);
	if (file.InitCheck() != B_OK) {
		return file.InitCheck();
	}
	
	BDataIO dataIO(&file, B_WRITE_ONLY);
	return SaveToData(document, dataIO);
}

// Load document from data

status_t DocumentIO::LoadFromData(DocumentModel* document, const uint8* data, size_t size) {
	if (!document || !data || size == 0) {
		return B_BAD_VALUE;
	}
	
	BDataIO dataIO(data, size, B_READ_ONLY);
	return LoadFromData(document, &dataIO);
}

status_t DocumentIO::LoadFromData(DocumentModel* document, BDataIO* data) {
	if (!document || !data) {
		return B_BAD_VALUE;
	}
	
	// Read and verify header
	uint32 magic, version;
	status_t result = ReadHeader(data, &magic, &version);
	
	if (result != B_OK) {
		return result;
	}
	
	// Check magic number
	if (magic != kMagicNumber) {
		return B_BAD_DATA;
	}
	
	// Check version
	if (version > kFormatVersion) {
		return B_UNSUPPORTED;
	}
	
	// Clear existing document
	document->Clear();
	
	// Read document info
	result = ReadDocumentInfo(data, document);
	if (result != B_OK) {
		return result;
	}
	
	// Read page settings
	result = ReadPageSettings(data, document);
	if (result != B_OK) {
		return result;
	}
	
	// Read elements
	result = ReadElements(data, document);
	if (result != B_OK) {
		return result;
	}
	
	// Reset modified flag
	document->SetModified(false);
	
	return B_OK;
}

// Save document to data

status_t DocumentIO::SaveToData(DocumentModel* document, uint8** data, size_t* size) {
	if (!document || !data || !size) {
		return B_BAD_VALUE;
	}
	
	// First, calculate the required size
	// This is a simplified approach - in reality we'd need to calculate the exact size
	// For now, we'll use a buffer that grows as needed
	
	BDataIO* dataIO = new BDataIO();
	
	status_t result = SaveToData(document, dataIO);
	
	if (result == B_OK) {
		// Get the data
		dataIO->Seek(0, SEEK_END);
		*size = dataIO->Position();
		dataIO->Seek(0, SEEK_SET);
		
		*data = new uint8[*size];
		dataIO->Read(*data, *size);
	}
	
	delete dataIO;
	
	return result;
}

status_t DocumentIO::SaveToData(DocumentModel* document, BDataIO* data) {
	if (!document || !data) {
		return B_BAD_VALUE;
	}
	
	// Write header
	status_t result = WriteHeader(data);
	if (result != B_OK) {
		return result;
	}
	
	// Write document info
	result = WriteDocumentInfo(data, document);
	if (result != B_OK) {
		return result;
	}
	
	// Write page settings
	result = WritePageSettings(data, document);
	if (result != B_OK) {
		return result;
	}
	
	// Write elements
	result = WriteElements(data, document);
	if (result != B_OK) {
		return result;
	}
	
	return B_OK;
}

// Check if file is valid YellowDocs Writer document

bool DocumentIO::IsValidDocumentFile(const BString& path) {
	BFile file(path.String(), B_READ_ONLY);
	if (file.InitCheck() != B_OK) {
		return false;
	}
	
	BDataIO dataIO(&file, B_READ_ONLY);
	
	uint32 magic, version;
	return ReadHeader(&dataIO, &magic, &version) == B_OK && magic == kMagicNumber;
}

// Get document format version

uint32 DocumentIO::GetFormatVersion() const {
	return kFormatVersion;
}

// Helper methods

status_t DocumentIO::ReadHeader(BDataIO* data, uint32* magic, uint32* version) {
	if (!data || !magic || !version) {
		return B_BAD_VALUE;
	}
	
	// Read magic number
	if (data->Read(magic, sizeof(uint32)) != sizeof(uint32)) {
		return B_IO_ERROR;
	}
	
	// Read version
	if (data->Read(version, sizeof(uint32)) != sizeof(uint32)) {
		return B_IO_ERROR;
	}
	
	return B_OK;
}

status_t DocumentIO::WriteHeader(BDataIO* data) {
	if (!data) {
		return B_BAD_VALUE;
	}
	
	// Write magic number
	uint32 magic = kMagicNumber;
	if (data->Write(&magic, sizeof(uint32)) != sizeof(uint32)) {
		return B_IO_ERROR;
	}
	
	// Write version
	uint32 version = kFormatVersion;
	if (data->Write(&version, sizeof(uint32)) != sizeof(uint32)) {
		return B_IO_ERROR;
	}
	
	return B_OK;
}

status_t DocumentIO::ReadDocumentInfo(BDataIO* data, DocumentModel* document) {
	if (!data || !document) {
		return B_BAD_VALUE;
	}
	
	// Read title
	BString title;
	status_t result = ReadString(data, &title);
	if (result != B_OK) {
		return result;
	}
	document->SetTitle(title);
	
	// Read author
	BString author;
	result = ReadString(data, &author);
	if (result != B_OK) {
		return result;
	}
	document->SetAuthor(author);
	
	// Read subject
	BString subject;
	result = ReadString(data, &subject);
	if (result != B_OK) {
		return result;
	}
	document->SetSubject(subject);
	
	// Read keywords
	BString keywords;
	result = ReadString(data, &keywords);
	if (result != B_OK) {
		return result;
	}
	document->SetKeywords(keywords);
	
	return B_OK;
}

status_t DocumentIO::WriteDocumentInfo(BDataIO* data, DocumentModel* document) {
	if (!data || !document) {
		return B_BAD_VALUE;
	}
	
	// Write title
	status_t result = WriteString(data, document->Title());
	if (result != B_OK) {
		return result;
	}
	
	// Write author
	result = WriteString(data, document->Author());
	if (result != B_OK) {
		return result;
	}
	
	// Write subject
	result = WriteString(data, document->Subject());
	if (result != B_OK) {
		return result;
	}
	
	// Write keywords
	result = WriteString(data, document->Keywords());
	if (result != B_OK) {
		return result;
	}
	
	return B_OK;
}

status_t DocumentIO::ReadPageSettings(BDataIO* data, DocumentModel* document) {
	if (!data || !document) {
		return B_BAD_VALUE;
	}
	
	// Read page size
	uint32 pageSize;
	if (data->Read(&pageSize, sizeof(uint32)) != sizeof(uint32)) {
		return B_IO_ERROR;
	}
	document->SetPageSize(static_cast<PageSize>(pageSize));
	
	// Read page width
	float pageWidth;
	if (data->Read(&pageWidth, sizeof(float)) != sizeof(float)) {
		return B_IO_ERROR;
	}
	document->SetPageWidth(pageWidth);
	
	// Read page height
	float pageHeight;
	if (data->Read(&pageHeight, sizeof(float)) != sizeof(float)) {
		return B_IO_ERROR;
	}
	document->SetPageHeight(pageHeight);
	
	// Read page orientation
	uint32 orientation;
	if (data->Read(&orientation, sizeof(uint32)) != sizeof(uint32)) {
		return B_IO_ERROR;
	}
	document->SetPageOrientation(static_cast<PageOrientation>(orientation));
	
	// Read margins
	float margin;
	if (data->Read(&margin, sizeof(float)) != sizeof(float)) {
		return B_IO_ERROR;
	}
	document->SetMarginLeft(margin);
	
	if (data->Read(&margin, sizeof(float)) != sizeof(float)) {
		return B_IO_ERROR;
	}
	document->SetMarginRight(margin);
	
	if (data->Read(&margin, sizeof(float)) != sizeof(float)) {
		return B_IO_ERROR;
	}
	document->SetMarginTop(margin);
	
	if (data->Read(&margin, sizeof(float)) != sizeof(float)) {
		return B_IO_ERROR;
	}
	document->SetMarginBottom(margin);
	
	return B_OK;
}

status_t DocumentIO::WritePageSettings(BDataIO* data, DocumentModel* document) {
	if (!data || !document) {
		return B_BAD_VALUE;
	}
	
	// Write page size
	uint32 pageSize = document->PageSize();
	if (data->Write(&pageSize, sizeof(uint32)) != sizeof(uint32)) {
		return B_IO_ERROR;
	}
	
	// Write page width
	float pageWidth = document->PageWidth();
	if (data->Write(&pageWidth, sizeof(float)) != sizeof(float)) {
		return B_IO_ERROR;
	}
	
	// Write page height
	float pageHeight = document->PageHeight();
	if (data->Write(&pageHeight, sizeof(float)) != sizeof(float)) {
		return B_IO_ERROR;
	}
	
	// Write page orientation
	uint32 orientation = document->PageOrientation();
	if (data->Write(&orientation, sizeof(uint32)) != sizeof(uint32)) {
		return B_IO_ERROR;
	}
	
	// Write margins
	float margin = document->MarginLeft();
	if (data->Write(&margin, sizeof(float)) != sizeof(float)) {
		return B_IO_ERROR;
	}
	
	margin = document->MarginRight();
	if (data->Write(&margin, sizeof(float)) != sizeof(float)) {
		return B_IO_ERROR;
	}
	
	margin = document->MarginTop();
	if (data->Write(&margin, sizeof(float)) != sizeof(float)) {
		return B_IO_ERROR;
	}
	
	margin = document->MarginBottom();
	if (data->Write(&margin, sizeof(float)) != sizeof(float)) {
		return B_IO_ERROR;
	}
	
	return B_OK;
}

status_t DocumentIO::ReadElements(BDataIO* data, DocumentModel* document) {
	if (!data || !document) {
		return B_BAD_VALUE;
	}
	
	// Read number of elements
	int32 elementCount;
	if (data->Read(&elementCount, sizeof(int32)) != sizeof(int32)) {
		return B_IO_ERROR;
	}
	
	// Read each element
	for (int32 i = 0; i < elementCount; i++) {
		// Read element type
		uint32 elementType;
		if (data->Read(&elementType, sizeof(uint32)) != sizeof(uint32)) {
			return B_IO_ERROR;
		}
		
		// Read based on element type
		switch (elementType) {
			case DOC_ELEMENT_TEXT:
				ReadTextElement(data, document);
				break;
			case DOC_ELEMENT_PARAGRAPH:
				ReadParagraphElement(data, document);
				break;
			case DOC_ELEMENT_IMAGE:
				ReadImageElement(data, document);
				break;
			case DOC_ELEMENT_TABLE:
				ReadTableElement(data, document);
				break;
			default:
				// Unknown element type, skip
				break;
		}
	}
	
	return B_OK;
}

status_t DocumentIO::WriteElements(BDataIO* data, DocumentModel* document) {
	if (!data || !document) {
		return B_BAD_VALUE;
	}
	
	// Write number of elements
	int32 elementCount = document->CountElements();
	if (data->Write(&elementCount, sizeof(int32)) != sizeof(int32)) {
		return B_IO_ERROR;
	}
	
	// Write each element
	for (int32 i = 0; i < elementCount; i++) {
		DocumentElement* element = document->ElementAt(i);
		if (!element) {
			continue;
		}
		
		// Write element type
		uint32 elementType = element->Type();
		if (data->Write(&elementType, sizeof(uint32)) != sizeof(uint32)) {
			return B_IO_ERROR;
		}
		
		// Write based on element type
		switch (elementType) {
			case DOC_ELEMENT_TEXT:
				WriteTextElement(data, element);
				break;
			case DOC_ELEMENT_PARAGRAPH:
				WriteParagraphElement(data, element);
				break;
			case DOC_ELEMENT_IMAGE:
				WriteImageElement(data, element);
				break;
			case DOC_ELEMENT_TABLE:
				WriteTableElement(data, element);
				break;
			default:
				// Unknown element type, skip
				break;
		}
	}
	
	return B_OK;
}

status_t DocumentIO::ReadTextElement(BDataIO* data, DocumentModel* document) {
	if (!data || !document) {
		return B_BAD_VALUE;
	}
	
	// Read text
	BString text;
	status_t result = ReadString(data, &text);
	if (result != B_OK) {
		return result;
	}
	
	// Read style
	TextStyle style;
	result = ReadTextStyle(data, &style);
	if (result != B_OK) {
		return result;
	}
	
	// Add text element
	document->AddTextElement(text, style);
	
	return B_OK;
}

status_t DocumentIO::WriteTextElement(BDataIO* data, DocumentElement* element) {
	if (!data || !element || element->Type() != DOC_ELEMENT_TEXT) {
		return B_BAD_VALUE;
	}
	
	TextElement* textElement = dynamic_cast<TextElement*>(element);
	if (!textElement) {
		return B_BAD_VALUE;
	}
	
	// Write text
	status_t result = WriteString(data, textElement->Text());
	if (result != B_OK) {
		return result;
	}
	
	// Write style
	result = WriteTextStyle(data, &textElement->Style());
	if (result != B_OK) {
		return result;
	}
	
	return B_OK;
}

status_t DocumentIO::ReadParagraphElement(BDataIO* data, DocumentModel* document) {
	if (!data || !document) {
		return B_BAD_VALUE;
	}
	
	// Read style
	ParagraphStyle style;
	status_t result = ReadParagraphStyle(data, &style);
	if (result != B_OK) {
		return result;
	}
	
	// Add paragraph element
	document->AddParagraphElement(style);
	
	return B_OK;
}

status_t DocumentIO::WriteParagraphElement(BDataIO* data, DocumentElement* element) {
	if (!data || !element || element->Type() != DOC_ELEMENT_PARAGRAPH) {
		return B_BAD_VALUE;
	}
	
	ParagraphElement* paragraphElement = dynamic_cast<ParagraphElement*>(element);
	if (!paragraphElement) {
		return B_BAD_VALUE;
	}
	
	// Write style
	return WriteParagraphStyle(data, &paragraphElement->Style());
}

status_t DocumentIO::ReadImageElement(BDataIO* data, DocumentModel* document) {
	if (!data || !document) {
		return B_BAD_VALUE;
	}
	
	// Read image data
	ImageData image;
	
	// Read file path
	BString filePath;
	status_t result = ReadString(data, &filePath);
	if (result != B_OK) {
		return result;
	}
	image.SetFilePath(filePath);
	
	// Read dimensions
	float width, height;
	if (data->Read(&width, sizeof(float)) != sizeof(float)) {
		return B_IO_ERROR;
	}
	if (data->Read(&height, sizeof(float)) != sizeof(float)) {
		return B_IO_ERROR;
	}
	image.SetWidth(width);
	image.SetHeight(height);
	
	// Read alt text
	BString altText;
	result = ReadString(data, &altText);
	if (result != B_OK) {
		return result;
	}
	
	// Add image element
	document->AddImageElement(image, altText);
	
	return B_OK;
}

status_t DocumentIO::WriteImageElement(BDataIO* data, DocumentElement* element) {
	if (!data || !element || element->Type() != DOC_ELEMENT_IMAGE) {
		return B_BAD_VALUE;
	}
	
	ImageElement* imageElement = dynamic_cast<ImageElement*>(element);
	if (!imageElement) {
		return B_BAD_VALUE;
	}
	
	// Write file path
	status_t result = WriteString(data, imageElement->Image().FilePath());
	if (result != B_OK) {
		return result;
	}
	
	// Write dimensions
	float width = imageElement->Image().Width();
	float height = imageElement->Image().Height();
	
	if (data->Write(&width, sizeof(float)) != sizeof(float)) {
		return B_IO_ERROR;
	}
	if (data->Write(&height, sizeof(float)) != sizeof(float)) {
		return B_IO_ERROR;
	}
	
	// Write alt text
	result = WriteString(data, imageElement->AltText());
	if (result != B_OK) {
		return result;
	}
	
	return B_OK;
}

status_t DocumentIO::ReadTableElement(BDataIO* data, DocumentModel* document) {
	if (!data || !document) {
		return B_BAD_VALUE;
	}
	
	// Read rows and columns
	int32 rows, cols;
	if (data->Read(&rows, sizeof(int32)) != sizeof(int32)) {
		return B_IO_ERROR;
	}
	if (data->Read(&cols, sizeof(int32)) != sizeof(int32)) {
		return B_IO_ERROR;
	}
	
	// Add table element
	document->AddTableElement(rows, cols);
	
	// Read cell data
	for (int32 i = 0; i < rows; i++) {
		for (int32 j = 0; j < cols; j++) {
			BString cellText;
			status_t result = ReadString(data, &cellText);
			if (result != B_OK) {
				return result;
			}
			
			// Get the table element and set cell text
			DocumentElement* element = document->ElementAt(document->CountElements() - 1);
			if (element && element->Type() == DOC_ELEMENT_TABLE) {
				TableElement* table = dynamic_cast<TableElement*>(element);
				if (table) {
					table->SetCellText(i, j, cellText);
				}
			}
		}
	}
	
	return B_OK;
}

status_t DocumentIO::WriteTableElement(BDataIO* data, DocumentElement* element) {
	if (!data || !element || element->Type() != DOC_ELEMENT_TABLE) {
		return B_BAD_VALUE;
	}
	
	TableElement* tableElement = dynamic_cast<TableElement*>(element);
	if (!tableElement) {
		return B_BAD_VALUE;
	}
	
	// Write rows and columns
	int32 rows = tableElement->Rows();
	int32 cols = tableElement->Cols();
	
	if (data->Write(&rows, sizeof(int32)) != sizeof(int32)) {
		return B_IO_ERROR;
	}
	if (data->Write(&cols, sizeof(int32)) != sizeof(int32)) {
		return B_IO_ERROR;
	}
	
	// Write cell data
	for (int32 i = 0; i < rows; i++) {
		for (int32 j = 0; j < cols; j++) {
			status_t result = WriteString(data, tableElement->CellText(i, j));
			if (result != B_OK) {
				return result;
			}
		}
	}
	
	return B_OK;
}

// Read/write style

status_t DocumentIO::ReadTextStyle(BDataIO* data, TextStyle* style) {
	if (!data || !style) {
		return B_BAD_VALUE;
	}
	
	// Read font family
	BString fontFamily;
	status_t result = ReadString(data, &fontFamily);
	if (result != B_OK) {
		return result;
	}
	style->SetFontFamily(fontFamily);
	
	// Read font size
	float fontSize;
	if (data->Read(&fontSize, sizeof(float)) != sizeof(float)) {
		return B_IO_ERROR;
	}
	style->SetFontSize(fontSize);
	
	// Read flags
	uint8 flags;
	if (data->Read(&flags, sizeof(uint8)) != sizeof(uint8)) {
		return B_IO_ERROR;
	}
	
	style->SetBold((flags & 0x01) != 0);
	style->SetItalic((flags & 0x02) != 0);
	style->SetUnderline((flags & 0x04) != 0);
	style->SetStrikeout((flags & 0x08) != 0);
	style->SetSubscript((flags & 0x10) != 0);
	style->SetSuperscript((flags & 0x20) != 0);
	style->SetSmallCaps((flags & 0x40) != 0);
	
	// Read text color
	rgb_color textColor;
	result = ReadColor(data, &textColor);
	if (result != B_OK) {
		return result;
	}
	style->SetTextColor(textColor);
	
	// Read background color
	rgb_color bgColor;
	result = ReadColor(data, &bgColor);
	if (result != B_OK) {
		return result;
	}
	style->SetBackgroundColor(bgColor);
	
	return B_OK;
}

status_t DocumentIO::WriteTextStyle(BDataIO* data, const TextStyle* style) {
	if (!data || !style) {
		return B_BAD_VALUE;
	}
	
	// Write font family
	status_t result = WriteString(data, style->FontFamily());
	if (result != B_OK) {
		return result;
	}
	
	// Write font size
	float fontSize = style->FontSize();
	if (data->Write(&fontSize, sizeof(float)) != sizeof(float)) {
		return B_IO_ERROR;
	}
	
	// Write flags
	uint8 flags = 0;
	if (style->IsBold()) flags |= 0x01;
	if (style->IsItalic()) flags |= 0x02;
	if (style->IsUnderline()) flags |= 0x04;
	if (style->IsStrikeout()) flags |= 0x08;
	if (style->IsSubscript()) flags |= 0x10;
	if (style->IsSuperscript()) flags |= 0x20;
	if (style->IsSmallCaps()) flags |= 0x40;
	
	if (data->Write(&flags, sizeof(uint8)) != sizeof(uint8)) {
		return B_IO_ERROR;
	}
	
	// Write text color
	result = WriteColor(data, style->TextColor());
	if (result != B_OK) {
		return result;
	}
	
	// Write background color
	result = WriteColor(data, style->BackgroundColor());
	if (result != B_OK) {
		return result;
	}
	
	return B_OK;
}

status_t DocumentIO::ReadParagraphStyle(BDataIO* data, ParagraphStyle* style) {
	if (!data || !style) {
		return B_BAD_VALUE;
	}
	
	// Read alignment
	uint8 alignment;
	if (data->Read(&alignment, sizeof(uint8)) != sizeof(uint8)) {
		return B_IO_ERROR;
	}
	style->SetAlignment(static_cast<TextAlignment>(alignment));
	
	// Read indentation
	float indent;
	if (data->Read(&indent, sizeof(float)) != sizeof(float)) {
		return B_IO_ERROR;
	}
	style->SetLeftIndent(indent);
	
	if (data->Read(&indent, sizeof(float)) != sizeof(float)) {
		return B_IO_ERROR;
	}
	style->SetRightIndent(indent);
	
	if (data->Read(&indent, sizeof(float)) != sizeof(float)) {
		return B_IO_ERROR;
	}
	style->SetFirstLineIndent(indent);
	
	// Read spacing
	float spacing;
	if (data->Read(&spacing, sizeof(float)) != sizeof(float)) {
		return B_IO_ERROR;
	}
	style->SetLineSpacing(spacing);
	
	if (data->Read(&spacing, sizeof(float)) != sizeof(float)) {
		return B_IO_ERROR;
	}
	style->SetSpaceBefore(spacing);
	
	if (data->Read(&spacing, sizeof(float)) != sizeof(float)) {
		return B_IO_ERROR;
	}
	style->SetSpaceAfter(spacing);
	
	// Read list style
	uint8 listStyle;
	if (data->Read(&listStyle, sizeof(uint8)) != sizeof(uint8)) {
		return B_IO_ERROR;
	}
	style->SetListStyle(static_cast<ListStyle>(listStyle));
	
	// Read list level
	int8 listLevel;
	if (data->Read(&listLevel, sizeof(int8)) != sizeof(int8)) {
		return B_IO_ERROR;
	}
	style->SetListLevel(listLevel);
	
	// Read border
	uint8 hasBorder;
	if (data->Read(&hasBorder, sizeof(uint8)) != sizeof(uint8)) {
		return B_IO_ERROR;
	}
	style->SetHasBorder(hasBorder != 0);
	
	// Read border color
	rgb_color borderColor;
	status_t result = ReadColor(data, &borderColor);
	if (result != B_OK) {
		return result;
	}
	style->SetBorderColor(borderColor);
	
	// Read border width
	float borderWidth;
	if (data->Read(&borderWidth, sizeof(float)) != sizeof(float)) {
		return B_IO_ERROR;
	}
	style->SetBorderWidth(borderWidth);
	
	// Read background color
	rgb_color bgColor;
	result = ReadColor(data, &bgColor);
	if (result != B_OK) {
		return result;
	}
	style->SetBackgroundColor(bgColor);
	
	return B_OK;
}

status_t DocumentIO::WriteParagraphStyle(BDataIO* data, const ParagraphStyle* style) {
	if (!data || !style) {
		return B_BAD_VALUE;
	}
	
	// Write alignment
	uint8 alignment = style->Alignment();
	if (data->Write(&alignment, sizeof(uint8)) != sizeof(uint8)) {
		return B_IO_ERROR;
	}
	
	// Write indentation
	float indent = style->LeftIndent();
	if (data->Write(&indent, sizeof(float)) != sizeof(float)) {
		return B_IO_ERROR;
	}
	
	indent = style->RightIndent();
	if (data->Write(&indent, sizeof(float)) != sizeof(float)) {
		return B_IO_ERROR;
	}
	
	indent = style->FirstLineIndent();
	if (data->Write(&indent, sizeof(float)) != sizeof(float)) {
		return B_IO_ERROR;
	}
	
	// Write spacing
	float spacing = style->LineSpacing();
	if (data->Write(&spacing, sizeof(float)) != sizeof(float)) {
		return B_IO_ERROR;
	}
	
	spacing = style->SpaceBefore();
	if (data->Write(&spacing, sizeof(float)) != sizeof(float)) {
		return B_IO_ERROR;
	}
	
	spacing = style->SpaceAfter();
	if (data->Write(&spacing, sizeof(float)) != sizeof(float)) {
		return B_IO_ERROR;
	}
	
	// Write list style
	uint8 listStyle = style->ListStyle();
	if (data->Write(&listStyle, sizeof(uint8)) != sizeof(uint8)) {
		return B_IO_ERROR;
	}
	
	// Write list level
	int8 listLevel = style->ListLevel();
	if (data->Write(&listLevel, sizeof(int8)) != sizeof(int8)) {
		return B_IO_ERROR;
	}
	
	// Write border
	uint8 hasBorder = style->HasBorder() ? 1 : 0;
	if (data->Write(&hasBorder, sizeof(uint8)) != sizeof(uint8)) {
		return B_IO_ERROR;
	}
	
	// Write border color
	status_t result = WriteColor(data, style->BorderColor());
	if (result != B_OK) {
		return result;
	}
	
	// Write border width
	float borderWidth = style->BorderWidth();
	if (data->Write(&borderWidth, sizeof(float)) != sizeof(float)) {
		return B_IO_ERROR;
	}
	
	// Write background color
	result = WriteColor(data, style->BackgroundColor());
	if (result != B_OK) {
		return result;
	}
	
	return B_OK;
}

// Read/write color

status_t DocumentIO::ReadColor(BDataIO* data, rgb_color* color) {
	if (!data || !color) {
		return B_BAD_VALUE;
	}
	
	uint8 red, green, blue, alpha;
	
	if (data->Read(&red, sizeof(uint8)) != sizeof(uint8)) {
		return B_IO_ERROR;
	}
	if (data->Read(&green, sizeof(uint8)) != sizeof(uint8)) {
		return B_IO_ERROR;
	}
	if (data->Read(&blue, sizeof(uint8)) != sizeof(uint8)) {
		return B_IO_ERROR;
	}
	if (data->Read(&alpha, sizeof(uint8)) != sizeof(uint8)) {
		return B_IO_ERROR;
	}
	
	color->red = red;
	color->green = green;
	color->blue = blue;
	color->alpha = alpha;
	
	return B_OK;
}

status_t DocumentIO::WriteColor(BDataIO* data, rgb_color color) {
	if (!data) {
		return B_BAD_VALUE;
	}
	
	uint8 red = color.red;
	uint8 green = color.green;
	uint8 blue = color.blue;
	uint8 alpha = color.alpha;
	
	if (data->Write(&red, sizeof(uint8)) != sizeof(uint8)) {
		return B_IO_ERROR;
	}
	if (data->Write(&green, sizeof(uint8)) != sizeof(uint8)) {
		return B_IO_ERROR;
	}
	if (data->Write(&blue, sizeof(uint8)) != sizeof(uint8)) {
		return B_IO_ERROR;
	}
	if (data->Write(&alpha, sizeof(uint8)) != sizeof(uint8)) {
		return B_IO_ERROR;
	}
	
	return B_OK;
}

// Read/write string

status_t DocumentIO::ReadString(BDataIO* data, BString* string) {
	if (!data || !string) {
		return B_BAD_VALUE;
	}
	
	// Read string length
	uint32 length;
	if (data->Read(&length, sizeof(uint32)) != sizeof(uint32)) {
		return B_IO_ERROR;
	}
	
	if (length == 0) {
		*string = "";
		return B_OK;
	}
	
	// Read string data
	char* buffer = new char[length + 1];
	if (data->Read(buffer, length) != length) {
		delete[] buffer;
		return B_IO_ERROR;
	}
	
	buffer[length] = '\0';
	*string = buffer;
	delete[] buffer;
	
	return B_OK;
}

status_t DocumentIO::WriteString(BDataIO* data, const BString& string) {
	if (!data) {
		return B_BAD_VALUE;
	}
	
	// Write string length
	uint32 length = string.Length();
	if (data->Write(&length, sizeof(uint32)) != sizeof(uint32)) {
		return B_IO_ERROR;
	}
	
	if (length > 0) {
		// Write string data
		if (data->Write(string.String(), length) != length) {
			return B_IO_ERROR;
		}
	}
	
	return B_OK;
}

} // namespace Core
} // namespace YellowDocs
