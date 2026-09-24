#ifndef YELLOWDOCS_CORE_MIME_TYPE_REGISTRATION_H
#define YELLOWDOCS_CORE_MIME_TYPE_REGISTRATION_H

#include <SupportDefs.h>
#include <String.h>

namespace YellowDocs {
namespace Core {

// MIME type registration constants
const char* const kAppMIMEType = "application/x-vnd.yellowdocs-writer-app";
const char* const kDocumentMIMEType = "application/x-vnd.yellowdocs-writer";
const char* const kDocumentExtension = ".ywrite";

// MIME type registration class
class MIMETypeRegistration {
public:
	MIMETypeRegistration();
	~MIMETypeRegistration();

	// Register MIME types
	status_t RegisterAll();
	
	// Register document MIME type
	status_t RegisterDocumentMIMEType();
	
	// Register application MIME type
	status_t RegisterApplicationMIMEType();
	
	// Register file extension
	status_t RegisterFileExtension();
	
	// Register with Tracker
	status_t RegisterWithTracker();
	
	// Check if MIME types are registered
	bool IsDocumentMIMETypeRegistered() const;
	bool IsApplicationMIMETypeRegistered() const;
	
private:
	// Helper methods
	status_t CreateMIMETypeDirectory();
	status_t WriteMIMETypeFile(const BString& mimeType, const BString& content);
};

} // namespace Core
} // namespace YellowDocs

#endif // YELLOWDOCS_CORE_MIME_TYPE_REGISTRATION_H
