#ifndef YELLOWDOCS_UI_TOOLBAR_H
#define YELLOWDOCS_UI_TOOLBAR_H

#include <View.h>
#include <String.h>
#include <Vector.h>
#include <Message.h>

namespace YellowDocs {
namespace UI {

// Forward declaration
class ToolbarButton;

// Toolbar item alignment
enum ToolbarItemAlignment {
	TOOLBAR_ALIGN_LEFT,
	TOOLBAR_ALIGN_CENTER,
	TOOLBAR_ALIGN_RIGHT
};

// Toolbar item type
enum ToolbarItemType {
	TOOLBAR_ITEM_BUTTON,
	TOOLBAR_ITEM_SEPARATOR,
	TOOLBAR_ITEM_SPACER,
	TOOLBAR_ITEM_CUSTOM
};

// Base toolbar item class
class ToolbarItem {
public:
	ToolbarItem(ToolbarItemType type, const BString& id = "");
	virtual ~ToolbarItem();

	ToolbarItemType Type() const;
	BString Id() const;
	
	void SetEnabled(bool enabled);
	bool IsEnabled() const;
	
	void SetVisible(bool visible);
	bool IsVisible() const;
	
	virtual void Draw(BView* view, BRect frame) = 0;
	virtual void MouseDown(BPoint point) = 0;
	virtual void MouseUp(BPoint point) = 0;
	virtual void MouseMoved(BPoint point, uint32 transit, const BMessage* dragMessage) = 0;
	
	virtual BSize MinSize() = 0;
	virtual BSize MaxSize() = 0;
	virtual BSize PreferredSize() = 0;

private:
	ToolbarItemType fType;
	BString fId;
	bool fEnabled;
	bool fVisible;
};

// Toolbar button class
class ToolbarButton : public ToolbarItem {
public:
	ToolbarButton(const BString& id, const BString& label = "", 
		const BString& tooltip = "", uint32 command = 0);
	~ToolbarButton();

	void SetLabel(const BString& label);
	BString Label() const;
	
	void SetTooltip(const BString& tooltip);
	BString Tooltip() const;
	
	void SetCommand(uint32 command);
	uint32 Command() const;
	
	void SetIcon(const BBitmap* icon);
	const BBitmap* Icon() const;
	
	void SetPressed(bool pressed);
	bool IsPressed() const;
	
	void SetToggle(bool toggle);
	bool IsToggle() const;
	
	void SetToggleState(bool state);
	bool ToggleState() const;

	void Draw(BView* view, BRect frame) override;
	void MouseDown(BPoint point) override;
	void MouseUp(BPoint point) override;
	void MouseMoved(BPoint point, uint32 transit, const BMessage* dragMessage) override;

	BSize MinSize() override;
	BSize MaxSize() override;
	BSize PreferredSize() override;

private:
	BString fLabel;
	BString fTooltip;
	uint32 fCommand;
	const BBitmap* fIcon;
	bool fPressed;
	bool fToggle;
	bool fToggleState;
	bool fHover;
};

// Toolbar separator class
class ToolbarSeparator : public ToolbarItem {
public:
	ToolbarSeparator();
	~ToolbarSeparator();

	void Draw(BView* view, BRect frame) override;
	void MouseDown(BPoint point) override;
	void MouseUp(BPoint point) override;
	void MouseMoved(BPoint point, uint32 transit, const BMessage* dragMessage) override;

	BSize MinSize() override;
	BSize MaxSize() override;
	BSize PreferredSize() override;
};

// Toolbar spacer class
class ToolbarSpacer : public ToolbarItem {
public:
	ToolbarSpacer(float width = 10.0f);
	~ToolbarSpacer();

	void SetWidth(float width);
	float Width() const;

	void Draw(BView* view, BRect frame) override;
	void MouseDown(BPoint point) override;
	void MouseUp(BPoint point) override;
	void MouseMoved(BPoint point, uint32 transit, const BMessage* dragMessage) override;

	BSize MinSize() override;
	BSize MaxSize() override;
	BSize PreferredSize() override;

private:
	float fWidth;
};

// Main toolbar class
class Toolbar : public BView {
public:
	Toolbar(const BString& name = "Toolbar");
	~Toolbar();

	// Item management
	ToolbarButton* AddButton(const BString& id, const BString& label = "",
		const BString& tooltip = "", uint32 command = 0);
	
	ToolbarSeparator* AddSeparator();
	
	ToolbarSpacer* AddSpacer(float width = 10.0f);
	
	bool RemoveItem(const BString& id);
	bool RemoveItem(ToolbarItem* item);
	
	ToolbarItem* FindItem(const BString& id) const;
	
	void Clear();

	// Alignment
	void SetItemAlignment(const BString& id, ToolbarItemAlignment alignment);
	ToolbarItemAlignment ItemAlignment(const BString& id) const;

	// Layout
	void LayoutItems();

	// Messages
	void SetMessageTarget(BHandler* target);
	BHandler* MessageTarget() const;

	// Appearance
	void SetOrientation(bool horizontal);
	bool IsHorizontal() const;

	// Drawing
	void Draw(BRect frame) override;
	
	// Mouse handling
	void MouseDown(BPoint point) override;
	void MouseUp(BPoint point) override;
	void MouseMoved(BPoint point, uint32 transit, const BMessage* dragMessage) override;

	// Size management
	BSize MinSize() override;
	BSize MaxSize() override;
	BSize PreferredSize() override;

private:
	BVector<ToolbarItem*> fItems;
	BVector<ToolbarItemAlignment> fAlignments;
	BHandler* fMessageTarget;
	bool fHorizontal;
	
	// Track hovered item
	ToolbarItem* fHoverItem;
	ToolbarItem* fPressedItem;
};

} // namespace UI
} // namespace YellowDocs

#endif // YELLOWDOCS_UI_TOOLBAR_H
