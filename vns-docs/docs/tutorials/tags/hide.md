# Hide Character(s)

The `[hide]` tag is used to remove character images from the screen, either selectively or all at once, for the current and subsequent dialogues.

### Syntax

```
[hide]image1 image2 ...
```

*image1*, *image2*, etc., represent the filenames of the character images to be hidden.

To hide all characters at once, use:

```
[hide]*
```

### Example

```vns
[hide]character1.png character2.png
```

In this example, the images `character1.png` and `character2.png` will be hidden, while other character images (if any) remain visible.

Alternatively, using:

```vns
[hide]*
```

will hide all currently displayed character images.
