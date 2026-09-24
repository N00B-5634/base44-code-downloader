#ifndef YELLOWDOCS_WRITER_TOOLBAR_VIEW_H
#define YELLOWDOCS_WRITER_TOOLBAR_VIEW_H

#include <View.h>
#include <String.h>
#include <Vector.h>

#include "../../libs/ui/src/Toolbar.h"

namespace YellowDocs {
namespace Writer {

// Toolbar view for YellowDocs Writer
class ToolbarView : public YellowDocs::UI::Toolbar {
public:
	ToolbarView(const BString& name = "ToolbarView");
	~ToolbarView();

	// Initialize toolbar
	void Initialize();

	// Update toolbar state
	void UpdateState();

	// Message handling
	void MessageReceived(BMessage* message) override;

private:
	// Create toolbar items
	void CreateItems();
	
	// Message constants
	enum {
		MSG_TOOLBAR_NEW = 'tbnw',
		MSG_TOOLBAR_OPEN = 'tbop',
		MSG_TOOLBAR_SAVE = 'tbsv',
		MSG_TOOLBAR_BOLD = 'tbbd',
		MSG_TOOLBAR_ITALIC = 'tbit',
		MSG_TOOLBAR_UNDERLINE = 'tbul',
		MSG_TOOLBAR_ALIGN_LEFT = 'tbal',
		MSG_TOOLBAR_ALIGN_CENTER = 'tbac',
		MSG_TOOLBAR_ALIGN_RIGHT = 'tbar',
		MSG_TOOLBAR_ALIGN_JUSTIFY = 'tba j'
	};
};

} // namespace Writer
} // namespace YellowDocs

#endif // YELLOWDOCS_WRITER_TOOLBAR_VIEW_H
