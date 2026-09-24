#include "DocumentView.h"

#include <Font.h>
#include <Message.h>
#include <ScrollView.h>
#include <String.h>
#include <Window.h>

#include "Theme.h"
#include "Colors.h"

namespace YellowDocs {
namespace UI {

// DocumentView implementation

DocumentView::DocumentView(const BString& name)
	: BView(name.String(), B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE | B_FRAME_EVENTS),
	  fDocumentModel(nullptr),
	  fDelegate(nullptr),
	  fShowMargin(true),
	  fMarginSize(25.0f),
	  fMarginColor({240, 240, 240, 255}),
	  fLineSpacing(1.5f),
	  fMouseTracking(false)
{
	SetViewColor(GetTheme().DocumentBackgroundColor());
}

DocumentView::~DocumentView() {
	// Don't delete the document model - it's owned by the document
}

// Document model

void DocumentView::SetDocumentModel(Core::DocumentModel* model) {
	fDocumentModel = model;
	Invalidate();
}

Core::DocumentModel* DocumentView::DocumentModel() const {
	return fDocumentModel;
}

// Delegate

void DocumentView::SetDelegate(DocumentViewDelegate* delegate) {
	fDelegate = delegate;
}

DocumentViewDelegate* DocumentView::Delegate() const {
	return fDelegate;
}

// Selection

void DocumentView::SetSelection(const Core::Selection& selection) {
	fSelection = selection;
	
	if (fDelegate) {
		fDelegate->SelectionChanged(fSelection);
	}
	
	Invalidate();
	ScrollToSelection();
}

Core::Selection DocumentView::Selection() const {
	return fSelection;
}

void DocumentView::SelectAll() {
	if (fDocumentModel) {
		BString text = fDocumentModel->GetAllText();
		fSelection.SetRange(0, text.Length() - 1);
		
		if (fDelegate) {
			fDelegate->SelectionChanged(fSelection);
		}
		
		Invalidate();
	}
}

void DocumentView::SelectNone() {
	fSelection.Reset();
	
	if (fDelegate) {
		fDelegate->SelectionChanged(fSelection);
	}
	
	Invalidate();
}

// Text editing

bool DocumentView::InsertText(const BString& text) {
	if (!fDocumentModel) {
		return false;
	}
	
	int32 position = fSelection.Start();
	
	if (fDocumentModel->InsertText(position, text)) {
		// Move selection past inserted text
		fSelection.SetStart(position + text.Length());
		fSelection.SetEnd(position + text.Length());
		
		if (fDelegate) {
			fDelegate->TextChanged();
			fDelegate->SelectionChanged(fSelection);
			fDelegate->DocumentModified();
		}
		
		Invalidate();
		ScrollToSelection();
		return true;
	}
	
	return false;
}

bool DocumentView::DeleteSelection() {
	if (!fDocumentModel || !fSelection.IsValid()) {
		return false;
	}
	
	int32 start = fSelection.Start();
	int32 length = fSelection.Length();
	
	if (fDocumentModel->DeleteText(start, length)) {
		fSelection.SetStart(start);
		fSelection.SetEnd(start);
		
		if (fDelegate) {
			fDelegate->TextChanged();
			fDelegate->SelectionChanged(fSelection);
			fDelegate->DocumentModified();
		}
		
		Invalidate();
		return true;
	}
	
	return false;
}

bool DocumentView::ReplaceSelection(const BString& text) {
	if (!fDocumentModel) {
		return false;
	}
	
	if (fSelection.IsValid() && fSelection.Length() > 0) {
		int32 start = fSelection.Start();
		int32 length = fSelection.Length();
		
		if (fDocumentModel->ReplaceText(start, length, text)) {
			fSelection.SetStart(start + text.Length());
			fSelection.SetEnd(start + text.Length());
			
			if (fDelegate) {
				fDelegate->TextChanged();
				fDelegate->SelectionChanged(fSelection);
				fDelegate->DocumentModified();
			}
			
			Invalidate();
			ScrollToSelection();
			return true;
		}
	} else {
		// No selection, just insert
		return InsertText(text);
	}
	
	return false;
}

// Style

void DocumentView::SetTextStyle(const Core::TextStyle& style) {
	fTextStyle = style;
	
	if (fDelegate) {
		fDelegate->StyleChanged();
	}
	
	Invalidate();
}

Core::TextStyle DocumentView::TextStyle() const {
	return fTextStyle;
}

void DocumentView::SetParagraphStyle(const Core::ParagraphStyle& style) {
	fParagraphStyle = style;
	
	if (fDelegate) {
		fDelegate->StyleChanged();
	}
	
	Invalidate();
}

Core::ParagraphStyle DocumentView::ParagraphStyle() const {
	return fParagraphStyle;
}

// View settings

void DocumentView::SetShowMargin(bool show) {
	fShowMargin = show;
	Invalidate();
}

bool DocumentView::ShowMargin() const {
	return fShowMargin;
}

void DocumentView::SetMarginSize(float size) {
	fMarginSize = size;
	Invalidate();
}

float DocumentView::MarginSize() const {
	return fMarginSize;
}

void DocumentView::SetMarginColor(rgb_color color) {
	fMarginColor = color;
	Invalidate();
}

rgb_color DocumentView::MarginColor() const {
	return fMarginColor;
}

void DocumentView::SetLineSpacing(float spacing) {
	fLineSpacing = spacing;
	Invalidate();
}

float DocumentView::LineSpacing() const {
	return fLineSpacing;
}

// Drawing

void DocumentView::Draw(BRect frame) {
	Theme& theme = GetTheme();
	
	// Draw background
	SetHighColor(theme.DocumentBackgroundColor());
	FillRect(frame);
	
	// Draw margin if enabled
	if (fShowMargin) {
		DrawMargin(frame);
	}
	
	// Draw text content
	DrawText(frame);
	
	// Draw selection
	if (fSelection.IsValid()) {
		DrawSelection(frame);
	}
}

// Mouse handling

void DocumentView::MouseDown(BPoint point) {
	fMouseDownPoint = point;
	fMouseTracking = true;
	
	// Set selection start
	Core::Selection newSelection;
	newSelection.SetStart(TextPointToPosition(point).x);
	newSelection.SetEnd(TextPointToPosition(point).x);
	SetSelection(newSelection);
	
	MakeFocus(true);
}

void DocumentView::MouseUp(BPoint point) {
	fMouseTracking = false;
	
	// Set selection end
	UpdateSelection(point);
}

void DocumentView::MouseMoved(BPoint point, uint32 transit, const BMessage* dragMessage) {
	if (fMouseTracking) {
		UpdateSelection(point);
	}
}

// Keyboard handling

void DocumentView::KeyDown(const char* bytes, int32 numBytes) {
	if (!fDocumentModel) {
		BView::KeyDown(bytes, numBytes);
		return;
	}
	
	// Handle special keys
	if (numBytes == 1) {
		switch (bytes[0]) {
			case B_BACKSPACE: {
				DeleteSelection();
				break;
			}
			case B_DELETE: {
				// Forward delete
				int32 position = fSelection.End();
				if (fDocumentModel->DeleteText(position, 1)) {
					fSelection.SetStart(position);
					fSelection.SetEnd(position);
					if (fDelegate) {
						fDelegate->TextChanged();
						fDelegate->SelectionChanged(fSelection);
						fDelegate->DocumentModified();
					}
					Invalidate();
				}
				break;
			}
			case B_ENTER: {
				// Insert paragraph
				InsertText("\n");
				break;
			}
			case B_TAB: {
				// Insert tab
				InsertText("\t");
				break;
			}
			default: {
				// Regular character
				BString text(bytes, numBytes);
				ReplaceSelection(text);
				break;
			}
		}
	}
}

void DocumentView::KeyUp(const char* bytes, int32 numBytes) {
	BView::KeyUp(bytes, numBytes);
}

// Focus handling

void DocumentView::MakeFocus(bool focus) {
	BView::MakeFocus(focus);
	
	if (focus) {
		// Scroll to make selection visible when focused
		ScrollToSelection();
	}
}

// Size management

BSize DocumentView::MinSize() {
	return BSize(100, 100);
}

BSize DocumentView::MaxSize() {
	return BSize(B_SIZE_UNLIMITED, B_SIZE_UNLIMITED);
}

BSize DocumentView::PreferredSize() {
	return BSize(600, 400);
}

// Scrolling

void DocumentView::ScrollToSelection() {
	if (!fSelection.IsValid() || !fDocumentModel) {
		return;
	}
	
	// Get the scroll view parent
	BScrollView* scrollView = dynamic_cast<BScrollView*>(Parent());
	if (!scrollView) {
		return;
	}
	
	// Calculate the position of the selection
	BPoint textPoint = PositionToTextPoint(fSelection.Start());
	
	// Convert to scroll view coordinates
	BPoint scrollPoint = ConvertToParent(textPoint);
	
	// Scroll to make the selection visible
	scrollView->ScrollTo(scrollPoint);
}

void DocumentView::ScrollBy(float dx, float dy) {
	BScrollView* scrollView = dynamic_cast<BScrollView*>(Parent());
	if (scrollView) {
		BPoint current = scrollView->Target();
		scrollView->ScrollTo(BPoint(current.x + dx, current.y + dy));
	}
}

// Helper methods

BPoint DocumentView::TextPointToPosition(BPoint point) const {
	// Convert screen point to text position
	// This is a simplified implementation
	
	BFont font;
	GetFont(&font);
	
	font_height fontHeight;
	font.GetHeight(&fontHeight);
	
	// Calculate line and column
	int32 line = static_cast<int32>(point.y / (fontHeight.ascent + fontHeight.descent + fontHeight.leading));
	
	// For now, return a simple position
	// TODO: Implement proper text layout and position calculation
	return BPoint(0, line);
}

BPoint DocumentView::PositionToTextPoint(int32 position) const {
	// Convert text position to screen point
	// This is a simplified implementation
	
	BFont font;
	GetFont(&font);
	
	font_height fontHeight;
	font.GetHeight(&fontHeight);
	
	// For now, return a simple point
	// TODO: Implement proper text layout and point calculation
	return BPoint(10 + position * 8, 10);
}

void DocumentView::DrawText(BRect frame) {
	if (!fDocumentModel) {
		return;
	}
	
	Theme& theme = GetTheme();
	BFont font;
	GetFont(&font);
	
	// Draw all text elements
	BString allText = fDocumentModel->GetAllText();
	
	if (!allText.IsEmpty()) {
		SetHighColor(theme.DocumentTextColor());
		SetFont(&font);
		
		// Draw text with margin
		BRect textRect = frame;
		textRect.left += fMarginSize;
		textRect.top += fMarginSize;
		textRect.right -= fMarginSize;
		textRect.bottom -= fMarginSize;
		
		// Draw text at origin
		BPoint textPoint(textRect.left, textRect.top + font.Size() + 2);
		
		// Apply text style
		if (fTextStyle.IsBold()) {
			BFont boldFont;
			font.GetFont(&boldFont);
			boldFont.SetFace(B_BOLD_FACE);
			SetFont(&boldFont);
		}
		
		if (fTextStyle.IsItalic()) {
			BFont italicFont;
			font.GetFont(&italicFont);
			italicFont.SetFace(B_ITALIC_FACE);
			SetFont(&italicFont);
		}
		
		DrawString(allText.String(), textPoint);
	}
}

void DocumentView::DrawSelection(BRect frame) {
	Theme& theme = GetTheme();
	
	// Draw selection rectangle
	if (fSelection.IsValid() && fSelection.Length() > 0) {
		BPoint startPoint = PositionToTextPoint(fSelection.Start());
		BPoint endPoint = PositionToTextPoint(fSelection.End());
		
		BRect selectionRect(startPoint.x, startPoint.y, endPoint.x + 4, endPoint.y + 14);
		
		SetHighColor(theme.SelectionColor());
		FillRect(selectionRect);
		
		SetHighColor(theme.SelectionTextColor());
		// Redraw text in selection area
		// TODO: Implement proper text redrawing
	}
}

void DocumentView::DrawMargin(BRect frame) {
	if (!fShowMargin) {
		return;
	}
	
	// Draw left margin
	BRect marginRect = frame;
	marginRect.right = marginRect.left + fMarginSize;
	
	SetHighColor(fMarginColor);
	FillRect(marginRect);
	
	// Draw margin line
	SetHighColor(GetTheme().BorderColor());
	StrokeLine(BPoint(marginRect.right, marginRect.top), 
		BPoint(marginRect.right, marginRect.bottom));
}

void DocumentView::UpdateSelection(BPoint point) {
	if (!fMouseTracking) {
		return;
	}
	
	BPoint textPoint = TextPointToPosition(point);
	int32 position = textPoint.x;
	
	// Update selection
	if (fSelection.Start() <= position) {
		fSelection.SetEnd(position);
	} else {
		fSelection.SetStart(position);
		fSelection.SetEnd(fSelection.Start());
	}
	
	fSelection.Normalize();
	
	if (fDelegate) {
		fDelegate->SelectionChanged(fSelection);
	}
	
	Invalidate();
}

void DocumentView::CalculateLayout() {
	// Calculate text layout
	// This will be implemented for proper text rendering
}

} // namespace UI
} // namespace YellowDocs
