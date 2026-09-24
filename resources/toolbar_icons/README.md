# Toolbar Icons

This directory contains icons for the YellowDocs Writer toolbar.

## Icon Files

- `new.png`: New document (24x24)
- `open.png`: Open document (24x24)
- `save.png`: Save document (24x24)
- `bold.png`: Bold formatting (24x24)
- `italic.png`: Italic formatting (24x24)
- `underline.png`: Underline formatting (24x24)
- `align_left.png`: Left alignment (24x24)
- `align_center.png`: Center alignment (24x24)
- `align_right.png`: Right alignment (24x24)
- `align_justify.png`: Justify alignment (24x24)

## Icon Design Guidelines

- **Size**: 24x24 pixels for toolbar icons
- **Color**: Use warm yellow/amber (#FFBF00) for active states
- **Style**: Flat, clean icons matching Haiku system style
- **Background**: Transparent background
- **Format**: PNG format

## Creating Toolbar Icons

1. Design icons at 24x24 pixels
2. Use a consistent visual style
3. Ensure icons are recognizable at small sizes
4. Use transparent background
5. Save as PNG format

## Icon States

Toolbar icons may have different states:
- **Normal**: Gray or system color
- **Hover**: Slightly lighter
- **Active**: Yellow accent color
- **Disabled**: Very light gray

## Usage

Icons are loaded and used in the ToolbarView class. Each button can have an associated icon that is displayed along with or instead of text.
