# Basic Tutorials:

### The Basics of VNS

VNS are written in a plain text file with the `.vns` extension. Imagine a script filled with descriptions, dialogues, and choices. VNS uses special tags to turn this script into an immersive experience, complete with characters, backgrounds, music, and branching narratives.

### **Let's dive into the building blocks:**

**1. The tag party:**

Tags are the secret sauce of VNS. They tell the engine what to display.



**2. Essential tags:**

- **ID:** `[id]string` defines the ID of the script
- **Language:** `[language]string` specifies the language of the script.
- **Section:** `[section]string` specifies the section of all the following dialogues.
- **Background Image:** `[bgi]string` specifies the background image for the current and following dialogues.
- **Background Music:** `[bgm]string` specifies the background music for the current and following dialogues.
- **Show Character(s):** `[show]*string(s)` introduce the character to the current and following dialogues.



**3. A glimpse into storytelling:**

Imagine this simple scene:

```vns
[id]1
[language]English
[section]dialog_example

[bgm]music1.ogg
[bgi]bg1.png
[show]example.png

Example:
- Hellow world!
```

Restult:
```json
{
    "compiler": {...},
    "dialogs": {
        "dialog_example": {
            "head": {
                "background_image": "bg1.png",
                "background_music": "music1.ogg",
                "character_images": [
                    "example.png"
                ],
                "contents": [
                    "Hellow world!"
                ],
                "narrator": "Example"
            }
        }
    },
    "id": "1",
    "language": "English"
}
```
See how tags build a scene, introduce characters?



**Ready to explore more?**

This is just the beginning! Check out the VNS documentation for in-depth details. Remember, practice makes perfect. Start simple, experiment, and most importantly, have fun creating your unique stories!



**With dedication and creativity, your journey awaits! Happy storytelling!**
