# VEP 4 - Introducing [sfx] Tag
![Static Badge](https://img.shields.io/badge/vns-2.4-blue?style=for-the-badge) ![Static Badge](https://img.shields.io/badge/Status-Final-green?style=for-the-badge "Accepted and implementation complete, or no longer active")

[TOC]

## Abstract
This VEP proposes a new tag, `[sfx]`, to enable developers to specify sound effects that should be played during a dialogue. Unlike background music which persists across dialogues, sound effects are one-shot audio cues tied to specific dialogue moments. This enhancement aims to enrich the audio experience in visual novels by allowing precise control over short audio clips.

## Motivation
Visual novels rely heavily on audio to create immersive experiences. While the existing `[bgm]` tag handles background music effectively, there is no standardized way to trigger short sound effects during specific dialogues. Sound effects such as door knocks, footsteps, thunder, or character-specific audio cues are essential for enhancing dramatic moments and player immersion. The introduction of the `[sfx]` tag addresses this gap by providing a dedicated mechanism for triggering one-shot sound effects.

## Rationale
The `[sfx]` tag differs from `[bgm]` in several key ways:

1. **Scope**: Background music persists across dialogues until changed, while sound effects are cleared after each dialogue.
2. **Multiplicity**: Multiple sound effects can be triggered simultaneously for a single dialogue, enabling layered audio experiences.
3. **Intent**: Sound effects are designed for short audio cues, while background music handles ambient and continuous audio.

By providing a separate tag for sound effects, developers gain clearer semantic distinction between different audio types, improving script readability and maintainability.

## Specification

### Syntax
The `[sfx]` tag follows a straightforward syntax:

```
[sfx] <sound_file1> [sound_file2] ...
```

Or using the alternative long form:

```
[sound-effect] <sound_file1> [sound_file2] ...
```

Where `<sound_file1>`, `<sound_file2>`, etc., are the filenames of the sound effect files to be played. Multiple files can be specified, separated by spaces.

### Behavior

1. **Replacement**: Each use of `[sfx]` clears any previously specified sound effects and sets new ones. This is similar to how `[display]` works for character images.
2. **One-shot**: Sound effects are automatically cleared after the dialogue is processed. They do not persist to subsequent dialogues.
3. **Multiple files**: When multiple sound effect files are specified, they should all be triggered for the current dialogue.

### Example Usage

#### Basic Usage
```vns
[sfx]doorbell.wav

Alice:
- Someone's at the door!
```
In this example, the doorbell sound effect plays when Alice's dialogue appears.

#### Multiple Sound Effects
```vns
[sfx]thunder.wav rain.wav

Narrator:
- The storm raged outside the window.
```
Here, both thunder and rain sound effects are triggered simultaneously.

#### Sequential Dialogues
```vns
[sfx]footsteps.wav

Alice:
- Did you hear that?

Bob:
- Hear what?
```
The footsteps sound effect only plays for Alice's dialogue. Bob's dialogue has no sound effects because they are cleared after each dialogue.

#### Combining with Background Music
```vns
[bgm]suspense.mp3
[sfx]creaking_door.wav

Alice:
- This house gives me the creeps...
```
The background music continues playing while the sound effect is triggered for this specific dialogue.

## Backward Compatibility
The introduction of the `[sfx]` tag will not affect existing scripts as it introduces new functionality without altering the behavior of current tags. Scripts that do not use the `[sfx]` tag will continue to work as expected.

## Rejected Idea(s)

### Reusing [bgm] Tag with Parameters
An alternative approach considered was extending the `[bgm]` tag to accept a parameter distinguishing between background music and sound effects. This was rejected because:

1. It conflates two semantically different concepts
2. It would complicate the behavior of an existing, well-understood tag
3. It would not naturally support the one-shot clearing behavior needed for sound effects

### Persistent Sound Effects
Another approach considered was having sound effects persist like background music until explicitly changed. This was rejected because:

1. Sound effects are typically short and meant for specific moments
2. Accidental persistence could lead to unwanted repeated audio
3. The one-shot behavior aligns better with common visual novel conventions

## Conclusion
The `[sfx]` tag provides a standardized mechanism for triggering sound effects in VNS scripts. By enabling precise control over one-shot audio cues, developers can create more immersive and dynamic audio experiences. This enhancement integrates seamlessly with existing audio tags while maintaining full backwards compatibility.

## VEP Development Process

A live draft for this VEP lives on GitHub. There is also an issue tracker, where much of the technical discussion takes place.
The draft on GitHub is updated regularly in small increments. The official VEPs repo is (usually) only updated when a new draft is posted to the Linpg Foundation.

## Copyright

This document has been placed in the public domain.
