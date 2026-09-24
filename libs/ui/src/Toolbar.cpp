#include "Toolbar.h"

#include <Bitmap.h>
#include <Font.h>
#include <Region.h>
#include <String.h>
#include <Window.h>

#include "Theme.h"

namespace YellowDocs {
namespace UI {

// ToolbarItem implementation

ToolbarItem::ToolbarItem(ToolbarItemType type, const BString& id)
	: fType(type),
	  fId(id),
	  fEnabled(true),
	  fVisible(true)
{
}

ToolbarItem::~ToolbarItem() {
}

ToolbarItemType ToolbarItem::Type() const {
	return fType;
}

BString ToolbarItem::Id() const {
	return fId;
}

void ToolbarItem::SetEnabled(bool enabled) {
	fEnabled = enabled;
}

bool ToolbarItem::IsEnabled() const {
	return fEnabled;
}

void ToolbarItem::SetVisible(bool visible) {
	fVisible = visible;
}

bool ToolbarItem::IsVisible() const {
	return fVisible;
}

// ToolbarButton implementation

ToolbarButton::ToolbarButton(const BString& id, const BString& label,
		const BString& tooltip, uint32 command)
	: ToolbarItem(TOOLBAR_ITEM_BUTTON, id),
	  fLabel(label),
	  fTooltip(tooltip),
	  fCommand(command),
	  fIcon(nullptr),
	  fPressed(false),
	  fToggle(false),
	  fToggleState(false),
	  fHover(false)
{
}

ToolbarButton::~ToolbarButton() {
}

void ToolbarButton::SetLabel(const BString& label) {
	fLabel = label;
}

BString ToolbarButton::Label() const {
	return fLabel;
}

void ToolbarButton::SetTooltip(const BString& tooltip) {
	fTooltip = tooltip;
}

BString ToolbarButton::Tooltip() const {
	return fTooltip;
}

void ToolbarButton::SetCommand(uint32 command) {
	fCommand = command;
}

uint32 ToolbarButton::Command() const {
	return fCommand;
}

void ToolbarButton::SetIcon(const BBitmap* icon) {
	fIcon = icon;
}

const BBitmap* ToolbarButton::Icon() const {
	return fIcon;
}

void ToolbarButton::SetPressed(bool pressed) {
	fPressed = pressed;
}

bool ToolbarButton::IsPressed() const {
	return fPressed;
}

void ToolbarButton::SetToggle(bool toggle) {
	fToggle = toggle;
}

bool ToolbarButton::IsToggle() const {
	return fToggle;
}

void ToolbarButton::SetToggleState(bool state) {
	fToggleState = state;
}

bool ToolbarButton::ToggleState() const {
	return fToggleState;
}

void ToolbarButton::Draw(BView* view, BRect frame) {
	Theme& theme = GetTheme();
	
	// Draw background
	rgb_color backgroundColor;
	
	if (fPressed) {
		backgroundColor = theme.ToolbarButtonActiveColor();
	} else if (fHover && IsEnabled()) {
		backgroundColor = theme.ToolbarButtonHoverColor();
	} else {
		backgroundColor = theme.ToolbarButtonColor();
	}
	
	view->SetHighColor(backgroundColor);
	view->FillRect(frame);
	
	// Draw border
	view->SetHighColor(theme.ToolbarBackgroundColor());
	view->StrokeRect(frame);
	
	// Draw icon or label
	BRect contentRect = frame;
	contentRect.InsetBy(2, 2);
	
	if (fIcon) {
		float iconX = contentRect.left + (contentRect.Width() - fIcon->Bounds().Width()) / 2;
		float iconY = contentRect.top + (contentRect.Height() - fIcon->Bounds().Height()) / 2;
		
		view->SetDrawingMode(B_OP_ALPHA);
		view->DrawBitmap(fIcon, BPoint(iconX, iconY));
		view->SetDrawingMode(B_OP_COPY);
	} else if (!fLabel.IsEmpty()) {
		view->SetHighColor(IsEnabled() ? theme.ToolbarTextColor() : theme.DisabledColor());
		
		BFont font;
		view->GetFont(&font);
		
		font_height fontHeight;
		font.GetHeight(&fontHeight);
		
		float textX = contentRect.left + 4;
		float textY = contentRect.top + (contentRect.Height() - fontHeight.ascent - fontHeight.descent) / 2 + fontHeight.ascent;
		
		view->SetFont(&font);
		view->DrawString(fLabel.String(), BPoint(textX, textY));
	}
	
	// Draw focus indicator if pressed
	if (fPressed) {
		view->SetHighColor(theme.AccentColor());
		view->StrokeRect(frame, B_SOLID_HIGH);
	}
}

void ToolbarButton::MouseDown(BPoint point) {
	if (!IsEnabled()) {
		return;
	}
	
	fPressed = true;
	
	if (fToggle) {
		fToggleState = !fToggleState;
	}
}

void ToolbarButton::MouseUp(BPoint point) {
	fPressed = false;
}

void ToolbarButton::MouseMoved(BPoint point, uint32 transit, const BMessage* dragMessage) {
	fHover = transit == B_ENTERED_VIEW || transit == B_INSIDE_VIEW;
}

BSize ToolbarButton::MinSize() {
	if (fIcon) {
		return BSize(fIcon->Bounds().Width() + 8, fIcon->Bounds().Height() + 8);
	}
	
	BFont font;
	font.SetSize(12);
	
	font_height fontHeight;
	font.GetHeight(&fontHeight);
	
	float width = font.StringWidth(fLabel.String()) + 12;
	return BSize(width, fontHeight.ascent + fontHeight.descent + 8);
}

BSize ToolbarButton::MaxSize() {
	return BSize(B_SIZE_UNLIMITED, B_SIZE_UNLIMITED);
}

BSize ToolbarButton::PreferredSize() {
	return MinSize();
}

// ToolbarSeparator implementation

ToolbarSeparator::ToolbarSeparator()
	: ToolbarItem(TOOLBAR_ITEM_SEPARATOR, "separator")
{
}

ToolbarSeparator::~ToolbarSeparator() {
}

void ToolbarSeparator::Draw(BView* view, BRect frame) {
	Theme& theme = GetTheme();
	
	// Draw vertical line
	view->SetHighColor(theme.BorderColor());
	
	BPoint start = BPoint(frame.left + frame.Width() / 2, frame.top + 4);
	BPoint end = BPoint(frame.left + frame.Width() / 2, frame.bottom - 4);
	
	view->StrokeLine(start, end);
}

void ToolbarSeparator::MouseDown(BPoint point) {
}

void ToolbarSeparator::MouseUp(BPoint point) {
}

void ToolbarSeparator::MouseMoved(BPoint point, uint32 transit, const BMessage* dragMessage) {
}

BSize ToolbarSeparator::MinSize() {
	return BSize(8, 20);
}

BSize ToolbarSeparator::MaxSize() {
	return BSize(8, B_SIZE_UNLIMITED);
}

BSize ToolbarSeparator::PreferredSize() {
	return BSize(8, 20);
}

// ToolbarSpacer implementation

ToolbarSpacer::ToolbarSpacer(float width)
	: ToolbarItem(TOOLBAR_ITEM_SPACER, "spacer"),
	  fWidth(width)
{
}

ToolbarSpacer::~ToolbarSpacer() {
}

void ToolbarSpacer::SetWidth(float width) {
	fWidth = width;
}

float ToolbarSpacer::Width() const {
	return fWidth;
}

void ToolbarSpacer::Draw(BView* view, BRect frame) {
	// Spacer is invisible
}

void ToolbarSpacer::MouseDown(BPoint point) {
}

void ToolbarSpacer::MouseUp(BPoint point) {
}

void ToolbarSpacer::MouseMoved(BPoint point, uint32 transit, const BMessage* dragMessage) {
}

BSize ToolbarSpacer::MinSize() {
	return BSize(fWidth, 1);
}

BSize ToolbarSpacer::MaxSize() {
	return BSize(fWidth, B_SIZE_UNLIMITED);
}

BSize ToolbarSpacer::PreferredSize() {
	return BSize(fWidth, 1);
}

// Toolbar implementation

Toolbar::Toolbar(const BString& name)
	: BView(name.String(), B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE),
	  fMessageTarget(nullptr),
	  fHorizontal(true),
	  fHoverItem(nullptr),
	  fPressedItem(nullptr)
{
	SetViewColor(GetTheme().ToolbarBackgroundColor());
}

Toolbar::~Toolbar() {
	Clear();
}

// Item management

ToolbarButton* Toolbar::AddButton(const BString& id, const BString& label,
		const BString& tooltip, uint32 command) {
	ToolbarButton* button = new ToolbarButton(id, label, tooltip, command);
	fItems.Add(button);
	fAlignments.Add(TOOLBAR_ALIGN_LEFT);
	
	LayoutItems();
	Invalidate();
	
	return button;
}

ToolbarSeparator* Toolbar::AddSeparator() {
	ToolbarSeparator* separator = new ToolbarSeparator();
	fItems.Add(separator);
	fAlignments.Add(TOOLBAR_ALIGN_LEFT);
	
	LayoutItems();
	Invalidate();
	
	return separator;
}

ToolbarSpacer* Toolbar::AddSpacer(float width) {
	ToolbarSpacer* spacer = new ToolbarSpacer(width);
	fItems.Add(spacer);
	fAlignments.Add(TOOLBAR_ALIGN_LEFT);
	
	LayoutItems();
	Invalidate();
	
	return spacer;
}

bool Toolbar::RemoveItem(const BString& id) {
	for (int32 i = 0; i < fItems.CountItems(); i++) {
		if (fItems.ItemAt(i)->Id() == id) {
			delete fItems.RemoveItemAt(i);
			fAlignments.RemoveItemAt(i);
			
			if (fHoverItem == fItems.ItemAt(i)) {
				fHoverItem = nullptr;
			}
			if (fPressedItem == fItems.ItemAt(i)) {
				fPressedItem = nullptr;
			}
			
			LayoutItems();
			Invalidate();
			return true;
		}
	}
	return false;
}

bool Toolbar::RemoveItem(ToolbarItem* item) {
	for (int32 i = 0; i < fItems.CountItems(); i++) {
		if (fItems.ItemAt(i) == item) {
			delete fItems.RemoveItemAt(i);
			fAlignments.RemoveItemAt(i);
			
			if (fHoverItem == item) {
				fHoverItem = nullptr;
			}
			if (fPressedItem == item) {
				fPressedItem = nullptr;
			}
			
			LayoutItems();
			Invalidate();
			return true;
		}
	}
	return false;
}

ToolbarItem* Toolbar::FindItem(const BString& id) const {
	for (int32 i = 0; i < fItems.CountItems(); i++) {
		if (fItems.ItemAt(i)->Id() == id) {
			return fItems.ItemAt(i);
		}
	}
	return nullptr;
}

void Toolbar::Clear() {
	for (int32 i = 0; i < fItems.CountItems(); i++) {
		delete fItems.ItemAt(i);
	}
	fItems.MakeEmpty();
	fAlignments.MakeEmpty();
	
	fHoverItem = nullptr;
	fPressedItem = nullptr;
	
	Invalidate();
}

// Alignment

void Toolbar::SetItemAlignment(const BString& id, ToolbarItemAlignment alignment) {
	for (int32 i = 0; i < fItems.CountItems(); i++) {
		if (fItems.ItemAt(i)->Id() == id) {
			fAlignments.ReplaceItemAt(i, alignment);
			LayoutItems();
			Invalidate();
			return;
		}
	}
}

ToolbarItemAlignment Toolbar::ItemAlignment(const BString& id) const {
	for (int32 i = 0; i < fItems.CountItems(); i++) {
		if (fItems.ItemAt(i)->Id() == id) {
			return fAlignments.ItemAt(i);
		}
	}
	return TOOLBAR_ALIGN_LEFT;
}

// Layout

void Toolbar::LayoutItems() {
	// Layout items based on their alignment
	// This is a simplified implementation
	
	BRect bounds = Bounds();
	float x = bounds.left;
	float y = bounds.top;
	
	// Calculate total width of left-aligned items
	float leftWidth = 0;
	float centerWidth = 0;
	float rightWidth = 0;
	
	for (int32 i = 0; i < fItems.CountItems(); i++) {
		ToolbarItem* item = fItems.ItemAt(i);
		ToolbarItemAlignment alignment = fAlignments.ItemAt(i);
		
		if (!item->IsVisible()) {
			continue;
		}
		
		BSize size = item->PreferredSize();
		
		if (alignment == TOOLBAR_ALIGN_LEFT) {
			leftWidth += size.width;
		} else if (alignment == TOOLBAR_ALIGN_CENTER) {
			centerWidth += size.width;
		} else {
			rightWidth += size.width;
		}
	}
	
	// Position left-aligned items
	float currentX = bounds.left;
	for (int32 i = 0; i < fItems.CountItems(); i++) {
		ToolbarItem* item = fItems.ItemAt(i);
		ToolbarItemAlignment alignment = fAlignments.ItemAt(i);
		
		if (!item->IsVisible() || alignment != TOOLBAR_ALIGN_LEFT) {
			continue;
		}
		
		BSize size = item->PreferredSize();
		BRect frame(currentX, bounds.top, currentX + size.width, bounds.bottom);
		
		// Adjust for vertical toolbar
		if (!fHorizontal) {
			frame = BRect(bounds.left, currentX, bounds.right, currentX + size.height);
		}
		
		item->Draw(this, frame);
		currentX += size.width;
	}
	
	// Position center-aligned items
	currentX = bounds.left + (bounds.Width() - centerWidth) / 2;
	for (int32 i = 0; i < fItems.CountItems(); i++) {
		ToolbarItem* item = fItems.ItemAt(i);
		ToolbarItemAlignment alignment = fAlignments.ItemAt(i);
		
		if (!item->IsVisible() || alignment != TOOLBAR_ALIGN_CENTER) {
			continue;
		}
		
		BSize size = item->PreferredSize();
		BRect frame(currentX, bounds.top, currentX + size.width, bounds.bottom);
		
		// Adjust for vertical toolbar
		if (!fHorizontal) {
			frame = BRect(bounds.left, currentX, bounds.right, currentX + size.height);
		}
		
		item->Draw(this, frame);
		currentX += size.width;
	}
	
	// Position right-aligned items
	currentX = bounds.right - rightWidth;
	for (int32 i = 0; i < fItems.CountItems(); i++) {
		ToolbarItem* item = fItems.ItemAt(i);
		ToolbarItemAlignment alignment = fAlignments.ItemAt(i);
		
		if (!item->IsVisible() || alignment != TOOLBAR_ALIGN_RIGHT) {
			continue;
		}
		
		BSize size = item->PreferredSize();
		BRect frame(currentX, bounds.top, currentX + size.width, bounds.bottom);
		
		// Adjust for vertical toolbar
		if (!fHorizontal) {
			frame = BRect(bounds.left, currentX, bounds.right, currentX + size.height);
		}
		
		item->Draw(this, frame);
		currentX += size.width;
	}
}

// Messages

void Toolbar::SetMessageTarget(BHandler* target) {
	fMessageTarget = target;
}

BHandler* Toolbar::MessageTarget() const {
	return fMessageTarget;
}

// Appearance

void Toolbar::SetOrientation(bool horizontal) {
	fHorizontal = horizontal;
	LayoutItems();
	Invalidate();
}

bool Toolbar::IsHorizontal() const {
	return fHorizontal;
}

// Drawing

void Toolbar::Draw(BRect frame) {
	Theme& theme = GetTheme();
	
	// Draw background
	SetHighColor(theme.ToolbarBackgroundColor());
	FillRect(frame);
	
	// Draw border at bottom
	SetHighColor(theme.BorderColor());
	StrokeLine(BPoint(frame.left, frame.bottom - 1), BPoint(frame.right, frame.bottom - 1));
	
	// Draw items
	LayoutItems();
}

// Mouse handling

void Toolbar::MouseDown(BPoint point) {
	for (int32 i = 0; i < fItems.CountItems(); i++) {
		ToolbarItem* item = fItems.ItemAt(i);
		if (!item->IsVisible() || !item->IsEnabled()) {
			continue;
		}
		
		BSize size = item->PreferredSize();
		BRect frame = BRect(point.x, point.y, point.x + size.width, point.y + size.height);
		
		if (frame.Contains(point)) {
			fPressedItem = item;
			item->MouseDown(point);
			
			if (item->Type() == TOOLBAR_ITEM_BUTTON) {
				ToolbarButton* button = dynamic_cast<ToolbarButton*>(item);
				if (button && fMessageTarget) {
					BMessage message(button->Command());
					fMessageTarget->MessageReceived(&message);
				}
			}
			
			Invalidate();
			return;
		}
	}
	
	fPressedItem = nullptr;
}

void Toolbar::MouseUp(BPoint point) {
	if (fPressedItem) {
		fPressedItem->MouseUp(point);
		fPressedItem = nullptr;
		Invalidate();
	}
}

void Toolbar::MouseMoved(BPoint point, uint32 transit, const BMessage* dragMessage) {
	ToolbarItem* newHoverItem = nullptr;
	
	for (int32 i = 0; i < fItems.CountItems(); i++) {
		ToolbarItem* item = fItems.ItemAt(i);
		if (!item->IsVisible() || !item->IsEnabled()) {
			continue;
		}
		
		BSize size = item->PreferredSize();
		BRect frame = BRect(point.x, point.y, point.x + size.width, point.y + size.height);
		
		if (frame.Contains(point)) {
			newHoverItem = item;
			break;
		}
	}
	
	if (newHoverItem != fHoverItem) {
		if (fHoverItem) {
			fHoverItem->MouseMoved(point, B_EXITED_VIEW, dragMessage);
		}
		fHoverItem = newHoverItem;
		if (fHoverItem) {
			fHoverItem->MouseMoved(point, B_ENTERED_VIEW, dragMessage);
		}
		Invalidate();
	}
	
	if (fHoverItem) {
		fHoverItem->MouseMoved(point, transit, dragMessage);
	}
}

// Size management

BSize Toolbar::MinSize() {
	float width = 0;
	float height = 0;
	
	for (int32 i = 0; i < fItems.CountItems(); i++) {
		ToolbarItem* item = fItems.ItemAt(i);
		if (item->IsVisible()) {
			BSize size = item->MinSize();
			width += size.width;
			height = std::max(height, size.height);
		}
	}
	
	return BSize(width, height);
}

BSize Toolbar::MaxSize() {
	return BSize(B_SIZE_UNLIMITED, B_SIZE_UNLIMITED);
}

BSize Toolbar::PreferredSize() {
	float width = 0;
	float height = 0;
	
	for (int32 i = 0; i < fItems.CountItems(); i++) {
		ToolbarItem* item = fItems.ItemAt(i);
		if (item->IsVisible()) {
			BSize size = item->PreferredSize();
			width += size.width;
			height = std::max(height, size.height);
		}
	}
	
	return BSize(width, height);
}

} // namespace UI
} // namespace YellowDocs
