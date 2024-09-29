# Display character(s):

This command is used to display character images for current or upcoming dialogues. Unlike the `[show]` tag, which adds new character images while retaining existing ones, the `[display]` tag first clears any previously shown images before displaying the specified character image(s). This ensures that only the designated character images appear on the screen.

### Syntax

```
[display]image1 image2 ...
```

*image1*, *image2*, etc., represent the filenames of the character images to be displayed.

### Example

```vns
[display]character1.png character2.png
```

In this example, any previously displayed images will be cleared, and the images `character1.png` and `character2.png` will be shown.