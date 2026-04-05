# Notes

## Interface

### Catalog

Back -> Settings
Enter -> Reader

Vertical menu with pictures on the left, and title, length, progress on the right (2 per page)

### Settings

Back -> Catalog
Enter -> Select/deselect item

Vertical menu with single text lines

- Orientation
  - Portrait \*
  - Landscape
- Buttons (Next Previous Enter Back)
  - Portrait (Top down)
    - PEBN \*
    - PBEN
  - Landscape (Left to right)
    - BPNE \*
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

- Changelog
- Power saver mode
- Add debug macro
- Review all uses of `sleep`
- Battery sense https://github.com/elehobica/pico_battery_op/blob/eb301545807bc7fde54abbf009f13d41569a3e73/power_management.cpp#L86
- No magic numbers
- Partial refresh
- https://en.wikipedia.org/wiki/Knuth%E2%80%93Plass_line-breaking_algorithm
- SD module support
- Figure out why fonts have to be extern
- Word wrap types (truncate/none, word boundary, anywhere), indent options, x_end and y_end
- Fix draw_string background color
- Add license to gui
- Use the TinyUSB bundled fatfs
