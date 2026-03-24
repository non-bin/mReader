# mReader

## Flow

### Catalog
Back -> Settings
Enter -> Reader

Vertical menu with pictures on the left, and title, length, progress on the right (2 per page)

### Settings
Back -> Catalog
Enter -> Select/deselect item

Vertical menu with single text lines
- Orientation
  - Portrait *
  - Landscape
- Buttons (Next Previous Enter Back)
  - Portrait (Top down)
    - PEBN *
    - PBEN
  - Landscape (Left to right)
    - BPNE *
    - PBEN
- Theme (Light/dark)
- Text size (px)
- Scroll size (Number of lines. 1 > screenHeight/textSize)
- Status bar size (Disabled/px) (display bar while hovering settings)
- Status bar theme (Light/dark)
- Software update -> BOOTSEL

### Reader
Back -> Catalog
Enter -> Chapters

- Status bar
  - Title, length, progress

### Chapters
Back -> Reader
Enter -> Reader at selected chapter

Vertical menu with single text lines
Chapter names if available, or h1-h3, or text after 1/2/3 line breaks (don't trigger again until character threshold)

## TODO

- Review all uses of `sleep`
