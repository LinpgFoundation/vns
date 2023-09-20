# Visual Novel Script

Visual Novel Script, short for VNS, is a universal scripting language created with the goal of creating a language that allows visual novel developers to write once and compile everywhere, across different platforms, game libraries, and programming languages.

VNS provides a number of features that make it easy to create visual novels, such as:

- A simple and easy-to-learn syntax
- Support for branching dialogue and choices
- Support for music and sound effects

Unlike Ren'py, the visual novel script is more like writing a story instead of a program, and the developers also have the choice to create their own implementation.

# Tutorials:

VNS are written in a plain text file with the `. vns` extension. The script file contains a series of tags, which are used to specify the different elements of the visual novel, such as the characters, backgrounds, dialogue, music, and so on.

## Tags with value

`[tag]value`

The `[tag]` represents the kind of value you are trying to represent, and the value is, well, the value. 

### ID:

`[id]number`, ex: `[id]1`

Specifies the ID of the current dialogue file. Every script file must have an ID.

### Language:

`[language]str`, ex: `[language]English`

Specifies the language of the current dialogue. Every script file must specifies a language.

### Section:

`[section]str`, ex: `[section]dialog_example`

Specifies the section of all the following dialogues.

### Background image:

`[bgi]str`, ex: `[bgi]bg1.png`

Specifies the background image for the current and following dialogues.

### Background music:

`[bgm]str`, ex: `[bgm]bgm1.ogg`

Specifies the background music for the current and following dialogues.

### Display characters

`[display]*str`, ex: `[display]character1.png character2.png`

Displays the character(s) for the current and following dialogues.

### Hide characters

`[hide]*str`, ex: `[hide]character1.png character2.png` or `[hide]*` for hiding all.

Hides the character(s) for the current and following dialogues.

### New scene

`[scene]str`, ex: `[scene]bg2.png`

Enters a new scene and displays the specified background image.

### Label

`[label]str`, ex: `[label]jump_point1`

Creates a label for the branch command.

### Entry

`[entry]str`, ex: `[label]ifPickUpItem`

Utilizes the value as the key for the subsequent conversation.

## Tags without value

### End

`[end]`

Marks the end of the dialogue. Although the compiler will stop compiling immediately after reaching the end, it is still recommended that you add this tag to the end of your script file.

### **Block**

`[block]`

The player can't go back to the previous dialogue.

### Comments:

`# this is a comment`

Commenting the script to make it easier to understand.

### Notes:

`// this is a note`

Notes are very similar to comments. But usually, it's used to make a note for a specific conversation and is saved in the file that was made.

## Dialogues:

Dialogues are written in the form of this:

```vns
Character name:
- Dialogue text
- More dialogue text
```

The character name is followed by a colon and then one or more lines of dialogue text. Each line of dialogue text must start with a hyphen and a space.

## **Example**:

Here is a simple example of a VNS script:

```vns
[id]1
[language]English
[section]dialog_example

[bgi]bg1.png
[display]character1.png character2.png

Mabel:
- Hello my name is Mabel!
- How are you doing today!

[hide]character1.png

Dipper:
- Hi Mabel! I'm doing well, thanks for asking.

[end]
```

This script would display the background image `bg1.png` and the character images `character1.png` and `character2.png`. Mabel would then say "Hello my name is Mabel!" and "How are you doing today!". Next, `character1.png` would be hidden and Dipper would say "Hi Mabel! I'm doing well, thanks for asking." Finally, the script would end.
