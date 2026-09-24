#include "StatusBar.h"

#include <Font.h>
#include <String.h>

#include "Theme.h"

namespace YellowDocs {
namespace UI {

// StatusBarField implementation

StatusBarField::StatusBarField(const BString& id, const BString& text,
		StatusBarFieldAlignment alignment, float width)
	: fId(id),
	  fText(text),
	  fAlignment(alignment),
	  fWidth(width),
	  fEnabled(true)
{
}

StatusBarField::~StatusBarField() {
}

void StatusBarField::SetId(const BString& id) {
	fId = id;
}

BString StatusBarField::Id() const {
	return fId;
}

void StatusBarField::SetText(const BString& text) {
	fText = text;
}

BString StatusBarField::Text() const {
	return fText;
}

void StatusBarField::SetAlignment(StatusBarFieldAlignment alignment) {
	fAlignment = alignment;
}

StatusBarFieldAlignment StatusBarField::Alignment() const {
	return fAlignment;
}

void StatusBarField::SetWidth(float width) {
	fWidth = width;
}

float StatusBarField::Width() const {
	return fWidth;
}

void StatusBarField::SetEnabled(bool enabled) {
	fEnabled = enabled;
}

bool StatusBarField::IsEnabled() const {
	return fEnabled;
}

// StatusBar implementation

StatusBar::StatusBar(const BString& name)
	: BView(name.String(), B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE),
	  fHeight(20)
{
	SetViewColor(GetTheme().StatusBarBackgroundColor());
}

StatusBar::~StatusBar() {
	Clear();
}

// Field management

StatusBarField* StatusBar::AddField(const BString& id, const BString& text,
		StatusBarFieldAlignment alignment, float width) {
	StatusBarField* field = new StatusBarField(id, text, alignment, width);
	fFields.Add(field);
	
	LayoutFields();
	Invalidate();
	
	return field;
}

bool StatusBar::RemoveField(const BString& id) {
	for (int32 i = 0; i < fFields.CountItems(); i++) {
		if (fFields.ItemAt(i)->Id() == id) {
			delete fFields.RemoveItemAt(i);
			LayoutFields();
			Invalidate();
			return true;
		}
	}
	return false;
}

bool StatusBar::RemoveField(StatusBarField* field) {
	for (int32 i = 0; i < fFields.CountItems(); i++) {
		if (fFields.ItemAt(i) == field) {
			delete fFields.RemoveItemAt(i);
			LayoutFields();
			Invalidate();
			return true;
		}
	}
	return false;
}

StatusBarField* StatusBar::FindField(const BString& id) const {
	for (int32 i = 0; i < fFields.CountItems(); i++) {
		if (fFields.ItemAt(i)->Id() == id) {
			return fFields.ItemAt(i);
		}
	}
	return nullptr;
}

void StatusBar::Clear() {
	for (int32 i = 0; i < fFields.CountItems(); i++) {
		delete fFields.ItemAt(i);
	}
	fFields.MakeEmpty();
	Invalidate();
}

// Text management

void StatusBar::SetFieldText(const BString& id, const BString& text) {
	StatusBarField* field = FindField(id);
	if (field) {
		field->SetText(text);
		Invalidate();
	}
}

BString StatusBar::FieldText(const BString& id) const {
	StatusBarField* field = FindField(id);
	if (field) {
		return field->Text();
	}
	return "";
}

// Layout

void StatusBar::LayoutFields() {
	// This is handled in Draw() for simplicity
}

// Drawing

void StatusBar::Draw(BRect frame) {
	Theme& theme = GetTheme();
	
	// Draw background
	SetHighColor(theme.StatusBarBackgroundColor());
	FillRect(frame);
	
	// Draw border at top
	SetHighColor(theme.BorderColor());
	StrokeLine(BPoint(frame.left, frame.top), BPoint(frame.right, frame.top));
	
	// Draw fields
	BFont font;
	GetFont(&font);
	
	font_height fontHeight;
	font.GetHeight(&fontHeight);
	
	float y = frame.top + (frame.Height() - fontHeight.ascent - fontHeight.descent) / 2 + fontHeight.ascent;
	
	// Calculate field positions
	float leftWidth = 0;
	float centerWidth = 0;
	float rightWidth = 0;
	
	for (int32 i = 0; i < fFields.CountItems(); i++) {
		StatusBarField* field = fFields.ItemAt(i);
		if (!field->IsEnabled()) {
			continue;
		}
		
		float textWidth = font.StringWidth(field->Text().String()) + 8;
		
		if (field->Width() > 0) {
			textWidth = field->Width();
		}
		
		if (field->Alignment() == STATUS_ALIGN_LEFT) {
			leftWidth += textWidth;
		} else if (field->Alignment() == STATUS_ALIGN_CENTER) {
			centerWidth += textWidth;
		} else {
			rightWidth += textWidth;
		}
	}
	
	// Draw left-aligned fields
	float x = frame.left + 4;
	for (int32 i = 0; i < fFields.CountItems(); i++) {
		StatusBarField* field = fFields.ItemAt(i);
		if (!field->IsEnabled() || field->Alignment() != STATUS_ALIGN_LEFT) {
			continue;
		}
		
		float textWidth = font.StringWidth(field->Text().String()) + 8;
		if (field->Width() > 0) {
			textWidth = field->Width();
		}
		
		SetHighColor(theme.StatusBarTextColor());
		SetFont(&font);
		
		// Draw separator for non-first fields
		if (x > frame.left + 4) {
			SetHighColor(theme.BorderColor());
			StrokeLine(BPoint(x - 4, frame.top + 4), BPoint(x - 4, frame.bottom - 4));
			SetHighColor(theme.StatusBarTextColor());
		}
		
		DrawString(field->Text().String(), BPoint(x + 4, y));
		x += textWidth;
	}
	
	// Draw center-aligned fields
	x = frame.left + (frame.Width() - centerWidth) / 2;
	for (int32 i = 0; i < fFields.CountItems(); i++) {
		StatusBarField* field = fFields.ItemAt(i);
		if (!field->IsEnabled() || field->Alignment() != STATUS_ALIGN_CENTER) {
			continue;
		}
		
		float textWidth = font.StringWidth(field->Text().String()) + 8;
		if (field->Width() > 0) {
			textWidth = field->Width();
		}
		
		SetHighColor(theme.StatusBarTextColor());
		SetFont(&font);
		
		DrawString(field->Text().String(), BPoint(x + 4, y));
		x += textWidth;
	}
	
	// Draw right-aligned fields
	x = frame.right - rightWidth;
	for (int32 i = 0; i < fFields.CountItems(); i++) {
		StatusBarField* field = fFields.ItemAt(i);
		if (!field->IsEnabled() || field->Alignment() != STATUS_ALIGN_RIGHT) {
			continue;
		}
		
		float textWidth = font.StringWidth(field->Text().String()) + 8;
		if (field->Width() > 0) {
			textWidth = field->Width();
		}
		
		SetHighColor(theme.StatusBarTextColor());
		SetFont(&font);
		
		// Draw separator for non-first fields
		if (x < frame.right - textWidth) {
			SetHighColor(theme.BorderColor());
			StrokeLine(BPoint(x + textWidth + 4, frame.top + 4), 
				BPoint(x + textWidth + 4, frame.bottom - 4));
			SetHighColor(theme.StatusBarTextColor());
		}
		
		DrawString(field->Text().String(), BPoint(x + 4, y));
		x += textWidth;
	}
}

// Size management

BSize StatusBar::MinSize() {
	return BSize(100, fHeight);
}

BSize StatusBar::MaxSize() {
	return BSize(B_SIZE_UNLIMITED, fHeight);
}

BSize StatusBar::PreferredSize() {
	BFont font;
	GetFont(&font);
	
	font_height fontHeight;
	font.GetHeight(&fontHeight);
	
	fHeight = fontHeight.ascent + fontHeight.descent + 6;
	
	return BSize(400, fHeight);
}

} // namespace UI
} // namespace YellowDocs
