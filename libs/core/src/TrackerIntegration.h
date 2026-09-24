#ifndef YELLOWDOCS_CORE_TRACKER_INTEGRATION_H
#define YELLOWDOCS_CORE_TRACKER_INTEGRATION_H

#include <SupportDefs.h>
#include <String.h>
#include <Vector.h>

namespace YellowDocs {
namespace Core {

// Tracker integration class
class TrackerIntegration {
public:
	TrackerIntegration();
	~TrackerIntegration();

	// Register file type with Tracker
	status_t RegisterFileType(const BString& mimeType, const BString& extension,
		const BString& description, const BString& appSignature);
	
	// Register application with Tracker
	status_t RegisterApplication(const BString& appName, const BString& appSignature,
		const BString& mimeType);
	
	// Create desktop file for Tracker
	status_t CreateDesktopFile(const BString& appName, const BString& appSignature,
		const BString& mimeType);
	
	// Set file icon
	status_t SetFileIcon(const BString& mimeType, const BString& iconPath);
	
	// Set application icon
	status_t SetApplicationIcon(const BString& appSignature, const BString& iconPath);
	
	// Get file info from Tracker
	status_t GetFileInfo(const BString& path, BString& mimeType, BString& appSignature);
	
	// Open file with default application
	status_t OpenFileWithDefaultApp(const BString& path);
	
	// Check if file type is registered
	bool IsFileTypeRegistered(const BString& mimeType);
	
private:
	// Helper methods
	status_t CreateTrackerDirectory();
	status_t WriteDesktopFile(const BString& path, const BString& content);
	status_t CreateMIMEIconFile(const BString& mimeType, const BString& iconPath);
};

} // namespace Core
} // namespace YellowDocs

#endif // YELLOWDOCS_CORE_TRACKER_INTEGRATION_H
