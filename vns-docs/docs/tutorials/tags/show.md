# Show Character(s)

The `[show]` tag is used to display one or more character images on the screen, corresponding to the ongoing and upcoming dialogues. Unlike the `[display]` tag, this does not clear existing images but adds the specified character images alongside any already present.

### Syntax

```
[show]character_image1 character_image2 ...
```

*character_image1*, *character_image2*, etc., represent the filenames of the character images to be shown.

### Example

```vns
[show]character1.png character2.png
```

In this example, the character images `character1.png` and `character2.png` are introduced, without removing any previously displayed characters.
