#include "TrackerIntegration.h"

#include <AppFileInfo.h>
#include <Directory.h>
#include <Entry.h>
#include <File.h>
#include <FindDirectory.h>
#include <Mime.h>
#include <Path.h>
#include <Roster.h>
#include <String.h>

namespace YellowDocs {
namespace Core {

// TrackerIntegration implementation

TrackerIntegration::TrackerIntegration() {
}

TrackerIntegration::~TrackerIntegration() {
}

// Register file type with Tracker

status_t TrackerIntegration::RegisterFileType(const BString& mimeType, const BString& extension,
		const BString& description, const BString& appSignature) {
	
	// Register MIME type
	BMimeType mime(mimeType.String());
	if (mime.InitCheck() != B_OK) {
		if (mime.SetTo(mimeType.String()) != B_OK) {
			return B_ERROR;
		}
	}
	
	// Set MIME type properties
	mime.SetShortDescription(description.String());
	mime.SetLongDescription(description.String());
	
	// Add file extension
	if (!extension.IsEmpty()) {
		BString ext = extension;
		if (ext.StartsWith(".")) {
			ext.Truncate(1);
		}
		mime.AddFileExtension(ext.String());
	}
	
	// Set preferred application
	if (!appSignature.IsEmpty()) {
		mime.SetPreferredApp(appSignature.String());
	}
	
	// Save to MIME database
	status_t result = mime.Save();
	if (result != B_OK) {
		return result;
	}
	
	// Create desktop file
	result = CreateDesktopFile(appSignature, appSignature, mimeType);
	
	return result;
}

// Register application with Tracker

status_t TrackerIntegration::RegisterApplication(const BString& appName, const BString& appSignature,
		const BString& mimeType) {
	
	// Register MIME type for application
	BMimeType appMime(appSignature.String());
	if (appMime.InitCheck() != B_OK) {
		if (appMime.SetTo(appSignature.String()) != B_OK) {
			return B_ERROR;
		}
	}
	
	appMime.SetShortDescription(appName.String());
	appMime.SetLongDescription(appName.String());
	
	status_t result = appMime.Save();
	if (result != B_OK) {
		return result;
	}
	
	// Create desktop file
	result = CreateDesktopFile(appName, appSignature, mimeType);
	
	return result;
}

// Create desktop file for Tracker

status_t TrackerIntegration::CreateDesktopFile(const BString& appName, const BString& appSignature,
		const BString& mimeType) {
	
	// Create tracker directory if it doesn't exist
	status_t result = CreateTrackerDirectory();
	if (result != B_OK) {
		return result;
	}
	
	// Create desktop file content
	BString content;
	content << "application/" << appSignature << "\n";
	
	// Write desktop file
	BPath path;
	
	// Try system desktop database first
	if (find_directory(B_SYSTEM_DATA_DIRECTORY, &path) == B_OK) {
		path.Append("desktop_db");
		path.Append(appSignature.String());
		
		return WriteDesktopFile(path.Path(), content);
	}
	
	// Try user desktop database
	if (find_directory(B_USER_DATA_DIRECTORY, &path) == B_OK) {
		path.Append("desktop_db");
		path.Append(appSignature.String());
		
		return WriteDesktopFile(path.Path(), content);
	}
	
	return B_ERROR;
}

// Set file icon

status_t TrackerIntegration::SetFileIcon(const BString& mimeType, const BString& iconPath) {
	// Create MIME icon file
	return CreateMIMEIconFile(mimeType, iconPath);
}

// Set application icon

status_t TrackerIntegration::SetApplicationIcon(const BString& appSignature, const BString& iconPath) {
	// Set icon for the application
	BFile file(iconPath.String(), B_READ_ONLY);
	if (file.InitCheck() != B_OK) {
		return file.InitCheck();
	}
	
	BAppFileInfo appInfo(&file);
	
	// Load icon bitmap
	BBitmap* icon = BTranslationUtils::GetBitmap(&file);
	if (!icon) {
		return B_ERROR;
	}
	
	status_t result = appInfo.SetIcon(icon, B_LARGE_ICON);
	delete icon;
	return result;
}

// Get file info from Tracker

status_t TrackerIntegration::GetFileInfo(const BString& path, BString& mimeType, BString& appSignature) {
	BEntry entry(path.String());
	if (entry.InitCheck() != B_OK) {
		return entry.InitCheck();
	}
	
	BNode node(&entry);
	BAppFileInfo appInfo(&node);
	char mime[B_MIME_TYPE_LENGTH];
	
	if (appInfo.GetSignature(mime) == B_OK) {
		mimeType = mime;
		
		// Get preferred app
		char appSig[B_MIME_TYPE_LENGTH];
		if (appInfo.GetApp(appSig) == B_OK) {
			appSignature = appSig;
			return B_OK;
		}
	}
	
	return B_ERROR;
}

// Open file with default application

status_t TrackerIntegration::OpenFileWithDefaultApp(const BString& path) {
	BEntry entry(path.String());
	if (entry.InitCheck() != B_OK) {
		return entry.InitCheck();
	}
	
	BNode node(&entry);
	BAppFileInfo appInfo(&node);
	
	// Get preferred app
	char appSig[B_MIME_TYPE_LENGTH];
	if (appInfo.GetSignature(appSig) == B_OK) {
		// Launch the application
		BMessage message(B_REFS_RECEIVED);
		entry_ref ref;
		entry.GetRef(&ref);
		message.AddRef("refs", &ref);
		
		BMessenger messenger(appSig);
		if (messenger.IsValid()) {
			return messenger.SendMessage(&message);
		}
	}
	
	return B_ERROR;
}

// Check if file type is registered

bool TrackerIntegration::IsFileTypeRegistered(const BString& mimeType) {
	BMimeType mime(mimeType.String());
	return mime.IsValid();
}

// Helper methods

status_t TrackerIntegration::CreateTrackerDirectory() {
	BPath path;
	
	// Try to find the desktop database directory
	if (find_directory(B_SYSTEM_DATA_DIRECTORY, &path) == B_OK) {
		path.Append("desktop_db");
		
		BDirectory dir(path.Path());
		if (dir.InitCheck() == B_OK) {
			return B_OK;
		}
		
		// Try to create it
		if (create_directory(path.Path(), 0755) == B_OK) {
			return B_OK;
		}
	}
	
	// Try user desktop database
	if (find_directory(B_USER_DATA_DIRECTORY, &path) == B_OK) {
		path.Append("desktop_db");
		
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

status_t TrackerIntegration::WriteDesktopFile(const BString& path, const BString& content) {
	BFile file(path.String(), B_WRITE_ONLY | B_CREATE_FILE | B_ERASE_FILE);
	if (file.InitCheck() != B_OK) {
		return file.InitCheck();
	}
	
	ssize_t bytesWritten = file.Write(content.String(), content.Length());
	return bytesWritten == content.Length() ? B_OK : B_IO_ERROR;
}

status_t TrackerIntegration::CreateMIMEIconFile(const BString& mimeType, const BString& iconPath) {
	// Create MIME icon directory
	BPath path;
	
	if (find_directory(B_SYSTEM_DATA_DIRECTORY, &path) == B_OK) {
		path.Append("mime_db");
		path.Append("icons");
		path.Append(mimeType.String());
		
		// Create directory
		if (create_directory(path.Path(), 0755) != B_OK) {
			return B_ERROR;
		}
		
		// Copy icon file
		BEntry iconEntry(iconPath.String());
		if (iconEntry.Exists() && iconEntry.IsFile()) {
			BFile iconFile(iconPath.String(), B_READ_ONLY);
			if (iconFile.InitCheck() == B_OK) {
				BPath iconPath = path;
				iconPath.Append("icon.png");
				
				BFile destFile(iconPath.Path(), B_WRITE_ONLY | B_CREATE_FILE | B_ERASE_FILE);
				if (destFile.InitCheck() == B_OK) {
					// Copy file data
					char buffer[4096];
					ssize_t bytesRead;
					
					while ((bytesRead = iconFile.Read(buffer, sizeof(buffer))) > 0) {
						if (destFile.Write(buffer, bytesRead) != bytesRead) {
							return B_IO_ERROR;
						}
					}
					
					return B_OK;
				}
			}
		}
	}
	
	return B_ERROR;
}

} // namespace Core
} // namespace YellowDocs
