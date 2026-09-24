#ifndef YELLOWDOCS_CORE_DOCUMENT_MODEL_H
#define YELLOWDOCS_CORE_DOCUMENT_MODEL_H

#include <SupportDefs.h>
#include <String.h>
#include <Vector.h>
#include <List.h>
#include "DocumentTypes.h"
#include "TextStyle.h"
#include "ParagraphStyle.h"
#include "ImageData.h"
#include "Selection.h"

namespace YellowDocs {
namespace Core {

// Forward declarations
class DocumentElement;
class TextElement;
class ParagraphElement;
class ImageElement;
class TableElement;

class DocumentModel {
public:
	DocumentModel();
	~DocumentModel();

	// Document properties
	void SetTitle(const BString& title);
	BString Title() const;

	void SetAuthor(const BString& author);
	BString Author() const;

	void SetSubject(const BString& subject);
	BString Subject() const;

	void SetKeywords(const BString& keywords);
	BString Keywords() const;

	// Page settings
	void SetPageSize(PageSize size);
	PageSize PageSize() const;

	void SetPageWidth(float width);
	float PageWidth() const;

	void SetPageHeight(float height);
	float PageHeight() const;

	void SetPageOrientation(PageOrientation orientation);
	PageOrientation PageOrientation() const;

	void SetMarginLeft(float margin);
	float MarginLeft() const;

	void SetMarginRight(float margin);
	float MarginRight() const;

	void SetMarginTop(float margin);
	float MarginTop() const;

	void SetMarginBottom(float margin);
	float MarginBottom() const;

	// Document elements
	int32 CountElements() const;
	DocumentElement* ElementAt(int32 index) const;
	DocumentElement* ElementAt(int32 index);

	// Element management
	int32 AddTextElement(const BString& text, const TextStyle& style);
	int32 AddParagraphElement(const ParagraphStyle& style);
	int32 AddImageElement(const ImageData& image, const BString& altText);
	int32 AddTableElement(int rows, int cols);

	bool RemoveElement(int32 index);
	bool RemoveElement(DocumentElement* element);

	void Clear();

	// Text content
	BString GetAllText() const;
	void SetAllText(const BString& text);

	// Selection
	void SetSelection(const Selection& selection);
	Selection Selection() const;

	BString GetSelectedText() const;
	
	// Modify text
	bool InsertText(int32 position, const BString& text);
	bool DeleteText(int32 position, int32 length);
	bool ReplaceText(int32 position, int32 length, const BString& text);

	// Style
	void SetDefaultTextStyle(const TextStyle& style);
	TextStyle DefaultTextStyle() const;

	void SetDefaultParagraphStyle(const ParagraphStyle& style);
	ParagraphStyle DefaultParagraphStyle() const;

	// Document state
	bool IsModified() const;
	void SetModified(bool modified);

	// File format version
	uint32 FormatVersion() const;

private:
	BString fTitle;
	BString fAuthor;
	BString fSubject;
	BString fKeywords;

	PageSize fPageSize;
	float fPageWidth;
	float fPageHeight;
	PageOrientation fPageOrientation;
	float fMarginLeft;
	float fMarginRight;
	float fMarginTop;
	float fMarginBottom;

	BVector<DocumentElement*> fElements;
	Selection fSelection;
	TextStyle fDefaultTextStyle;
	ParagraphStyle fDefaultParagraphStyle;
	bool fModified;
};

// Base element class
class DocumentElement {
public:
	DocumentElement(DocumentElementType type);
	virtual ~DocumentElement();

	DocumentElementType Type() const;
	
	virtual BString ToPlainText() const = 0;
	
private:
	DocumentElementType fType;
};

// Text element
class TextElement : public DocumentElement {
public:
	TextElement(const BString& text = "", const TextStyle& style = TextStyle());
	~TextElement();

	void SetText(const BString& text);
	BString Text() const;

	void SetStyle(const TextStyle& style);
	TextStyle Style() const;

	BString ToPlainText() const override;

private:
	BString fText;
	TextStyle fStyle;
};

// Paragraph element
class ParagraphElement : public DocumentElement {
public:
	ParagraphElement(const ParagraphStyle& style = ParagraphStyle());
	~ParagraphElement();

	void SetStyle(const ParagraphStyle& style);
	ParagraphStyle Style() const;

	BString ToPlainText() const override;

private:
	ParagraphStyle fStyle;
};

// Image element
class ImageElement : public DocumentElement {
public:
	ImageElement(const ImageData& image = ImageData(), const BString& altText = "");
	~ImageElement();

	void SetImage(const ImageData& image);
	ImageData Image() const;

	void SetAltText(const BString& text);
	BString AltText() const;

	BString ToPlainText() const override;

private:
	ImageData fImage;
	BString fAltText;
};

// Table element
class TableElement : public DocumentElement {
public:
	TableElement(int rows = 1, int cols = 1);
	~TableElement();

	int Rows() const;
	int Cols() const;

	void SetCellText(int row, int col, const BString& text);
	BString CellText(int row, int col) const;

	BString ToPlainText() const override;

private:
	int fRows;
	int fCols;
	BVector<BVector<BString>> fCells;
};

} // namespace Core
} // namespace YellowDocs

#endif // YELLOWDOCS_CORE_DOCUMENT_MODEL_H
