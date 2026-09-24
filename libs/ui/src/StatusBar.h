#ifndef YELLOWDOCS_UI_STATUS_BAR_H
#define YELLOWDOCS_UI_STATUS_BAR_H

#include <View.h>
#include <String.h>
#include <Vector.h>

namespace YellowDocs {
namespace UI {

// Status bar field alignment
enum StatusBarFieldAlignment {
	STATUS_ALIGN_LEFT,
	STATUS_ALIGN_CENTER,
	STATUS_ALIGN_RIGHT
};

// Status bar field class
class StatusBarField {
public:
	StatusBarField(const BString& id = "", const BString& text = "", 
		StatusBarFieldAlignment alignment = STATUS_ALIGN_LEFT, float width = 0);
	~StatusBarField();

	void SetId(const BString& id);
	BString Id() const;

	void SetText(const BString& text);
	BString Text() const;

	void SetAlignment(StatusBarFieldAlignment alignment);
	StatusBarFieldAlignment Alignment() const;

	void SetWidth(float width);
	float Width() const;

	void SetEnabled(bool enabled);
	bool IsEnabled() const;

private:
	BString fId;
	BString fText;
	StatusBarFieldAlignment fAlignment;
	float fWidth;
	bool fEnabled;
};

// Status bar class
class StatusBar : public BView {
public:
	StatusBar(const BString& name = "StatusBar");
	~StatusBar();

	// Field management
	StatusBarField* AddField(const BString& id, const BString& text = "",
		StatusBarFieldAlignment alignment = STATUS_ALIGN_LEFT, float width = 0);
	
	bool RemoveField(const BString& id);
	bool RemoveField(StatusBarField* field);
	
	StatusBarField* FindField(const BString& id) const;
	
	void Clear();

	// Text management
	void SetFieldText(const BString& id, const BString& text);
	BString FieldText(const BString& id) const;

	// Layout
	void LayoutFields();

	// Drawing
	void Draw(BRect frame) override;

	// Size management
	BSize MinSize() override;
	BSize MaxSize() override;
	BSize PreferredSize() override;

private:
	BVector<StatusBarField*> fFields;
	float fHeight;
};

} // namespace UI
} // namespace YellowDocs

#endif // YELLOWDOCS_UI_STATUS_BAR_H
