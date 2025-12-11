# VEP 5 - Removing [section] Tag
![Static Badge](https://img.shields.io/badge/vns-3.0-blue?style=for-the-badge) ![Static Badge](https://img.shields.io/badge/Status-Final-green?style=for-the-badge "Accepted and implementation complete, or no longer active")

[TOC]

## Abstract
This VEP proposes the removal of the `[section]` tag from VNS 3.0. The `[section]` tag was originally introduced to group dialogues into named sections for organization purposes. However, with the introduction of the `[label]` tag (VEP 3), the `[section]` tag has become redundant. In VNS 3.0, all dialogues are placed directly under the `dialogues` object starting with "head", eliminating the need for explicit section naming.

## Motivation
The `[section]` tag was designed to define named sections that group subsequent dialogues. However, this introduces unnecessary complexity:

1. **Implicit "head"**: Every VNS script starts with "head" as the first dialogue entry by default. The `[section]` tag is redundant because dialogues are always placed starting from "head".

2. **Redundancy with [label]**: For creating named jump targets within a script, the `[label]` tag (introduced in VNS 2.3) provides the same functionality with greater flexibility when combined with `[jump]`.

3. **Simplification**: Removing the `[section]` tag simplifies the language specification and the compiled output structure.

4. **Clarity**: Having two tags with overlapping functionality can cause confusion about when to use each one.

## Specification

### Removed Syntax
The following syntax is removed from VNS 3.0:

```
[section]section_name
```

### Migration Path

#### First Section Tag
The first `[section]` tag in a script should be **removed entirely**. Dialogues are automatically placed starting with "head".

**Before (VNS 2.x):**
```vns
[id]1
[language]English
[section]dialog_example

Alice:
- Hello, world!
```

Compiled to:
```json
{
    "dialogues": {
        "dialog_example": {
            "head": {
                "contents": ["Hello, world!"],
                "narrator": "Alice"
            }
        }
    }
}
```

**After (VNS 3.0):**
```vns
[id]1
[language]English

Alice:
- Hello, world!
```

Compiled to:
```json
{
    "dialogues": {
        "head": {
            "contents": ["Hello, world!"],
            "narrator": "Alice"
        }
    }
}
```

#### Subsequent Section Tags
Any subsequent `[section]` tags should be **replaced with `[label]`** to create named jump targets.

**Before (VNS 2.x):**
```vns
[section]main_story

Alice:
- Let me tell you a story.

[section]alternate_ending

Alice:
- Or maybe not.
```

**After (VNS 3.0):**
```vns
Alice:
- Let me tell you a story.

[label]alternate_ending

Alice:
- Or maybe not.
```

### Compiler Behavior
The VNS 3.0 compiler will emit an error when encountering the `[section]` tag, with a message indicating:
- For the first occurrence: Remove the tag, as dialogues start with "head" automatically
- For subsequent occurrences: Replace with `[label]` instead

## Backward Compatibility
This is a **breaking change**. Scripts written for VNS 2.x that use the `[section]` tag will need to be updated before they can be compiled with VNS 3.0.

The migration is straightforward:
1. Remove the first `[section]` tag entirely
2. Replace any subsequent `[section]` tags with `[label]`

## Rejected Idea(s)

### Deprecation Warning Instead of Removal
An alternative approach considered was to deprecate the `[section]` tag with a warning rather than removing it entirely. This was rejected because:

1. Maintaining deprecated features increases maintenance burden
2. A clean break in a major version (3.0) is the appropriate time for such changes
3. The migration path is simple and mechanical

### Aliasing [section] to [label]
Another approach considered was keeping `[section]` as an alias for `[label]`. This was rejected because:

1. It perpetuates the redundancy that motivated this change
2. It may confuse new users about which tag to use
3. It doesn't address the redundancy of the first section tag
4. Clean removal is preferable in a major version update

## Conclusion
The removal of the `[section]` tag in VNS 3.0 simplifies the language by eliminating redundancy. Dialogues are now placed directly under the `dialogues` object starting with "head", and the `[label]` tag handles all named jump targets. While this is a breaking change, the migration path is straightforward and the long-term benefits of a cleaner language specification outweigh the short-term migration cost.

## VEP Development Process

A live draft for this VEP lives on GitHub. There is also an issue tracker, where much of the technical discussion takes place.
The draft on GitHub is updated regularly in small increments. The official VEPs repo is (usually) only updated when a new draft is posted to the Linpg Foundation.

## Copyright

This document has been placed in the public domain.
