#ifndef YELLOWDOCS_CORE_UNDO_MANAGER_H
#define YELLOWDOCS_CORE_UNDO_MANAGER_H

#include <SupportDefs.h>
#include <String.h>
#include <Vector.h>
#include "DocumentTypes.h"

namespace YellowDocs {
namespace Core {

// Forward declaration
class DocumentModel;

class UndoAction {
public:
	UndoAction(ChangeType type, const BString& description = "");
	virtual ~UndoAction();

	ChangeType Type() const;
	BString Description() const;

	virtual status_t Undo(DocumentModel* document) = 0;
	virtual status_t Redo(DocumentModel* document) = 0;

private:
	ChangeType fType;
	BString fDescription;
};

class TextInsertAction : public UndoAction {
public:
	TextInsertAction(int32 position, const BString& text);
	~TextInsertAction();

	status_t Undo(DocumentModel* document) override;
	status_t Redo(DocumentModel* document) override;

private:
	int32 fPosition;
	BString fText;
};

class TextDeleteAction : public UndoAction {
public:
	TextDeleteAction(int32 position, const BString& deletedText);
	~TextDeleteAction();

	status_t Undo(DocumentModel* document) override;
	status_t Redo(DocumentModel* document) override;

private:
	int32 fPosition;
	BString fDeletedText;
};

class FormatChangeAction : public UndoAction {
public:
	FormatChangeAction(int32 start, int32 end, const TextStyle& oldStyle, const TextStyle& newStyle);
	~FormatChangeAction();

	status_t Undo(DocumentModel* document) override;
	status_t Redo(DocumentModel* document) override;

private:
	int32 fStart;
	int32 fEnd;
	TextStyle fOldStyle;
	TextStyle fNewStyle;
};

class UndoManager {
public:
	UndoManager(int32 maxActions = 100);
	~UndoManager();

	// Undo/redo
	bool CanUndo() const;
	bool CanRedo() const;

	status_t Undo(DocumentModel* document);
	status_t Redo(DocumentModel* document);

	// Action management
	void AddAction(UndoAction* action);
	void Clear();

	// Current action description
	BString UndoDescription() const;
	BString RedoDescription() const;

	// Limit management
	void SetMaxActions(int32 max);
	int32 MaxActions() const;

	// Disable/enable undo tracking
	void SetTrackingEnabled(bool enabled);
	bool IsTrackingEnabled() const;

private:
	BVector<UndoAction*> fUndoStack;
	BVector<UndoAction*> fRedoStack;
	int32 fMaxActions;
	bool fTrackingEnabled;
};

} // namespace Core
} // namespace YellowDocs

#endif // YELLOWDOCS_CORE_UNDO_MANAGER_H
