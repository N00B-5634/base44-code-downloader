#ifndef YELLOWDOCS_CORE_DOCUMENT_TYPES_H
#define YELLOWDOCS_CORE_DOCUMENT_TYPES_H

#include <SupportDefs.h>

namespace YellowDocs {
namespace Core {

// Document file format version
const uint32 kDocumentFormatVersion = 1;

// Document element types
enum DocumentElementType {
	DOC_ELEMENT_TEXT,
	DOC_ELEMENT_PARAGRAPH,
	DOC_ELEMENT_IMAGE,
	DOC_ELEMENT_TABLE,
	DOC_ELEMENT_LIST,
	DOC_ELEMENT_LIST_ITEM
};

// Text alignment
enum TextAlignment {
	TEXT_ALIGN_LEFT,
	TEXT_ALIGN_CENTER,
	TEXT_ALIGN_RIGHT,
	TEXT_ALIGN_JUSTIFY
};

// Vertical alignment
enum VerticalAlignment {
	VERT_ALIGN_TOP,
	VERT_ALIGN_MIDDLE,
	VERT_ALIGN_BOTTOM
};

// List style
enum ListStyle {
	LIST_STYLE_NONE,
	LIST_STYLE_BULLET,
	LIST_STYLE_NUMBERED,
	LIST_STYLE_LETTERED
};

// Unit types for measurements
enum UnitType {
	UNIT_PIXELS,
	UNIT_POINTS,
	UNIT_INCHES,
	UNIT_CENTIMETERS,
	UNIT_MILLIMETERS
};

// Page orientation
enum PageOrientation {
	PAGE_PORTRAIT,
	PAGE_LANDSCAPE
};

// Page size presets
enum PageSize {
	PAGE_SIZE_A4,
	PAGE_SIZE_LETTER,
	PAGE_SIZE_A5,
	PAGE_SIZE_LEGAL,
	PAGE_SIZE_CUSTOM
};

// Document view modes
enum ViewMode {
	VIEW_MODE_PRINT_LAYOUT,
	VIEW_MODE_DRAFT,
	VIEW_MODE_WEB_LAYOUT
};

// Zoom levels
enum ZoomLevel {
	ZOOM_50,
	ZOOM_75,
	ZOOM_100,
	ZOOM_125,
	ZOOM_150,
	ZOOM_200,
	ZOOM_CUSTOM
};

// Document change types for undo/redo
enum ChangeType {
	CHANGE_INSERT_TEXT,
	CHANGE_DELETE_TEXT,
	CHANGE_FORMAT_TEXT,
	CHANGE_INSERT_PARAGRAPH,
	CHANGE_DELETE_PARAGRAPH,
	CHANGE_FORMAT_PARAGRAPH,
	CHANGE_INSERT_IMAGE,
	CHANGE_DELETE_IMAGE,
	CHANGE_RESIZE_IMAGE,
	CHANGE_INSERT_TABLE,
	CHANGE_DELETE_TABLE,
	CHANGE_MODIFY_TABLE,
	CHANGE_DOCUMENT_SETTINGS
};

// File format types
enum FileFormat {
	FORMAT_NATIVE,    // .ywrite
	FORMAT_PLAIN_TEXT,
	FORMAT_HTML,
	FORMAT_PDF
};

} // namespace Core
} // namespace YellowDocs

#endif // YELLOWDOCS_CORE_DOCUMENT_TYPES_H
