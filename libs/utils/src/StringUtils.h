#ifndef YELLOWDOCS_UTILS_STRING_UTILS_H
#define YELLOWDOCS_UTILS_STRING_UTILS_H

#include <SupportDefs.h>
#include <String.h>
#include <Vector.h>

namespace YellowDocs {
namespace Utils {

// String utility functions

// Check if string is empty or whitespace
bool IsEmptyOrWhitespace(const BString& str);

// Trim whitespace from both ends
BString Trim(const BString& str);

// Trim whitespace from left
BString TrimLeft(const BString& str);

// Trim whitespace from right
BString TrimRight(const BString& str);

// Convert to lowercase
BString ToLower(const BString& str);

// Convert to uppercase
BString ToUpper(const BString& str);

// Check if string starts with prefix
bool StartsWith(const BString& str, const BString& prefix);

// Check if string ends with suffix
bool EndsWith(const BString& str, const BString& suffix);

// Check if string contains substring
bool Contains(const BString& str, const BString& substring);

// Count occurrences of substring
int32 CountOccurrences(const BString& str, const BString& substring);

// Replace all occurrences of substring
BString ReplaceAll(const BString& str, const BString& from, const BString& to);

// Split string by delimiter
void Split(const BString& str, const BString& delimiter, BVector<BString>& result);

// Split string by any of multiple delimiters
void SplitAny(const BString& str, const BVector<BString>& delimiters, BVector<BString>& result);

// Join strings with delimiter
BString Join(const BVector<BString>& strings, const BString& delimiter);

// Check if string is numeric
bool IsNumeric(const BString& str);

// Check if string is integer
bool IsInteger(const BString& str);

// Check if string is floating point
bool IsFloat(const BString& str);

// Convert string to integer
int32 ToInteger(const BString& str, int32 defaultValue = 0);

// Convert string to float
float ToFloat(const BString& str, float defaultValue = 0.0f);

// Convert integer to string
BString ToString(int32 value);

// Convert float to string
BString ToString(float value, int32 precision = 2);

// Convert boolean to string
BString ToString(bool value);

// Escape special characters
BString Escape(const BString& str);

// Unescape special characters
BString Unescape(const BString& str);

// URL encode string
BString URLEncode(const BString& str);

// URL decode string
BString URLDecode(const BString& str);

// HTML encode string
BString HTMLEncode(const BString& str);

// HTML decode string
BString HTMLDecode(const BString& str);

// Compare strings case-insensitive
int CompareCaseInsensitive(const BString& a, const BString& b);

// Check if strings are equal case-insensitive
bool EqualsCaseInsensitive(const BString& a, const BString& b);

// Get substring before first occurrence
BString SubstringBefore(const BString& str, const BString& delimiter);

// Get substring after first occurrence
BString SubstringAfter(const BString& str, const BString& delimiter);

// Get substring before last occurrence
BString SubstringBeforeLast(const BString& str, const BString& delimiter);

// Get substring after last occurrence
BString SubstringAfterLast(const BString& str, const BString& delimiter);

// Get first non-empty line from multi-line string
BString FirstNonEmptyLine(const BString& str);

// Get last non-empty line from multi-line string
BString LastNonEmptyLine(const BString& str);

// Count lines in string
int32 CountLines(const BString& str);

// Get line at index
BString GetLine(const BString& str, int32 index);

// Wrap text to specified width
BString WordWrap(const BString& text, float maxWidth, const BFont& font);

} // namespace Utils
} // namespace YellowDocs

#endif // YELLOWDOCS_UTILS_STRING_UTILS_H
