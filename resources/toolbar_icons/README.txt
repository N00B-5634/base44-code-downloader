YellowDocs Writer Toolbar Icons
===================================

This directory contains SVG vector icons for the YellowDocs Writer toolbar.

Icon Files (24x24 pixels):
- new.svg: New document icon
- open.svg: Open document icon
- save.svg: Save document icon
- bold.svg: Bold formatting icon
- italic.svg: Italic formatting icon
- underline.svg: Underline formatting icon
- align_left.svg: Left alignment icon
- align_center.svg: Center alignment icon
- align_right.svg: Right alignment icon
- align_justify.svg: Justify alignment icon

Icon Design Guidelines:
- Size: 24x24 pixels (SVG is scalable)
- Color: Use warm yellow/amber (#FFBF00) for active states and accents
- Style: Flat, clean icons matching Haiku system style
- Background: Transparent background
- Format: SVG vector format for scalability

Usage:
These icons are used in the ToolbarView class. Each toolbar button can display 
these icons along with or instead of text labels.

Conversion to PNG:
To use these icons in Haiku (which may require PNG format), convert using:
1. Inkscape: File -> Export PNG Image
2. Set size to 24x24 pixels
3. Ensure transparent background
4. Save as PNG

Icon States:
Toolbar icons can have different visual states:
- Normal: Default appearance
- Hover: Slightly highlighted
- Active: Yellow accent color
- Disabled: Grayed out

All icons are original creations for YellowDocs Writer.
