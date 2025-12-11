# Basic Tutorials:

## The Basics of VNS

VNS (Visual Novel Script) is a scripting language used to create interactive visual novels. These scripts are written in plain text files with the `.vns` extension. Imagine crafting a story filled with descriptions, dialogues, choices, and multimedia elements like images and music. VNS uses special tags to transform your script into an immersive experience featuring characters, backgrounds, music, and branching narratives.

## Let's dive into the building blocks:

### 1. Tags:
Tags are the core elements of VNS. They instruct the program on how to display content. Enclosed in square brackets, tags help define scenes, characters, backgrounds, and more.

### 2. Essential tags:

Here are some of the most commonly used tags in VNS:

- **ID:** `[id]string`

  Defines the unique identifier of the script.

- **Language:** `[language]string`

  Specifies the language of the script.

- **Background Image:** `[bgi]string`

  Sets the background image for the current and subsequent dialogues.

- **Background Music:** `[bgm]string`

  Sets the background music for the current and subsequent dialogues.

- **Show Character(s):** `[show]string(s)` 

  Introduces (new) character image(s) to the scene.

### 3. A glimpse into storytelling:

Let's put these tags into action with a simple example.

#### Example Script

```vns
[id]1
[language]English

[bgm]music1.ogg
[bgi]bg1.png
[show]character.png

Alex:
- Hellow world!
```

#### Explanation

- `[id]1`: Sets the script ID to "1".
- `[language]English`: Specifies that the script is in English.
- `[bgm]music1.ogg`: Plays "music1.ogg" as background music.
- `[bgi]bg1.png`: Displays "bg1.png" as the background image.
- `[show]character.png`: Shows "character.png" on the screen.
- `Alex:`: Indicates that the following dialogue is spoken by "Alex".
- `- Hello, world!`: The dialogue line spoken by the character.

#### Expected Output

When compiled, the script should produce a JSON representation like this:

```json
{
    "compiler": {...},
    "dialogues": {
        "head": {
            "background_image": "bg1.png",
            "background_music": "music1.ogg",
            "character_images": [
                "character.png"
            ],
            "contents": [
                "Hellow world!"
            ],
            "narrator": "Alex"
        }
    },
    "id": "1",
    "language": "English"
}
```
This JSON structure represents how the script is interpreted, showing the background, music, characters, and dialogue content.



## Ready to explore more?

This is just the beginning! Check out the documentation for in-depth details. Remember, practice makes perfect. Start simple, experiment, and most importantly, have fun creating your unique stories!



**With dedication and creativity, your journey awaits! Happy storytelling!**
