#include "MIMEUtils.h"

#include <AppFileInfo.h>
#include <File.h>
#include <FindDirectory.h>
#include <Path.h>
#include <MimeDatabase.h>

namespace YellowDocs {
namespace Utils {

// Register YellowDocs Writer MIME type

status_t RegisterYellowDocsWriterMIMEType() {
	// Check if already registered
	BMimeType mime(kYellowDocsWriterMIMEType);
	if (mime.IsValid()) {
		return B_OK;
	}
	
	// Create the MIME type
	if (mime.SetTo(kYellowDocsWriterMIMEType) != B_OK) {
		return B_ERROR;
	}
	
	// Set MIME type properties
	mime.SetShortDescription("YellowDocs Writer Document");
	mime.SetLongDescription("YellowDocs Writer rich text document");
	
	// Add file extension
	mime.AddFileExtension(kYellowDocsWriterExtension + 1); // Skip the dot
	
	// Set preferred application
	mime.SetPreferredApp("application/x-vnd.yellowdocs-writer-app");
	
	// Save to MIME database
	return mime.Save();
}

// Check if MIME type is YellowDocs Writer document

bool IsYellowDocsWriterDocument(const BString& mimeType) {
	return mimeType == kYellowDocsWriterMIMEType;
}

// Check if MIME type is YellowDocs Writer application

bool IsYellowDocsWriterApp(const BString& mimeType) {
	return mimeType == kYellowDocsWriterAppMIMEType;
}

// Get MIME type for YellowDocs Writer file

BString GetYellowDocsWriterMIMEType() {
	return BString(kYellowDocsWriterMIMEType);
}

// Get application MIME type for YellowDocs Writer

BString GetYellowDocsWriterAppMIMEType() {
	return BString(kYellowDocsWriterAppMIMEType);
}

// Get file extension for YellowDocs Writer

BString GetYellowDocsWriterExtension() {
	return BString(kYellowDocsWriterExtension);
}

// Register file type with Tracker

status_t RegisterFileTypeWithTracker(const BString& mimeType, const BString& extension,
	const BString& description, const BString& appSignature) {
	
	BMimeType mime(mimeType.String());
	if (mime.InitCheck() != B_OK) {
		return mime.InitCheck();
	}
	
	// Set descriptions
	mime.SetShortDescription(description.String());
	mime.SetLongDescription(description.String());
	
	// Add file extension
	if (!extension.IsEmpty() && extension.StartsWith(".")) {
		mime.AddFileExtension(extension.String() + 1);
	} else if (!extension.IsEmpty()) {
		mime.AddFileExtension(extension.String());
	}
	
	// Set preferred application
	if (!appSignature.IsEmpty()) {
		mime.SetPreferredApp(appSignature.String());
	}
	
	// Save to MIME database
	return mime.Save();
}

// Get MIME type from file extension

BString GetMIMETypeFromExtension(const BString& extension) {
	if (extension.IsEmpty()) {
		return "";
	}
	
	BString ext = extension;
	if (ext.StartsWith(".")) {
		ext.Truncate(1);
	}
	
	BMimeDatabase mimeDB;
	if (mimeDB.InitCheck() != B_OK) {
		return "";
	}
	
	BString mimeType;
	if (mimeDB.GuessMIMEType(ext.String(), &mimeType) == B_OK) {
		return mimeType;
	}
	
	return "";
}

// Get extension from MIME type

BString GetExtensionFromMIMEType(const BString& mimeType) {
	BMimeType mime(mimeType.String());
	if (mime.InitCheck() != B_OK) {
		return "";
	}
	
	BString extension;
	if (mime.GetFileExtension(&extension) == B_OK) {
		return BString(".") + extension;
	}
	
	return "";
}

// Check if MIME type is text-based

bool IsTextMIMEType(const BString& mimeType) {
	if (mimeType.IsEmpty()) {
		return false;
	}
	
	// Check for text/* MIME types
	if (mimeType.StartsWith("text/")) {
		return true;
	}
	
	// Check for specific text-based types
	if (mimeType == "application/x-vnd.yellowdocs-writer") {
		return true;
	}
	
	return false;
}

// Check if MIME type is image-based

bool IsImageMIMEType(const BString& mimeType) {
	if (mimeType.IsEmpty()) {
		return false;
	}
	
	// Check for image/* MIME types
	if (mimeType.StartsWith("image/")) {
		return true;
	}
	
	return false;
}

// Get MIME type for common file types

BString GetMIMETypeForCommonType(const BString& type) {
	if (type == "txt") return "text/plain";
	if (type == "html" || type == "htm") return "text/html";
	if (type == "pdf") return "application/pdf";
	if (type == "png") return "image/png";
	if (type == "jpg" || type == "jpeg") return "image/jpeg";
	if (type == "gif") return "image/gif";
	if (type == "bmp") return "image/x-bitmap";
	if (type == "ywrite") return kYellowDocsWriterMIMEType;
	
	return "";
}

} // namespace Utils
} // namespace YellowDocs
