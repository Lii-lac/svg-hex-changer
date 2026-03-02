## About
A fun little C program that changes a hex color within a `.SVG` so I could save my friends some time!

## Usage
Compile with your favorite compiler and then get to hex changing!

Right now, the program only supports changing one file at a time, in the form:
```bash
./svg-hex-color-changer.out <imageName> <toColor>
```
or
```bash
./svg-hex-color-changer.out <imageName> <fromColor> <toColor>
```
where the former changes any instance of the first hex color it reads to toColor, and the latter changes any instance of fromColor to toColor.
- `imageName` must be the name of the image, in the form `imageName.svg` (include the `.svg`).
- `toColor` must be a hex color, in the form `FFFFFF` (no `#`).
- `fromColor` must be a hex color, in the form `FFFFFF` (no `#`).

## Soon-to-come
This supposed to be a very simple personal project while I learn how to make my github face publicly. That said, right now I still plan to implement:
- Mass-altering of images by passing a directory as the first arg `imageName`, and applying `toColor` to each of those images.