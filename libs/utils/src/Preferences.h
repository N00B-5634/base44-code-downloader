#ifndef YELLOWDOCS_UTILS_PREFERENCES_H
#define YELLOWDOCS_UTILS_PREFERENCES_H

#include <SupportDefs.h>
#include <String.h>
#include <Message.h>
#include <File.h>

namespace YellowDocs {
namespace Utils {

// Preferences class for managing application settings
class Preferences {
public:
	Preferences(const BString& appName, const BString& fileName = "settings");
	~Preferences();

	// Load preferences from file
	status_t Load();

	// Save preferences to file
	status_t Save();

	// Get preference value
	bool GetBool(const BString& key, bool defaultValue = false) const;
	int32 GetInt32(const BString& key, int32 defaultValue = 0) const;
	float GetFloat(const BString& key, float defaultValue = 0.0f) const;
	BString GetString(const BString& key, const BString& defaultValue = "") const;
	BMessage GetMessage(const BString& key, const BMessage& defaultValue = BMessage()) const;

	// Set preference value
	void SetBool(const BString& key, bool value);
	void SetInt32(const BString& key, int32 value);
	void SetFloat(const BString& key, float value);
	void SetString(const BString& key, const BString& value);
	void SetMessage(const BString& key, const BMessage& value);

	// Remove preference
	void Remove(const BString& key);

	// Check if preference exists
	bool Has(const BString& key) const;

	// Clear all preferences
	void Clear();

	// Get all preference keys
	void GetKeys(BVector<BString>& keys) const;

	// Set file path
	void SetFilePath(const BString& path);

	// Get file path
	BString FilePath() const;

private:
	BString fAppName;
	BString fFileName;
	BString fFilePath;
	BMessage fPreferences;
	bool fModified;
	
	// Initialize file path
	void InitializeFilePath();
	
	// Create default preferences file
	status_t CreateDefaultFile();
};

} // namespace Utils
} // namespace YellowDocs

#endif // YELLOWDOCS_UTILS_PREFERENCES_H
