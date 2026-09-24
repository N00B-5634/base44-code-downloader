#include "Preferences.h"

#include <FindDirectory.h>
#include <Path.h>
#include <File.h>
#include <DataIO.h>

namespace YellowDocs {
namespace Utils {

// Preferences implementation

Preferences::Preferences(const BString& appName, const BString& fileName)
	: fAppName(appName),
	  fFileName(fileName),
	  fModified(false)
{
	InitializeFilePath();
}

Preferences::~Preferences() {
	// Save preferences on destruction if modified
	if (fModified) {
		Save();
	}
}

// Load preferences from file

status_t Preferences::Load() {
	BFile file(fFilePath.String(), B_READ_ONLY);
	if (file.InitCheck() != B_OK) {
		// File doesn't exist, create default
		return CreateDefaultFile();
	}
	
	// Read file into message
	BMessage message;
	if (message.Unflatten(&file) != B_OK) {
		return B_ERROR;
	}
	
	fPreferences = message;
	return B_OK;
}

// Save preferences to file

status_t Preferences::Save() {
	BFile file(fFilePath.String(), B_WRITE_ONLY | B_CREATE_FILE | B_ERASE_FILE);
	if (file.InitCheck() != B_OK) {
		return file.InitCheck();
	}
	
	// Flatten message to file
	if (fPreferences.Flatten(&file) != B_OK) {
		return B_ERROR;
	}
	
	fModified = false;
	return B_OK;
}

// Get preference value

bool Preferences::GetBool(const BString& key, bool defaultValue) const {
	bool value;
	if (fPreferences.FindBool(key.String(), &value) == B_OK) {
		return value;
	}
	return defaultValue;
}

int32 Preferences::GetInt32(const BString& key, int32 defaultValue) const {
	int32 value;
	if (fPreferences.FindInt32(key.String(), &value) == B_OK) {
		return value;
	}
	return defaultValue;
}

float Preferences::GetFloat(const BString& key, float defaultValue) const {
	float value;
	if (fPreferences.FindFloat(key.String(), &value) == B_OK) {
		return value;
	}
	return defaultValue;
}

BString Preferences::GetString(const BString& key, const BString& defaultValue) const {
	const char* value;
	if (fPreferences.FindString(key.String(), &value) == B_OK) {
		return BString(value);
	}
	return defaultValue;
}

BMessage Preferences::GetMessage(const BString& key, const BMessage& defaultValue) const {
	BMessage* value = nullptr;
	if (fPreferences.FindMessage(key.String(), &value) == B_OK && value) {
		return *value;
	}
	return defaultValue;
}

// Set preference value

void Preferences::SetBool(const BString& key, bool value) {
	fPreferences.ReplaceBool(key.String(), value);
	fModified = true;
}

void Preferences::SetInt32(const BString& key, int32 value) {
	fPreferences.ReplaceInt32(key.String(), value);
	fModified = true;
}

void Preferences::SetFloat(const BString& key, float value) {
	fPreferences.ReplaceFloat(key.String(), value);
	fModified = true;
}

void Preferences::SetString(const BString& key, const BString& value) {
	fPreferences.ReplaceString(key.String(), value.String());
	fModified = true;
}

void Preferences::SetMessage(const BString& key, const BMessage& value) {
	fPreferences.ReplaceMessage(key.String(), &value);
	fModified = true;
}

// Remove preference

void Preferences::Remove(const BString& key) {
	fPreferences.RemoveName(key.String());
	fModified = true;
}

// Check if preference exists

bool Preferences::Has(const BString& key) const {
	return fPreferences.HasName(key.String());
}

// Clear all preferences

void Preferences::Clear() {
	fPreferences.MakeEmpty();
	fModified = true;
}

// Get all preference keys

void Preferences::GetKeys(BVector<BString>& keys) const {
	keys.MakeEmpty();
	
	const char* name;
	int32 type;
	int32 count = fPreferences.CountNames(B_ANY_TYPE);
	
	for (int32 i = 0; i < count; i++) {
		if (fPreferences.GetNameInfo(i, &name, &type) == B_OK) {
			keys.Add(BString(name));
		}
	}
}

// Set file path

void Preferences::SetFilePath(const BString& path) {
	fFilePath = path;
}

// Get file path

BString Preferences::FilePath() const {
	return fFilePath;
}

// Initialize file path

void Preferences::InitializeFilePath() {
	// Try to find the user settings directory
	BPath path;
	
	if (find_directory(B_USER_SETTINGS_DIRECTORY, &path) == B_OK) {
		// Use app-specific subdirectory
		BString appDir = path.Path();
		appDir << "/" << fAppName;
		
		// Create directory if it doesn't exist
		BDirectory dir(appDir.String());
		if (dir.InitCheck() != B_OK) {
			create_directory(appDir.String(), 0755);
		}
		
		fFilePath = appDir;
		fFilePath << "/" << fFileName;
	} else {
		// Fallback to current directory
		fFilePath = fFileName;
	}
}

// Create default preferences file

status_t Preferences::CreateDefaultFile() {
	// Create an empty preferences file
	BFile file(fFilePath.String(), B_WRITE_ONLY | B_CREATE_FILE | B_ERASE_FILE);
	if (file.InitCheck() != B_OK) {
		return file.InitCheck();
	}
	
	// Write empty message
	BMessage empty;
	return empty.Flatten(&file);
}

} // namespace Utils
} // namespace YellowDocs
