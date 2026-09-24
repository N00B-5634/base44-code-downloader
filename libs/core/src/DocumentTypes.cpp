#include "DocumentTypes.h"

namespace YellowDocs {
namespace Core {

// Document type utility functions

const char* GetFileExtension(FileFormat format) {
	switch (format) {
		case FORMAT_NATIVE:
			return ".ywrite";
		case FORMAT_PLAIN_TEXT:
			return ".txt";
		case FORMAT_HTML:
			return ".html";
		case FORMAT_PDF:
			return ".pdf";
		default:
			return ".ywrite";
	}
}

const char* GetMIMEType(FileFormat format) {
	switch (format) {
		case FORMAT_NATIVE:
			return "application/x-vnd.yellowdocs-writer";
		case FORMAT_PLAIN_TEXT:
			return "text/plain";
		case FORMAT_HTML:
			return "text/html";
		case FORMAT_PDF:
			return "application/pdf";
		default:
			return "application/x-vnd.yellowdocs-writer";
	}
}

const char* GetFormatName(FileFormat format) {
	switch (format) {
		case FORMAT_NATIVE:
			return "YellowDocs Writer Document";
		case FORMAT_PLAIN_TEXT:
			return "Plain Text";
		case FORMAT_HTML:
			return "HTML";
		case FORMAT_PDF:
			return "PDF";
		default:
			return "YellowDocs Writer Document";
	}
}

const char* GetAlignmentName(TextAlignment alignment) {
	switch (alignment) {
		case TEXT_ALIGN_LEFT:
			return "Left";
		case TEXT_ALIGN_CENTER:
			return "Center";
		case TEXT_ALIGN_RIGHT:
			return "Right";
		case TEXT_ALIGN_JUSTIFY:
			return "Justify";
		default:
			return "Left";
	}
}

const char* GetListStyleName(ListStyle style) {
	switch (style) {
		case LIST_STYLE_NONE:
			return "None";
		case LIST_STYLE_BULLET:
			return "Bullets";
		case LIST_STYLE_NUMBERED:
			return "Numbers";
		case LIST_STYLE_LETTERED:
			return "Letters";
		default:
			return "None";
	}
}

const char* GetPageSizeName(PageSize size) {
	switch (size) {
		case PAGE_SIZE_A4:
			return "A4";
		case PAGE_SIZE_LETTER:
			return "Letter";
		case PAGE_SIZE_A5:
			return "A5";
		case PAGE_SIZE_LEGAL:
			return "Legal";
		case PAGE_SIZE_CUSTOM:
			return "Custom";
		default:
			return "A4";
	}
}

} // namespace Core
} // namespace YellowDocs
