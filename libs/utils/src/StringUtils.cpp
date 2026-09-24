#include "StringUtils.h"

#include <String.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace YellowDocs {
namespace Utils {

// Check if string is empty or whitespace

bool IsEmptyOrWhitespace(const BString& str) {
	for (int32 i = 0; i < str.Length(); i++) {
		if (!isspace(str[i])) {
			return false;
		}
	}
	return str.IsEmpty();
}

// Trim whitespace from both ends

BString Trim(const BString& str) {
	return TrimLeft(TrimRight(str));
}

// Trim whitespace from left

BString TrimLeft(const BString& str) {
	int32 start = 0;
	while (start < str.Length() && isspace(str[start])) {
		start++;
	}
	return str.String() + start;
}

// Trim whitespace from right

BString TrimRight(const BString& str) {
	int32 end = str.Length() - 1;
	while (end >= 0 && isspace(str[end])) {
		end--;
	}
	return str.String(0, end + 1);
}

// Convert to lowercase

BString ToLower(const BString& str) {
	BString result = str;
	for (int32 i = 0; i < result.Length(); i++) {
		result[i] = tolower(result[i]);
	}
	return result;
}

// Convert to uppercase

BString ToUpper(const BString& str) {
	BString result = str;
	for (int32 i = 0; i < result.Length(); i++) {
		result[i] = toupper(result[i]);
	}
	return result;
}

// Check if string starts with prefix

bool StartsWith(const BString& str, const BString& prefix) {
	if (prefix.Length() > str.Length()) {
		return false;
	}
	return strncmp(str.String(), prefix.String(), prefix.Length()) == 0;
}

// Check if string ends with suffix

bool EndsWith(const BString& str, const BString& suffix) {
	if (suffix.Length() > str.Length()) {
		return false;
	}
	return strncmp(str.String() + str.Length() - suffix.Length(), suffix.String(), suffix.Length()) == 0;
}

// Check if string contains substring

bool Contains(const BString& str, const BString& substring) {
	return str.FindFirst(substring) >= 0;
}

// Count occurrences of substring

int32 CountOccurrences(const BString& str, const BString& substring) {
	if (substring.IsEmpty()) {
		return 0;
	}
	
	int32 count = 0;
	int32 pos = 0;
	
	while ((pos = str.FindFirst(substring, pos)) >= 0) {
		count++;
		pos += substring.Length();
	}
	
	return count;
}

// Replace all occurrences of substring

BString ReplaceAll(const BString& str, const BString& from, const BString& to) {
	if (from.IsEmpty()) {
		return str;
	}
	
	BString result = str;
	int32 pos = 0;
	
	while ((pos = result.FindFirst(from, pos)) >= 0) {
		result.Replace(pos, from.Length(), to.String());
		pos += to.Length();
	}
	
	return result;
}

// Split string by delimiter

void Split(const BString& str, const BString& delimiter, BVector<BString>& result) {
	result.MakeEmpty();
	
	if (str.IsEmpty()) {
		return;
	}
	
	if (delimiter.IsEmpty()) {
		result.Add(str);
		return;
	}
	
	int32 start = 0;
	int32 pos = 0;
	
	while ((pos = str.FindFirst(delimiter, start)) >= 0) {
		result.Add(str.String(start, pos - start));
		start = pos + delimiter.Length();
	}
	
	// Add the last part
	if (start < str.Length()) {
		result.Add(str.String(start, str.Length() - start));
	}
}

// Split string by any of multiple delimiters

void SplitAny(const BString& str, const BVector<BString>& delimiters, BVector<BString>& result) {
	result.MakeEmpty();
	
	if (str.IsEmpty()) {
		return;
	}
	
	if (delimiters.CountItems() == 0) {
		result.Add(str);
		return;
	}
	
	int32 start = 0;
	int32 pos = 0;
	
	while (true) {
		pos = str.Length(); // Default to end of string
		
		// Find the earliest delimiter
		for (int32 i = 0; i < delimiters.CountItems(); i++) {
			const BString& delimiter = delimiters.ItemAt(i);
			if (!delimiter.IsEmpty()) {
				int32 found = str.FindFirst(delimiter, start);
				if (found >= 0 && found < pos) {
					pos = found;
				}
			}
		}
		
		if (pos >= str.Length()) {
			break;
		}
		
		// Find which delimiter was found
		int32 delimiterLength = 0;
		for (int32 i = 0; i < delimiters.CountItems(); i++) {
			const BString& delimiter = delimiters.ItemAt(i);
			if (!delimiter.IsEmpty() && str.FindFirst(delimiter, start) == pos) {
				delimiterLength = delimiter.Length();
				break;
			}
		}
		
		if (delimiterLength > 0) {
			result.Add(str.String(start, pos - start));
			start = pos + delimiterLength;
		} else {
			// No delimiter found, add remaining
			result.Add(str.String(start));
			break;
		}
	}
	
	// Add the last part
	if (start < str.Length()) {
		result.Add(str.String(start));
	}
}

// Join strings with delimiter

BString Join(const BVector<BString>& strings, const BString& delimiter) {
	if (strings.CountItems() == 0) {
		return "";
	}
	
	BString result = strings.ItemAt(0);
	
	for (int32 i = 1; i < strings.CountItems(); i++) {
		result << delimiter << strings.ItemAt(i);
	}
	
	return result;
}

// Check if string is numeric

bool IsNumeric(const BString& str) {
	if (str.IsEmpty()) {
		return false;
	}
	
	for (int32 i = 0; i < str.Length(); i++) {
		if (!isdigit(str[i])) {
			return false;
		}
	}
	
	return true;
}

// Check if string is integer

bool IsInteger(const BString& str) {
	if (str.IsEmpty()) {
		return false;
	}
	
	int32 i = 0;
	
	// Check for optional sign
	if (str[i] == '+' || str[i] == '-') {
		i++;
	}
	
	// Check for digits
	bool hasDigits = false;
	for (; i < str.Length(); i++) {
		if (!isdigit(str[i])) {
			return false;
		}
		hasDigits = true;
	}
	
	return hasDigits;
}

// Check if string is floating point

bool IsFloat(const BString& str) {
	if (str.IsEmpty()) {
		return false;
	}
	
	int32 i = 0;
	
	// Check for optional sign
	if (str[i] == '+' || str[i] == '-') {
		i++;
	}
	
	// Check for digits before decimal point
	bool hasDigits = false;
	for (; i < str.Length(); i++) {
		if (str[i] == '.') {
			break;
		}
		if (!isdigit(str[i])) {
			return false;
		}
		hasDigits = true;
	}
	
	// Check for decimal point
	if (i < str.Length() && str[i] == '.') {
		i++;
		
		// Check for digits after decimal point
		bool hasFractionalDigits = false;
		for (; i < str.Length(); i++) {
			if (!isdigit(str[i])) {
				return false;
			}
			hasFractionalDigits = true;
		}
		
		return hasDigits || hasFractionalDigits;
	}
	
	return hasDigits;
}

// Convert string to integer

int32 ToInteger(const BString& str, int32 defaultValue) {
	if (str.IsEmpty()) {
		return defaultValue;
	}
	
	return atoi(str.String());
}

// Convert string to float

float ToFloat(const BString& str, float defaultValue) {
	if (str.IsEmpty()) {
		return defaultValue;
	}
	
	return atof(str.String());
}

// Convert integer to string

BString ToString(int32 value) {
	char buffer[32];
	snprintf(buffer, sizeof(buffer), "%ld", value);
	return BString(buffer);
}

// Convert float to string

BString ToString(float value, int32 precision) {
	char buffer[64];
	char format[16];
	snprintf(format, sizeof(format), "%%.%ldf", precision);
	snprintf(buffer, sizeof(buffer), format, value);
	return BString(buffer);
}

// Convert boolean to string

BString ToString(bool value) {
	return value ? "true" : "false";
}

// Escape special characters

BString Escape(const BString& str) {
	BString result;
	
	for (int32 i = 0; i < str.Length(); i++) {
		char c = str[i];
		switch (c) {
			case '\n':
				result << "\\n";
				break;
			case '\r':
				result << "\\r";
				break;
			case '\t':
				result << "\\t";
				break;
			case '\\':
				result << "\\\\";
				break;
			case '"':
				result << "\\\"";
				break;
			case '\'':
				result << "\\'";
				break;
			default:
				result << c;
		}
	}
	
	return result;
}

// Unescape special characters

BString Unescape(const BString& str) {
	BString result;
	
	for (int32 i = 0; i < str.Length(); i++) {
		char c = str[i];
		
		if (c == '\\' && i + 1 < str.Length()) {
			switch (str[i + 1]) {
				case 'n':
					result << '\n';
					i++;
					break;
				case 'r':
					result << '\r';
					i++;
					break;
				case 't':
					result << '\t';
					i++;
					break;
				case '\\':
					result << '\\';
					i++;
					break;
				case '"':
					result << '"';
					i++;
					break;
				case '\'':
					result << '\'';
					i++;
					break;
				default:
					result << c;
			}
		} else {
			result << c;
		}
	}
	
	return result;
}

// URL encode string

BString URLEncode(const BString& str) {
	BString result;
	
	for (int32 i = 0; i < str.Length(); i++) {
		unsigned char c = str[i];
		
		if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
			result << c;
		} else if (c == ' ') {
			result << '+';
		} else {
			char buffer[4];
			snprintf(buffer, sizeof(buffer), "%%%02X", c);
			result << buffer;
		}
	}
	
	return result;
}

// URL decode string

BString URLDecode(const BString& str) {
	BString result;
	
	for (int32 i = 0; i < str.Length(); i++) {
		char c = str[i];
		
		if (c == '+') {
			result << ' ';
		} else if (c == '%' && i + 2 < str.Length()) {
			char hex[3] = {str[i + 1], str[i + 2], '\0'};
			char decoded = static_cast<char>(strtol(hex, nullptr, 16));
			result << decoded;
			i += 2;
		} else {
			result << c;
		}
	}
	
	return result;
}

// HTML encode string

BString HTMLEncode(const BString& str) {
	BString result;
	
	for (int32 i = 0; i < str.Length(); i++) {
		char c = str[i];
		switch (c) {
			case '<':
				result << "&lt;";
				break;
			case '>':
				result << "&gt;";
				break;
			case '&':
				result << "&amp;";
				break;
			case '"':
				result << "&quot;";
				break;
			case '\'':
				result << "&apos;";
				break;
			default:
				result << c;
		}
	}
	
	return result;
}

// HTML decode string

BString HTMLDecode(const BString& str) {
	BString result;
	
	for (int32 i = 0; i < str.Length(); i++) {
		if (str[i] == '&' && i + 1 < str.Length()) {
			if (str[i + 1] == '#') {
				// Numeric entity
				int32 j = i + 2;
				while (j < str.Length() && str[j] != ';') {
					j++;
				}
				
				if (j < str.Length() && str[j] == ';') {
					BString numStr = str.String(i + 2, j - (i + 2));
					int32 num = ToInteger(numStr, 0);
					if (num > 0) {
						result << static_cast<char>(num);
						i = j;
					} else {
						result << str[i];
					}
				} else {
					result << str[i];
				}
			} else {
				// Named entity
				if (str[i + 1] == 'l' && i + 3 < str.Length() && str[i + 2] == 't' && str[i + 3] == ';') {
					result << '<';
					i += 3;
				} else if (str[i + 1] == 'g' && i + 3 < str.Length() && str[i + 2] == 't' && str[i + 3] == ';') {
					result << '>';
					i += 3;
				} else if (str[i + 1] == 'a' && i + 4 < str.Length() && str[i + 2] == 'm' && 
					str[i + 3] == 'p' && str[i + 4] == ';') {
					result << '&';
					i += 4;
				} else if (str[i + 1] == 'q' && i + 5 < str.Length() && str[i + 2] == 'u' && 
					str[i + 3] == 'o' && str[i + 4] == 't' && str[i + 5] == ';') {
					result << '"';
					i += 5;
				} else if (str[i + 1] == 'a' && i + 5 < str.Length() && str[i + 2] == 'p' && 
					str[i + 3] == 'o' && str[i + 4] == 's' && str[i + 5] == ';') {
					result << '\'';
					i += 5;
				} else {
					result << str[i];
				}
			}
		} else {
			result << str[i];
		}
	}
	
	return result;
}

// Compare strings case-insensitive

int CompareCaseInsensitive(const BString& a, const BString& b) {
	return strcasecmp(a.String(), b.String());
}

// Check if strings are equal case-insensitive

bool EqualsCaseInsensitive(const BString& a, const BString& b) {
	return CompareCaseInsensitive(a, b) == 0;
}

// Get substring before first occurrence

BString SubstringBefore(const BString& str, const BString& delimiter) {
	int32 pos = str.FindFirst(delimiter);
	if (pos >= 0) {
		return str.String(0, pos);
	}
	return str;
}

// Get substring after first occurrence

BString SubstringAfter(const BString& str, const BString& delimiter) {
	int32 pos = str.FindFirst(delimiter);
	if (pos >= 0) {
		return str.String(pos + delimiter.Length());
	}
	return "";
}

// Get substring before last occurrence

BString SubstringBeforeLast(const BString& str, const BString& delimiter) {
	int32 pos = str.FindLast(delimiter);
	if (pos >= 0) {
		return str.String(0, pos);
	}
	return str;
}

// Get substring after last occurrence

BString SubstringAfterLast(const BString& str, const BString& delimiter) {
	int32 pos = str.FindLast(delimiter);
	if (pos >= 0) {
		return str.String(pos + delimiter.Length());
	}
	return "";
}

// Get first non-empty line from multi-line string

BString FirstNonEmptyLine(const BString& str) {
	BVector<BString> lines;
	Split(str, "\n", lines);
	
	for (int32 i = 0; i < lines.CountItems(); i++) {
		BString line = Trim(lines.ItemAt(i));
		if (!line.IsEmpty()) {
			return line;
		}
	}
	
	return "";
}

// Get last non-empty line from multi-line string

BString LastNonEmptyLine(const BString& str) {
	BVector<BString> lines;
	Split(str, "\n", lines);
	
	for (int32 i = lines.CountItems() - 1; i >= 0; i--) {
		BString line = Trim(lines.ItemAt(i));
		if (!line.IsEmpty()) {
			return line;
		}
	}
	
	return "";
}

// Count lines in string

int32 CountLines(const BString& str) {
	if (str.IsEmpty()) {
		return 0;
	}
	
	return CountOccurrences(str, "\n") + 1;
}

// Get line at index

BString GetLine(const BString& str, int32 index) {
	BVector<BString> lines;
	Split(str, "\n", lines);
	
	if (index >= 0 && index < lines.CountItems()) {
		return lines.ItemAt(index);
	}
	
	return "";
}

// Wrap text to specified width

BString WordWrap(const BString& text, float maxWidth, const BFont& font) {
	// This is a placeholder implementation
	// A real implementation would measure text and wrap at word boundaries
	
	BVector<BString> words;
	Split(text, " ", words);
	
	BString result;
	BString currentLine;
	
	for (int32 i = 0; i < words.CountItems(); i++) {
		BString word = words.ItemAt(i);
		
		// Check if adding this word would exceed the width
		// This is simplified - in reality we'd need to measure the text
		if (!currentLine.IsEmpty()) {
			currentLine << " ";
		}
		
		float currentWidth = font.StringWidth(currentLine.String() + word.String());
		
		if (currentWidth > maxWidth && !currentLine.IsEmpty()) {
			// Start new line
			result << currentLine << "\n";
			currentLine = word;
		} else {
			currentLine << word;
		}
	}
	
	if (!currentLine.IsEmpty()) {
		result << currentLine;
	}
	
	return result;
}

} // namespace Utils
} // namespace YellowDocs
