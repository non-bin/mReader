# mReader

Tiny e-paper e-reader

## Parts

Essentials:
- [Waveshare 1.54inch V2 e-Paper with Module](https://www.waveshare.com/wiki/1.54inch_e-Paper_Module_Manual)
- Any RP2040 based board, I use the [Waveshare RP2040-Zero](https://www.waveshare.com/wiki/RP2040-Zero)
- 4 buttons
- Battery and charger board

Wiring can be inferred from [config.h](./src/config.h)

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

- Changelog
- Review all uses of `sleep`
- Battery sense https://github.com/elehobica/pico_battery_op/blob/eb301545807bc7fde54abbf009f13d41569a3e73/power_management.cpp#L86
- https://en.wikipedia.org/wiki/Knuth%E2%80%93Plass_line-breaking_algorithm
- No magic numbers
- SD module support
