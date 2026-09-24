#include "MIMETypeRegistration.h"

#include <AppFileInfo.h>
#include <Directory.h>
#include <File.h>
#include <FindDirectory.h>
#include <Mime.h>
#include <Path.h>
#include <String.h>

namespace YellowDocs {
namespace Core {

// MIMETypeRegistration implementation

MIMETypeRegistration::MIMETypeRegistration() {
}

MIMETypeRegistration::~MIMETypeRegistration() {
}

// Register MIME types

status_t MIMETypeRegistration::RegisterAll() {
	status_t result;
	
	// Register document MIME type
	result = RegisterDocumentMIMEType();
	if (result != B_OK) {
		return result;
	}
	
	// Register application MIME type
	result = RegisterApplicationMIMEType();
	if (result != B_OK) {
		return result;
	}
	
	// Register file extension
	result = RegisterFileExtension();
	if (result != B_OK) {
		return result;
	}
	
	// Register with Tracker
	result = RegisterWithTracker();
	if (result != B_OK) {
		return result;
	}
	
	return B_OK;
}

// Register document MIME type

status_t MIMETypeRegistration::RegisterDocumentMIMEType() {
	// Check if already registered
	BMimeType mime(kDocumentMIMEType);
	if (mime.IsValid()) {
		return B_OK;
	}
	
	// Create MIME type
	if (mime.SetTo(kDocumentMIMEType) != B_OK) {
		return B_ERROR;
	}
	
	// Set MIME type properties
	mime.SetShortDescription("YellowDocs Writer Document");
	mime.SetLongDescription("YellowDocs Writer rich text document");
	
	// Add file extension
	mime.AddFileExtension("ywrite");
	
	// Set preferred application
	mime.SetPreferredApp(kAppMIMEType);
	
	// Save to MIME database
	return mime.Save();
}

// Register application MIME type

status_t MIMETypeRegistration::RegisterApplicationMIMEType() {
	// Check if already registered
	BMimeType mime(kAppMIMEType);
	if (mime.IsValid()) {
		return B_OK;
	}
	
	// Create MIME type
	if (mime.SetTo(kAppMIMEType) != B_OK) {
		return B_ERROR;
	}
	
	// Set MIME type properties
	mime.SetShortDescription("YellowDocs Writer");
	mime.SetLongDescription("YellowDocs Writer application");
	
	// Save to MIME database
	return mime.Save();
}

// Register file extension

status_t MIMETypeRegistration::RegisterFileExtension() {
	// Create MIME type directory if it doesn't exist
	status_t result = CreateMIMETypeDirectory();
	if (result != B_OK) {
		return result;
	}
	
	// Write MIME type file
	BString content;
	content << "application/x-vnd.yellowdocs-writer\n";
	content << "  short_description: YellowDocs Writer Document\n";
	content << "  long_description: YellowDocs Writer rich text document\n";
	content << "  file_extensions: ywrite\n";
	content << "  preferred_app: " << kAppMIMEType << "\n";
	content << "  sniffer_rule: 0:4:0x5900445752 / 0x00000000\n";
	content << "  sniffer_priority: 100\n";
	
	return WriteMIMETypeFile(kDocumentMIMEType, content);
}

// Register with Tracker

status_t MIMETypeRegistration::RegisterWithTracker() {
	// Register file type with Tracker
	// This is done by creating the appropriate files in the MIME database
	
	// The RegisterDocumentMIMEType() already sets the preferred app
	// and file extension, which should make it visible in Tracker
	
	return B_OK;
}

// Check if MIME types are registered

bool MIMETypeRegistration::IsDocumentMIMETypeRegistered() const {
	BMimeType mime(kDocumentMIMEType);
	return mime.IsValid();
}

bool MIMETypeRegistration::IsApplicationMIMETypeRegistered() const {
	BMimeType mime(kAppMIMEType);
	return mime.IsValid();
}

// Helper methods

status_t MIMETypeRegistration::CreateMIMETypeDirectory() {
	BPath path;
	
	// Try to find the MIME database directory
	if (find_directory(B_SYSTEM_DATA_DIRECTORY, &path) == B_OK) {
		path.Append("mime_db");
		
		BDirectory dir(path.Path());
		if (dir.InitCheck() == B_OK) {
			return B_OK;
		}
		
		// Try to create it
		if (create_directory(path.Path(), 0755) == B_OK) {
			return B_OK;
		}
	}
	
	// Try user MIME database
	if (find_directory(B_USER_DATA_DIRECTORY, &path) == B_OK) {
		path.Append("mime_db");
		
		BDirectory dir(path.Path());
		if (dir.InitCheck() == B_OK) {
			return B_OK;
		}
		
		// Try to create it
		if (create_directory(path.Path(), 0755) == B_OK) {
			return B_OK;
		}
	}
	
	return B_ERROR;
}

status_t MIMETypeRegistration::WriteMIMETypeFile(const BString& mimeType, const BString& content) {
	BPath path;
	
	// Try system MIME database first
	if (find_directory(B_SYSTEM_DATA_DIRECTORY, &path) == B_OK) {
		path.Append("mime_db");
		path.Append(mimeType.String());
		
		BFile file(path.Path(), B_WRITE_ONLY | B_CREATE_FILE | B_ERASE_FILE);
		if (file.InitCheck() == B_OK) {
			ssize_t bytesWritten = file.Write(content.String(), content.Length());
			if (bytesWritten == content.Length()) {
				return B_OK;
			}
		}
	}
	
	// Try user MIME database
	if (find_directory(B_USER_DATA_DIRECTORY, &path) == B_OK) {
		path.Append("mime_db");
		path.Append(mimeType.String());
		
		BFile file(path.Path(), B_WRITE_ONLY | B_CREATE_FILE | B_ERASE_FILE);
		if (file.InitCheck() == B_OK) {
			ssize_t bytesWritten = file.Write(content.String(), content.Length());
			if (bytesWritten == content.Length()) {
				return B_OK;
			}
		}
	}
	
	return B_ERROR;
}

} // namespace Core
} // namespace YellowDocs
