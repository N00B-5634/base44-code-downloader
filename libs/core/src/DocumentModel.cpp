#include "DocumentModel.h"

namespace YellowDocs {
namespace Core {

// DocumentModel implementation

DocumentModel::DocumentModel()
	: fPageSize(PAGE_SIZE_A4),
	  fPageWidth(210), // A4 width in mm
	  fPageHeight(297), // A4 height in mm
	  fPageOrientation(PAGE_PORTRAIT),
	  fMarginLeft(25),
	  fMarginRight(25),
	  fMarginTop(25),
	  fMarginBottom(25),
	  fModified(false)
{
}

DocumentModel::~DocumentModel() {
	Clear();
}

// Document properties

void DocumentModel::SetTitle(const BString& title) {
	fTitle = title;
	fModified = true;
}

BString DocumentModel::Title() const {
	return fTitle;
}

void DocumentModel::SetAuthor(const BString& author) {
	fAuthor = author;
	fModified = true;
}

BString DocumentModel::Author() const {
	return fAuthor;
}

void DocumentModel::SetSubject(const BString& subject) {
	fSubject = subject;
	fModified = true;
}

BString DocumentModel::Subject() const {
	return fSubject;
}

void DocumentModel::SetKeywords(const BString& keywords) {
	fKeywords = keywords;
	fModified = true;
}

BString DocumentModel::Keywords() const {
	return fKeywords;
}

// Page settings

void DocumentModel::SetPageSize(PageSize size) {
	fPageSize = size;
	
	// Set default dimensions based on page size
	switch (size) {
		case PAGE_SIZE_A4:
			fPageWidth = 210;
			fPageHeight = 297;
			break;
		case PAGE_SIZE_LETTER:
			fPageWidth = 216;
			fPageHeight = 279;
			break;
		case PAGE_SIZE_A5:
			fPageWidth = 148;
			fPageHeight = 210;
			break;
		case PAGE_SIZE_LEGAL:
			fPageWidth = 216;
			fPageHeight = 356;
			break;
		case PAGE_SIZE_CUSTOM:
			// Keep existing custom dimensions
			break;
	}
	
	fModified = true;
}

PageSize DocumentModel::PageSize() const {
	return fPageSize;
}

void DocumentModel::SetPageWidth(float width) {
	fPageWidth = width;
	fPageSize = PAGE_SIZE_CUSTOM;
	fModified = true;
}

float DocumentModel::PageWidth() const {
	return fPageWidth;
}

void DocumentModel::SetPageHeight(float height) {
	fPageHeight = height;
	fPageSize = PAGE_SIZE_CUSTOM;
	fModified = true;
}

float DocumentModel::PageHeight() const {
	return fPageHeight;
}

void DocumentModel::SetPageOrientation(PageOrientation orientation) {
	fPageOrientation = orientation;
	fModified = true;
}

PageOrientation DocumentModel::PageOrientation() const {
	return fPageOrientation;
}

void DocumentModel::SetMarginLeft(float margin) {
	fMarginLeft = margin;
	fModified = true;
}

float DocumentModel::MarginLeft() const {
	return fMarginLeft;
}

void DocumentModel::SetMarginRight(float margin) {
	fMarginRight = margin;
	fModified = true;
}

float DocumentModel::MarginRight() const {
	return fMarginRight;
}

void DocumentModel::SetMarginTop(float margin) {
	fMarginTop = margin;
	fModified = true;
}

float DocumentModel::MarginTop() const {
	return fMarginTop;
}

void DocumentModel::SetMarginBottom(float margin) {
	fMarginBottom = margin;
	fModified = true;
}

float DocumentModel::MarginBottom() const {
	return fMarginBottom;
}

// Document elements

int32 DocumentModel::CountElements() const {
	return fElements.CountItems();
}

DocumentElement* DocumentModel::ElementAt(int32 index) const {
	if (index >= 0 && index < fElements.CountItems()) {
		return fElements.ItemAt(index);
	}
	return nullptr;
}

DocumentElement* DocumentModel::ElementAt(int32 index) {
	if (index >= 0 && index < fElements.CountItems()) {
		return fElements.ItemAt(index);
	}
	return nullptr;
}

// Element management

int32 DocumentModel::AddTextElement(const BString& text, const TextStyle& style) {
	TextElement* element = new TextElement(text, style);
	fElements.Add(element);
	fModified = true;
	return fElements.CountItems() - 1;
}

int32 DocumentModel::AddParagraphElement(const ParagraphStyle& style) {
	ParagraphElement* element = new ParagraphElement(style);
	fElements.Add(element);
	fModified = true;
	return fElements.CountItems() - 1;
}

int32 DocumentModel::AddImageElement(const ImageData& image, const BString& altText) {
	ImageElement* element = new ImageElement(image, altText);
	fElements.Add(element);
	fModified = true;
	return fElements.CountItems() - 1;
}

int32 DocumentModel::AddTableElement(int rows, int cols) {
	TableElement* element = new TableElement(rows, cols);
	fElements.Add(element);
	fModified = true;
	return fElements.CountItems() - 1;
}

bool DocumentModel::RemoveElement(int32 index) {
	if (index >= 0 && index < fElements.CountItems()) {
		delete fElements.RemoveItemAt(index);
		fModified = true;
		return true;
	}
	return false;
}

bool DocumentModel::RemoveElement(DocumentElement* element) {
	for (int32 i = 0; i < fElements.CountItems(); i++) {
		if (fElements.ItemAt(i) == element) {
			delete fElements.RemoveItemAt(i);
			fModified = true;
			return true;
		}
	}
	return false;
}

void DocumentModel::Clear() {
	for (int32 i = 0; i < fElements.CountItems(); i++) {
		delete fElements.ItemAt(i);
	}
	fElements.MakeEmpty();
	fModified = true;
}

// Text content

BString DocumentModel::GetAllText() const {
	BString result;
	
	for (int32 i = 0; i < fElements.CountItems(); i++) {
		DocumentElement* element = fElements.ItemAt(i);
		if (element) {
			BString text = element->ToPlainText();
			if (!text.IsEmpty()) {
				if (!result.IsEmpty()) {
					result << "\n";
				}
				result << text;
			}
		}
	}
	
	return result;
}

void DocumentModel::SetAllText(const BString& text) {
	Clear();
	AddTextElement(text, fDefaultTextStyle);
	fModified = true;
}

// Selection

void DocumentModel::SetSelection(const Selection& selection) {
	fSelection = selection;
}

Selection DocumentModel::Selection() const {
	return fSelection;
}

BString DocumentModel::GetSelectedText() const {
	if (!fSelection.IsValid()) {
		return "";
	}
	
	// TODO: Implement proper text extraction based on selection
	// For now, return text from all text elements
	BString result;
	
	for (int32 i = 0; i < fElements.CountItems(); i++) {
		DocumentElement* element = fElements.ItemAt(i);
		if (element && element->Type() == DOC_ELEMENT_TEXT) {
			TextElement* textElement = dynamic_cast<TextElement*>(element);
			if (textElement) {
				BString text = textElement->Text();
				if (!text.IsEmpty()) {
					if (!result.IsEmpty()) {
						result << " ";
					}
					result << text;
				}
			}
		}
	}
	
	return result;
}

// Modify text

bool DocumentModel::InsertText(int32 position, const BString& text) {
	// TODO: Implement proper text insertion at position
	// For now, append to the last text element or create a new one
	
	bool foundTextElement = false;
	
	for (int32 i = fElements.CountItems() - 1; i >= 0; i--) {
		DocumentElement* element = fElements.ItemAt(i);
		if (element && element->Type() == DOC_ELEMENT_TEXT) {
			TextElement* textElement = dynamic_cast<TextElement*>(element);
			if (textElement) {
				BString currentText = textElement->Text();
				if (position <= currentText.Length()) {
					BString newText = currentText;
					newText.Insert(text.String(), position);
					textElement->SetText(newText);
					fModified = true;
					return true;
				}
				position -= currentText.Length() + 1; // +1 for newline
				foundTextElement = true;
			}
		}
	}
	
	// If we get here, append to the end
	if (foundTextElement) {
		AddTextElement(text, fDefaultTextStyle);
	} else {
		// No text elements exist, create one
		AddTextElement(text, fDefaultTextStyle);
	}
	
	fModified = true;
	return true;
}

bool DocumentModel::DeleteText(int32 position, int32 length) {
	// TODO: Implement proper text deletion
	// For now, clear all text
	
	for (int32 i = 0; i < fElements.CountItems(); i++) {
		DocumentElement* element = fElements.ItemAt(i);
		if (element && element->Type() == DOC_ELEMENT_TEXT) {
			TextElement* textElement = dynamic_cast<TextElement*>(element);
			if (textElement) {
				BString currentText = textElement->Text();
				if (position < currentText.Length()) {
					int32 deleteLength = std::min(length, currentText.Length() - position);
					BString newText = currentText;
					newText.Remove(position, deleteLength);
					textElement->SetText(newText);
					fModified = true;
					return true;
				}
				position -= currentText.Length() + 1;
			}
		}
	}
	
	return false;
}

bool DocumentModel::ReplaceText(int32 position, int32 length, const BString& text) {
	if (DeleteText(position, length)) {
		InsertText(position, text);
		fModified = true;
		return true;
	}
	return false;
}

// Style

void DocumentModel::SetDefaultTextStyle(const TextStyle& style) {
	fDefaultTextStyle = style;
	fModified = true;
}

TextStyle DocumentModel::DefaultTextStyle() const {
	return fDefaultTextStyle;
}

void DocumentModel::SetDefaultParagraphStyle(const ParagraphStyle& style) {
	fDefaultParagraphStyle = style;
	fModified = true;
}

ParagraphStyle DocumentModel::DefaultParagraphStyle() const {
	return fDefaultParagraphStyle;
}

// Document state

bool DocumentModel::IsModified() const {
	return fModified;
}

void DocumentModel::SetModified(bool modified) {
	fModified = modified;
}

// File format version

uint32 DocumentModel::FormatVersion() const {
	return kDocumentFormatVersion;
}

// DocumentElement implementation

DocumentElement::DocumentElement(DocumentElementType type)
	: fType(type)
{
}

DocumentElement::~DocumentElement() {
}

DocumentElementType DocumentElement::Type() const {
	return fType;
}

// TextElement implementation

TextElement::TextElement(const BString& text, const TextStyle& style)
	: DocumentElement(DOC_ELEMENT_TEXT),
	  fText(text),
	  fStyle(style)
{
}

TextElement::~TextElement() {
}

void TextElement::SetText(const BString& text) {
	fText = text;
}

BString TextElement::Text() const {
	return fText;
}

void TextElement::SetStyle(const TextStyle& style) {
	fStyle = style;
}

TextStyle TextElement::Style() const {
	return fStyle;
}

BString TextElement::ToPlainText() const {
	return fText;
}

// ParagraphElement implementation

ParagraphElement::ParagraphElement(const ParagraphStyle& style)
	: DocumentElement(DOC_ELEMENT_PARAGRAPH),
	  fStyle(style)
{
}

ParagraphElement::~ParagraphElement() {
}

void ParagraphElement::SetStyle(const ParagraphStyle& style) {
	fStyle = style;
}

ParagraphStyle ParagraphElement::Style() const {
	return fStyle;
}

BString ParagraphElement::ToPlainText() const {
	return "\n";
}

// ImageElement implementation

ImageElement::ImageElement(const ImageData& image, const BString& altText)
	: DocumentElement(DOC_ELEMENT_IMAGE),
	  fImage(image),
	  fAltText(altText)
{
}

ImageElement::~ImageElement() {
}

void ImageElement::SetImage(const ImageData& image) {
	fImage = image;
}

ImageData ImageElement::Image() const {
	return fImage;
}

void ImageElement::SetAltText(const BString& text) {
	fAltText = text;
}

BString ImageElement::AltText() const {
	return fAltText;
}

BString ImageElement::ToPlainText() const {
	return fAltText.IsEmpty() ? "[Image]" : fAltText;
}

// TableElement implementation

TableElement::TableElement(int rows, int cols)
	: DocumentElement(DOC_ELEMENT_TABLE),
	  fRows(rows),
	  fCols(cols)
{
	fCells.Resize(rows);
	for (int i = 0; i < rows; i++) {
		fCells[i].Resize(cols);
		for (int j = 0; j < cols; j++) {
			fCells[i][j] = "";
		}
	}
}

TableElement::~TableElement() {
}

int TableElement::Rows() const {
	return fRows;
}

int TableElement::Cols() const {
	return fCols;
}

void TableElement::SetCellText(int row, int col, const BString& text) {
	if (row >= 0 && row < fRows && col >= 0 && col < fCols) {
		fCells[row][col] = text;
	}
}

BString TableElement::CellText(int row, int col) const {
	if (row >= 0 && row < fRows && col >= 0 && col < fCols) {
		return fCells[row][col];
	}
	return "";
}

BString TableElement::ToPlainText() const {
	BString result;
	
	for (int i = 0; i < fRows; i++) {
		for (int j = 0; j < fCols; j++) {
			if (j > 0) {
				result << "\t";
			}
			result << fCells[i][j];
		}
		if (i < fRows - 1) {
			result << "\n";
		}
	}
	
	return result;
}

} // namespace Core
} // namespace YellowDocs
