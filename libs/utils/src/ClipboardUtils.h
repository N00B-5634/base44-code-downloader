#ifndef YELLOWDOCS_UTILS_CLIPBOARD_UTILS_H
#define YELLOWDOCS_UTILS_CLIPBOARD_UTILS_H

#include <SupportDefs.h>
#include <String.h>
#include <Message.h>
#include <Clipboard.h>

namespace YellowDocs {
namespace Utils {

// Clipboard utility functions

// Check if clipboard contains text
bool HasClipboardText();

// Get text from clipboard
BString GetClipboardText();

// Set text to clipboard
bool SetClipboardText(const BString& text);

// Check if clipboard contains MIME type
bool HasClipboardMIMEType(const BString& mimeType);

// Get clipboard data for MIME type
BMessage GetClipboardData(const BString& mimeType);

// Set clipboard data with MIME type
bool SetClipboardData(const BMessage& data, const BString& mimeType);

// Clear clipboard
void ClearClipboard();

// Clipboard helper class
class ClipboardHelper {
public:
	ClipboardHelper();
	~ClipboardHelper();

	// Check if clipboard has text
	bool HasText() const;

	// Get text from clipboard
	BString GetText() const;

	// Set text to clipboard
	bool SetText(const BString& text);

	// Check if clipboard has specific MIME type
	bool HasMIMEType(const BString& mimeType) const;

	// Get clipboard data for MIME type
	BMessage GetData(const BString& mimeType) const;

	// Set clipboard data with MIME type
	bool SetData(const BMessage& data, const BString& mimeType);

	// Clear clipboard
	void Clear();

	// Lock clipboard
	bool Lock();

	// Unlock clipboard
	void Unlock();

private:
	BClipboard* fClipboard;
	bool fLocked;
};

} // namespace Utils
} // namespace YellowDocs

#endif // YELLOWDOCS_UTILS_CLIPBOARD_UTILS_H
