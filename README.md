# Visual Novel Script

Visual Novel Script, short for VNS, is a universal scripting language created with the goal of creating a language that allows visual novel developers to write once and compile everywhere, across different platforms, game libraries, and programming languages.

Unlike Ren'py, the visual novel script is more like writing a story instead of writing a program. The developer also have the choice to make their own implementation.



# Tutorials:

## Tags with value

`[tag]value`

[tag] represent what kind of value you are trying to represent, and the value is, well, the value.

### ID:

`[id]number`, ex: `[id]1`

**[id]** represent the id of this dialogue. Every script file requires to set a id.

### Language:

`[language]str`, ex: `[language]English`

**[language]** represent the id of this dialogue. Every script file requires to set a language.

### Section:

`[section]str`, ex: `[section]dialog_example`

**[section]** represent the section of the following dialogues.

### Background image:

`[bgi]str`, ex: `[bgi]bg1.png`

**[bgi]** represent the background image of the current and following dialogues.

### Background music:

`[bgm]str`, ex: `[bgm]bgm1.ogg`

**[bgm]** represent the background music of the current and following dialogues.

### Display characters

`[display]*str`, ex: `[display]character1.png character2.png`

display the character(s) of the current and following dialogues.

### Hide characters

`[hide]*str`, ex: `[hide]character1.png character2.png` or `[hide]*` for hiding all.

Hide/remove the character(s) from the current and following dialogues.

### New scene

`[scene]str`, ex: `[scene]bg2.png`

Enter a new scene, and display specified background image.

## Tags without value

### End

`[end]`

Mark the end of the dialogue. Although the compiler will stop compiling immediately after reaching the end, it is still recommended to add this tag to the end of your script file.

### **Block**

`[block]`

Player cannot go back to previous dialogue.

### Comments:

`# this is a comment`

Adding comments to the script to make it easier to understand.

### Notes:

`// this is a note`

Notes are very similar to comments. However, it is usually used to add note to a specific dialogue, and will be saved to the compiled file.

## Dialogues:

Dialogues are usually written in the form of this:

`Mabel:`

`- Hello my name is Mabel!`

`- How are you doing today!`

In this case, we have a character name call Mabel, and what she will say is usually start with a hyphen and a space, and follow by a sentence, which she wants to say. 
