#include "ClipboardUtils.h"

#include <Application.h>
#include <Message.h>

namespace YellowDocs {
namespace Utils {

// Global clipboard instance
static BClipboard* sClipboard = nullptr;

// Get clipboard instance

BClipboard* GetClipboard() {
	if (!sClipboard) {
		// Try to get clipboard from application
		if (be_app) {
			sClipboard = be_app->Clipboard();
		}
		
		// If still null, try to create one
		if (!sClipboard) {
			sClipboard = new BClipboard("system");
		}
	}
	
	return sClipboard;
}

// Check if clipboard contains text

bool HasClipboardText() {
	BClipboard* clipboard = GetClipboard();
	if (!clipboard) {
		return false;
	}
	
	BMessage* data = clipboard->Data();
	if (!data) {
		return false;
	}
	
	return data->HasString("text/plain");
}

// Get text from clipboard

BString GetClipboardText() {
	BClipboard* clipboard = GetClipboard();
	if (!clipboard) {
		return "";
	}
	
	BMessage* data = clipboard->Data();
	if (!data) {
		return "";
	}
	
	const char* text;
	if (data->FindString("text/plain", &text) == B_OK) {
		return BString(text);
	}
	
	return "";
}

// Set text to clipboard

bool SetClipboardText(const BString& text) {
	BClipboard* clipboard = GetClipboard();
	if (!clipboard) {
		return false;
	}
	
	// Clear clipboard
	clipboard->Clear();
	
	// Create message with text
	BMessage message(B_MIME_DATA);
	message.AddString("text/plain", text.String());
	
	// Add to clipboard
	clipboard->Commit(&message);
	
	return true;
}

// Check if clipboard contains MIME type

bool HasClipboardMIMEType(const BString& mimeType) {
	BClipboard* clipboard = GetClipboard();
	if (!clipboard) {
		return false;
	}
	
	BMessage* data = clipboard->Data();
	if (!data) {
		return false;
	}
	
	return data->HasString(mimeType.String());
}

// Get clipboard data for MIME type

BMessage GetClipboardData(const BString& mimeType) {
	BClipboard* clipboard = GetClipboard();
	BMessage empty;
	
	if (!clipboard) {
		return empty;
	}
	
	BMessage* data = clipboard->Data();
	if (!data) {
		return empty;
	}
	
	// Find the MIME type data
	const char* mimeString;
	if (data->FindString(mimeType.String(), &mimeString) == B_OK) {
		BMessage result;
		result.AddString(mimeType.String(), mimeString);
		return result;
	}
	
	return empty;
}

// Set clipboard data with MIME type

bool SetClipboardData(const BMessage& data, const BString& mimeType) {
	BClipboard* clipboard = GetClipboard();
	if (!clipboard) {
		return false;
	}
	
	// Clear clipboard
	clipboard->Clear();
	
	// Create message with MIME type
	BMessage message(B_MIME_DATA);
	
	// Copy all fields from data
	const char* name;
	int32 type;
	int32 count = data.CountNames(B_ANY_TYPE);
	
	for (int32 i = 0; i < count; i++) {
		if (data.GetNameInfo(i, &name, &type) == B_OK) {
			message.AddData(name, type, data.FindData(name, type), data.GetDataSize(name, type));
		}
	}
	
	// Add MIME type
	message.AddString("be:mimetype", mimeType.String());
	
	// Commit to clipboard
	clipboard->Commit(&message);
	
	return true;
}

// Clear clipboard

void ClearClipboard() {
	BClipboard* clipboard = GetClipboard();
	if (clipboard) {
		clipboard->Clear();
	}
}

// ClipboardHelper implementation

ClipboardHelper::ClipboardHelper()
	: fClipboard(nullptr),
	  fLocked(false)
{
	fClipboard = GetClipboard();
}

ClipboardHelper::~ClipboardHelper() {
	if (fLocked) {
		Unlock();
	}
}

// Check if clipboard has text

bool ClipboardHelper::HasText() const {
	return HasClipboardText();
}

// Get text from clipboard

BString ClipboardHelper::GetText() const {
	return GetClipboardText();
}

// Set text to clipboard

bool ClipboardHelper::SetText(const BString& text) {
	return SetClipboardText(text);
}

// Check if clipboard has specific MIME type

bool ClipboardHelper::HasMIMEType(const BString& mimeType) const {
	return HasClipboardMIMEType(mimeType);
}

// Get clipboard data for MIME type

BMessage ClipboardHelper::GetData(const BString& mimeType) const {
	return GetClipboardData(mimeType);
}

// Set clipboard data with MIME type

bool ClipboardHelper::SetData(const BMessage& data, const BString& mimeType) {
	return SetClipboardData(data, mimeType);
}

// Clear clipboard

void ClipboardHelper::Clear() {
	ClearClipboard();
}

// Lock clipboard

bool ClipboardHelper::Lock() {
	if (fLocked || !fClipboard) {
		return false;
	}
	
	fLocked = fClipboard->Lock();
	return fLocked;
}

// Unlock clipboard

void ClipboardHelper::Unlock() {
	if (fLocked && fClipboard) {
		fClipboard->Unlock();
		fLocked = false;
	}
}

} // namespace Utils
} // namespace YellowDocs
