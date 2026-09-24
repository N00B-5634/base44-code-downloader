#ifndef YELLOWDOCS_UTILS_FILE_UTILS_H
#define YELLOWDOCS_UTILS_FILE_UTILS_H

#include <SupportDefs.h>
#include <String.h>
#include <File.h>
#include <Path.h>
#include <Directory.h>
#include <Entry.h>

namespace YellowDocs {
namespace Utils {

// File utility functions

// Check if file exists
bool FileExists(const BString& path);

// Check if directory exists
bool DirectoryExists(const BString& path);

// Create directory (including parent directories)
status_t CreateDirectory(const BString& path, bool createParents = true);

// Get file size
status_t GetFileSize(const BString& path, off_t* size);

// Get file modification time
status_t GetFileModificationTime(const BString& path, time_t* modTime);

// Get file name from path
BString GetFileName(const BString& path);

// Get directory from path
BString GetDirectory(const BString& path);

// Get file extension
BString GetFileExtension(const BString& path);

// Get file name without extension
BString GetFileNameWithoutExtension(const BString& path);

// Join paths
BString JoinPaths(const BString& path1, const BString& path2);

// Normalize path (resolve .. and .)
BString NormalizePath(const BString& path);

// Get temporary directory
BString GetTemporaryDirectory();

// Create temporary file
BString CreateTemporaryFile(const BString& prefix = "yellowdocs");

// Get application support directory
BString GetApplicationSupportDirectory(const BString& appName);

// Get user documents directory
BString GetUserDocumentsDirectory();

// Get system settings directory
BString GetSystemSettingsDirectory();

// Read entire file into string
status_t ReadFileToString(const BString& path, BString& content);

// Write string to file
status_t WriteStringToFile(const BString& path, const BString& content);

// Copy file
status_t CopyFile(const BString& source, const BString& destination);

// Move file
status_t MoveFile(const BString& source, const BString& destination);

// Delete file
status_t DeleteFile(const BString& path);

// Check if path is absolute
bool IsAbsolutePath(const BString& path);

// Make path absolute
BString MakeAbsolutePath(const BString& path);

// Get relative path from base directory
BString GetRelativePath(const BString& path, const BString& baseDirectory);

// List files in directory
status_t ListFiles(const BString& path, BVector<BString>& files, bool recursive = false);

// Check if file is readable
bool IsFileReadable(const BString& path);

// Check if file is writable
bool IsFileWritable(const BString& path);

// Get file MIME type
BString GetFileMIMEType(const BString& path);

// Set file MIME type
status_t SetFileMIMEType(const BString& path, const BString& mimeType);

} // namespace Utils
} // namespace YellowDocs

#endif // YELLOWDOCS_UTILS_FILE_UTILS_H
