#include "FileUtils.h"

#include <AppFileInfo.h>
#include <FindDirectory.h>
#include <fs_attr.h>
#include <string.h>

namespace YellowDocs {
namespace Utils {

// Check if file exists

bool FileExists(const BString& path) {
	BFile file(path.String(), B_READ_ONLY);
	return file.InitCheck() == B_OK;
}

// Check if directory exists

bool DirectoryExists(const BString& path) {
	BDirectory dir(path.String());
	return dir.InitCheck() == B_OK;
}

// Create directory (including parent directories)

status_t CreateDirectory(const BString& path, bool createParents) {
	if (DirectoryExists(path)) {
		return B_OK;
	}
	
	if (createParents) {
		BPath parentPath(path.String());
		parentPath.GetParent(&parentPath);
		
		if (!parentPath.IsEmpty()) {
			status_t result = CreateDirectory(parentPath.Path(), true);
			if (result != B_OK) {
				return result;
			}
		}
	}
	
	BDirectory dir;
	return dir.CreateDirectory(path.String());
}

// Get file size

status_t GetFileSize(const BString& path, off_t* size) {
	BFile file(path.String(), B_READ_ONLY);
	if (file.InitCheck() != B_OK) {
		return file.InitCheck();
	}
	
	return file.GetSize(size);
}

// Get file modification time

status_t GetFileModificationTime(const BString& path, time_t* modTime) {
	BEntry entry(path.String());
	if (entry.InitCheck() != B_OK) {
		return entry.InitCheck();
	}
	
	entry.GetModificationTime(modTime);
	return B_OK;
}

// Get file name from path

BString GetFileName(const BString& path) {
	BPath bpath(path.String());
	return bpath.Leaf();
}

// Get directory from path

BString GetDirectory(const BString& path) {
	BPath bpath(path.String());
	BString dir;
	bpath.GetParent(&bpath);
	return bpath.Path();
}

// Get file extension

BString GetFileExtension(const BString& path) {
	BPath bpath(path.String());
	return bpath.PathExtension();
}

// Get file name without extension

BString GetFileNameWithoutExtension(const BString& path) {
	BString filename = GetFileName(path);
	BPath bpath(filename.String());
	BString name = bpath.Leaf();
	
	// Remove extension
	int32 dotIndex = name.FindLast('.');
	if (dotIndex > 0) {
		name.Truncate(dotIndex);
	}
	
	return name;
}

// Join paths

BString JoinPaths(const BString& path1, const BString& path2) {
	if (path1.IsEmpty()) {
		return path2;
	}
	if (path2.IsEmpty()) {
		return path1;
	}
	
	BString result = path1;
	
	// Ensure there's exactly one path separator between them
	if (!result.EndsWith("/") && !path2.StartsWith("/")) {
		result << "/";
	} else if (result.EndsWith("/") && path2.StartsWith("/")) {
		result.Truncate(result.Length() - 1);
	}
	
	result << path2;
	return result;
}

// Normalize path (resolve .. and .)

BString NormalizePath(const BString& path) {
	BPath bpath(path.String());
	bpath.Normalize();
	return bpath.Path();
}

// Get temporary directory

BString GetTemporaryDirectory() {
	BPath path;
	find_directory(B_SYSTEM_TEMP_DIRECTORY, &path);
	return path.Path();
}

// Create temporary file

BString CreateTemporaryFile(const BString& prefix) {
	BPath tempDir = GetTemporaryDirectory();
	
	BString tempName;
	tempName << tempDir << "/" << prefix << system_time();
	
	BFile file(tempName.String(), B_WRITE_ONLY | B_CREATE_FILE | B_ERASE_FILE);
	if (file.InitCheck() == B_OK) {
		return tempName;
	}
	
	// Fallback
	return BString(tempDir) << "/" << prefix << "_temp";
}

// Get application support directory

BString GetApplicationSupportDirectory(const BString& appName) {
	BPath path;
	
	// Try user settings directory first
	if (find_directory(B_USER_SETTINGS_DIRECTORY, &path) == B_OK) {
		BString dir = JoinPaths(path.Path(), appName);
		CreateDirectory(dir, true);
		return dir;
	}
	
	// Fallback to system settings
	if (find_directory(B_SYSTEM_SETTINGS_DIRECTORY, &path) == B_OK) {
		BString dir = JoinPaths(path.Path(), appName);
		CreateDirectory(dir, true);
		return dir;
	}
	
	return BString();
}

// Get user documents directory

BString GetUserDocumentsDirectory() {
	BPath path;
	
	// Try user documents directory
	if (find_directory(B_USER_DOCUMENTS_DIRECTORY, &path) == B_OK) {
		return path.Path();
	}
	
	// Fallback to home directory
	if (find_directory(B_HOME_DIRECTORY, &path) == B_OK) {
		BString docsDir = JoinPaths(path.Path(), "Documents");
		CreateDirectory(docsDir, true);
		return docsDir;
	}
	
	return BString();
}

// Get system settings directory

BString GetSystemSettingsDirectory() {
	BPath path;
	
	if (find_directory(B_SYSTEM_SETTINGS_DIRECTORY, &path) == B_OK) {
		return path.Path();
	}
	
	return BString();
}

// Read entire file into string

status_t ReadFileToString(const BString& path, BString& content) {
	BFile file(path.String(), B_READ_ONLY);
	if (file.InitCheck() != B_OK) {
		return file.InitCheck();
	}
	
	off_t size;
	file.GetSize(&size);
	
	if (size <= 0) {
		content = "";
		return B_OK;
	}
	
	char* buffer = new char[size + 1];
	ssize_t bytesRead = file.Read(buffer, size);
	
	if (bytesRead == size) {
		buffer[size] = '\0';
		content = buffer;
	} else {
		content = "";
	}
	
	delete[] buffer;
	return bytesRead == size ? B_OK : B_IO_ERROR;
}

// Write string to file

status_t WriteStringToFile(const BString& path, const BString& content) {
	BFile file(path.String(), B_WRITE_ONLY | B_CREATE_FILE | B_ERASE_FILE);
	if (file.InitCheck() != B_OK) {
		return file.InitCheck();
	}
	
	ssize_t bytesWritten = file.Write(content.String(), content.Length());
	return bytesWritten == content.Length() ? B_OK : B_IO_ERROR;
}

// Copy file

status_t CopyFile(const BString& source, const BString& destination) {
	BFile src(source.String(), B_READ_ONLY);
	if (src.InitCheck() != B_OK) {
		return src.InitCheck();
	}
	
	BFile dst(destination.String(), B_WRITE_ONLY | B_CREATE_FILE | B_ERASE_FILE);
	if (dst.InitCheck() != B_OK) {
		return dst.InitCheck();
	}
	
	char buffer[4096];
	ssize_t bytesRead;
	
	while ((bytesRead = src.Read(buffer, sizeof(buffer))) > 0) {
		if (dst.Write(buffer, bytesRead) != bytesRead) {
			return B_IO_ERROR;
		}
	}
	
	return B_OK;
}

// Move file

status_t MoveFile(const BString& source, const BString& destination) {
	BEntry srcEntry(source.String());
	if (srcEntry.InitCheck() != B_OK) {
		return srcEntry.InitCheck();
	}
	
	BEntry dstEntry(destination.String());
	if (dstEntry.InitCheck() != B_OK) {
		return dstEntry.InitCheck();
	}
	
	return srcEntry.Rename(&dstEntry, true);
}

// Delete file

status_t DeleteFile(const BString& path) {
	BEntry entry(path.String());
	if (entry.InitCheck() != B_OK) {
		return entry.InitCheck();
	}
	
	return entry.Remove();
}

// Check if path is absolute

bool IsAbsolutePath(const BString& path) {
	return path.StartsWith("/");
}

// Make path absolute

BString MakeAbsolutePath(const BString& path) {
	if (IsAbsolutePath(path)) {
		return path;
	}
	
	BPath current;
	getcwd(current.Path(), current.PathCapacity());
	
	BPath bpath(path.String());
	BPath absPath(current.Path());
	absPath.Append(bpath.Path());
	absPath.Normalize();
	
	return absPath.Path();
}

// Get relative path from base directory

BString GetRelativePath(const BString& path, const BString& baseDirectory) {
	BPath bpath(path.String());
	BPath basePath(baseDirectory.String());
	
	bpath.Normalize();
	basePath.Normalize();
	
	// Check if path starts with base directory
	BString pathStr = bpath.Path();
	BString baseStr = basePath.Path();
	
	if (pathStr.StartsWith(baseStr)) {
		BString relative = pathStr;
		relative.RemoveFirst(baseStr);
		
		// Remove leading slash
		if (relative.StartsWith("/")) {
			relative.RemoveFirst("/");
		}
		
		// Handle empty case
		if (relative.IsEmpty()) {
			return ".";
		}
		
		return relative;
	}
	
	// Not relative to base, return original
	return path;
}

// List files in directory

status_t ListFiles(const BString& path, BVector<BString>& files, bool recursive) {
	BDirectory dir(path.String());
	if (dir.InitCheck() != B_OK) {
		return dir.InitCheck();
	}
	
	BEntry entry;
	while (dir.GetNextEntry(&entry) == B_OK) {
		BString name = entry.Name();
		
		if (entry.IsDirectory()) {
			if (recursive && strcmp(name.String(), ".") != 0 && strcmp(name.String(), "..") != 0) {
				BString subPath = JoinPaths(path, name);
				BVector<BString> subFiles;
				ListFiles(subPath, subFiles, true);
				
				for (int32 i = 0; i < subFiles.CountItems(); i++) {
					files.Add(subFiles.ItemAt(i));
				}
			}
		} else {
			files.Add(name);
		}
	}
	
	return B_OK;
}

// Check if file is readable

bool IsFileReadable(const BString& path) {
	BFile file(path.String(), B_READ_ONLY);
	return file.InitCheck() == B_OK;
}

// Check if file is writable

bool IsFileWritable(const BString& path) {
	BFile file(path.String(), B_WRITE_ONLY | B_OPEN_AT_END);
	return file.InitCheck() == B_OK;
}

// Get file MIME type

BString GetFileMIMEType(const BString& path) {
	BFile file(path.String(), B_READ_ONLY);
	if (file.InitCheck() != B_OK) {
		return "";
	}
	
	BAppFileInfo appInfo(&file);
	char mimeType[B_MIME_TYPE_LENGTH];
	
	if (appInfo.GetType(mimeType) == B_OK) {
		return BString(mimeType);
	}
	
	return "";
}

// Set file MIME type

status_t SetFileMIMEType(const BString& path, const BString& mimeType) {
	BFile file(path.String(), B_READ_WRITE);
	if (file.InitCheck() != B_OK) {
		return file.InitCheck();
	}
	
	BAppFileInfo appInfo(&file);
	return appInfo.SetType(mimeType.String());
}

} // namespace Utils
} // namespace YellowDocs
