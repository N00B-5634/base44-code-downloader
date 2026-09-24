#ifndef YELLOWDOCS_UTILS_MIME_UTILS_H
#define YELLOWDOCS_UTILS_MIME_UTILS_H

#include <SupportDefs.h>
#include <String.h>
#include <Mime.h>

namespace YellowDocs {
namespace Utils {

// YellowDocs MIME types
const char* const kYellowDocsWriterMIMEType = "application/x-vnd.yellowdocs-writer";
const char* const kYellowDocsWriterAppMIMEType = "application/x-vnd.yellowdocs-writer-app";
const char* const kYellowDocsWriterExtension = ".ywrite";

// Register YellowDocs Writer MIME type
status_t RegisterYellowDocsWriterMIMEType();

// Check if MIME type is YellowDocs Writer document
bool IsYellowDocsWriterDocument(const BString& mimeType);

// Check if MIME type is YellowDocs Writer application
bool IsYellowDocsWriterApp(const BString& mimeType);

// Get MIME type for YellowDocs Writer file
BString GetYellowDocsWriterMIMEType();

// Get application MIME type for YellowDocs Writer
BString GetYellowDocsWriterAppMIMEType();

// Get file extension for YellowDocs Writer
BString GetYellowDocsWriterExtension();

// Register file type with Tracker
status_t RegisterFileTypeWithTracker(const BString& mimeType, const BString& extension,
	const BString& description, const BString& appSignature);

// Get MIME type from file extension
BString GetMIMETypeFromExtension(const BString& extension);

// Get extension from MIME type
BString GetExtensionFromMIMEType(const BString& mimeType);

// Check if MIME type is text-based
bool IsTextMIMEType(const BString& mimeType);

// Check if MIME type is image-based
bool IsImageMIMEType(const BString& mimeType);

// Get MIME type for common file types
BString GetMIMETypeForCommonType(const BString& type);

} // namespace Utils
} // namespace YellowDocs

#endif // YELLOWDOCS_UTILS_MIME_UTILS_H
