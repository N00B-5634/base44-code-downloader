#include "UndoManager.h"
#include "DocumentModel.h"

namespace YellowDocs {
namespace Core {

// UndoAction implementation

UndoAction::UndoAction(ChangeType type, const BString& description)
	: fType(type),
	  fDescription(description)
{
}

UndoAction::~UndoAction() {
}

ChangeType UndoAction::Type() const {
	return fType;
}

BString UndoAction::Description() const {
	return fDescription;
}

// TextInsertAction implementation

TextInsertAction::TextInsertAction(int32 position, const BString& text)
	: UndoAction(CHANGE_INSERT_TEXT, "Insert text"),
	  fPosition(position),
	  fText(text)
{
}

TextInsertAction::~TextInsertAction() {
}

status_t TextInsertAction::Undo(DocumentModel* document) {
	if (!document) {
		return B_BAD_VALUE;
	}
	
	return document->DeleteText(fPosition, fText.Length());
}

status_t TextInsertAction::Redo(DocumentModel* document) {
	if (!document) {
		return B_BAD_VALUE;
	}
	
	return document->InsertText(fPosition, fText);
}

// TextDeleteAction implementation

TextDeleteAction::TextDeleteAction(int32 position, const BString& deletedText)
	: UndoAction(CHANGE_DELETE_TEXT, "Delete text"),
	  fPosition(position),
	  fDeletedText(deletedText)
{
}

TextDeleteAction::~TextDeleteAction() {
}

status_t TextDeleteAction::Undo(DocumentModel* document) {
	if (!document) {
		return B_BAD_VALUE;
	}
	
	return document->InsertText(fPosition, fDeletedText);
}

status_t TextDeleteAction::Redo(DocumentModel* document) {
	if (!document) {
		return B_BAD_VALUE;
	}
	
	return document->DeleteText(fPosition, fDeletedText.Length());
}

// FormatChangeAction implementation

FormatChangeAction::FormatChangeAction(int32 start, int32 end, const TextStyle& oldStyle, const TextStyle& newStyle)
	: UndoAction(CHANGE_FORMAT_TEXT, "Format change"),
	  fStart(start),
	  fEnd(end),
	  fOldStyle(oldStyle),
	  fNewStyle(newStyle)
{
}

FormatChangeAction::~FormatChangeAction() {
}

status_t FormatChangeAction::Undo(DocumentModel* document) {
	// TODO: Implement format change undo
	// This would require tracking which elements were formatted
	return B_OK;
}

status_t FormatChangeAction::Redo(DocumentModel* document) {
	// TODO: Implement format change redo
	return B_OK;
}

// UndoManager implementation

UndoManager::UndoManager(int32 maxActions)
	: fMaxActions(maxActions),
	  fTrackingEnabled(true)
{
}

UndoManager::~UndoManager() {
	Clear();
}

// Undo/redo

bool UndoManager::CanUndo() const {
	return fUndoStack.CountItems() > 0 && fTrackingEnabled;
}

bool UndoManager::CanRedo() const {
	return fRedoStack.CountItems() > 0 && fTrackingEnabled;
}

status_t UndoManager::Undo(DocumentModel* document) {
	if (!CanUndo() || !document) {
		return B_BAD_VALUE;
	}
	
	UndoAction* action = fUndoStack.LastItem();
	if (!action) {
		return B_ERROR;
	}
	
	status_t result = action->Undo(document);
	
	if (result == B_OK) {
		fUndoStack.RemoveItem(fUndoStack.CountItems() - 1);
		fRedoStack.Add(action);
		
		// Limit redo stack size
		if (fRedoStack.CountItems() > fMaxActions) {
			delete fRedoStack.RemoveItem(0);
		}
	}
	
	return result;
}

status_t UndoManager::Redo(DocumentModel* document) {
	if (!CanRedo() || !document) {
		return B_BAD_VALUE;
	}
	
	UndoAction* action = fRedoStack.LastItem();
	if (!action) {
		return B_ERROR;
	}
	
	status_t result = action->Redo(document);
	
	if (result == B_OK) {
		fRedoStack.RemoveItem(fRedoStack.CountItems() - 1);
		fUndoStack.Add(action);
		
		// Limit undo stack size
		if (fUndoStack.CountItems() > fMaxActions) {
			delete fUndoStack.RemoveItem(0);
		}
	}
	
	return result;
}

// Action management

void UndoManager::AddAction(UndoAction* action) {
	if (!fTrackingEnabled || !action) {
		delete action;
		return;
	}
	
	// Clear redo stack when adding a new action
	for (int32 i = 0; i < fRedoStack.CountItems(); i++) {
		delete fRedoStack.ItemAt(i);
	}
	fRedoStack.MakeEmpty();
	
	// Add to undo stack
	fUndoStack.Add(action);
	
	// Limit undo stack size
	if (fUndoStack.CountItems() > fMaxActions) {
		delete fUndoStack.RemoveItem(0);
	}
}

void UndoManager::Clear() {
	for (int32 i = 0; i < fUndoStack.CountItems(); i++) {
		delete fUndoStack.ItemAt(i);
	}
	fUndoStack.MakeEmpty();
	
	for (int32 i = 0; i < fRedoStack.CountItems(); i++) {
		delete fRedoStack.ItemAt(i);
	}
	fRedoStack.MakeEmpty();
}

// Current action description

BString UndoManager::UndoDescription() const {
	if (fUndoStack.CountItems() > 0) {
		UndoAction* action = fUndoStack.LastItem();
		if (action) {
			return action->Description();
		}
	}
	return "";
}

BString UndoManager::RedoDescription() const {
	if (fRedoStack.CountItems() > 0) {
		UndoAction* action = fRedoStack.LastItem();
		if (action) {
			return action->Description();
		}
	}
	return "";
}

// Limit management

void UndoManager::SetMaxActions(int32 max) {
	fMaxActions = max;
	
	// Trim stacks if necessary
	while (fUndoStack.CountItems() > fMaxActions) {
		delete fUndoStack.RemoveItem(0);
	}
	
	while (fRedoStack.CountItems() > fMaxActions) {
		delete fRedoStack.RemoveItem(0);
	}
}

int32 UndoManager::MaxActions() const {
	return fMaxActions;
}

// Disable/enable undo tracking

void UndoManager::SetTrackingEnabled(bool enabled) {
	fTrackingEnabled = enabled;
}

bool UndoManager::IsTrackingEnabled() const {
	return fTrackingEnabled;
}

} // namespace Core
} // namespace YellowDocs
