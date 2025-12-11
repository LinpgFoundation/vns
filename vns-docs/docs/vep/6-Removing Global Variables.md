# VEP 6 - Removing Global Variables
![Static Badge](https://img.shields.io/badge/vns-3.0-blue?style=for-the-badge) ![Static Badge](https://img.shields.io/badge/Status-Final-green?style=for-the-badge "Accepted and implementation complete, or no longer active")

[TOC]

## Abstract
This VEP proposes the removal of global variables (variables prefixed with `@`) from VNS 3.0. With the removal of the `[section]` tag (VEP 5), the distinction between local and global variables becomes unnecessary. This simplification reduces complexity in the language specification and implementation.

## Motivation
Global variables were introduced in VEP 1 to allow data sharing across sections within a dialogue. However, with the removal of the `[section]` tag in VNS 3.0:

1. **Redundancy**: The primary use case for global variables was sharing data between sections. Without sections, this distinction is no longer meaningful.

2. **Simplification**: Reducing the number of variable scopes simplifies the language, making it easier to learn and use.

3. **Consistency**: With only local and persistent variables remaining, the variable system becomes more straightforward:
   - Local variables: Scoped to the current dialogue
   - Persistent variables: Persist across sessions

## Specification

### Removed Syntax
The following syntax is removed from VNS 3.0:

```
@variable_name = value
```

Variables starting with `@` will no longer be recognized as valid variable declarations.

### Migration Path
Scripts using global variables should migrate to using either local or persistent variables:

**Before (VNS 2.x):**
```vns
@shared_counter = 0
@shared_counter ++
```

**After (VNS 3.0):**

For dialogue-scoped data (most common case):
```vns
shared_counter = 0
shared_counter ++
```

For data that must persist across sessions:
```vns
&shared_counter = 0
&shared_counter ++
```

### Compiler Behavior
The VNS 3.0 compiler will emit an error when encountering variables prefixed with `@`, with a message suggesting the use of local variables (no prefix) or persistent variables (`&` prefix) instead.

## Backward Compatibility
This is a **breaking change**. Scripts written for VNS 2.x that use global variables (prefixed with `@`) will need to be updated before they can be compiled with VNS 3.0.

The migration is straightforward:
- Replace `@variable` with `variable` for dialogue-scoped data
- Replace `@variable` with `&variable` for data requiring persistence

## Rejected Idea(s)

### Keeping @ as an Alias for Local Variables
An alternative approach considered was keeping `@` as an alias for local variables. This was rejected because:

1. It perpetuates unnecessary syntax complexity
2. It may confuse new users about the difference between `@var` and `var`
3. Clean removal is preferable in a major version update

### Deprecation Warning Instead of Removal
Another approach considered was to deprecate global variables with a warning. This was rejected because:

1. The distinction between local and global is no longer meaningful without sections
2. A major version (3.0) is the appropriate time for breaking changes
3. The migration path is simple

## Conclusion
The removal of global variables in VNS 3.0 simplifies the variable system by eliminating a scope that is no longer needed after the removal of sections. This change aligns with VNS 3.0's goal of streamlining the language specification. While this is a breaking change, the migration is straightforward and the resulting simplicity benefits all users.

## VEP Development Process

A live draft for this VEP lives on GitHub. There is also an issue tracker, where much of the technical discussion takes place.
The draft on GitHub is updated regularly in small increments. The official VEPs repo is (usually) only updated when a new draft is posted to the Linpg Foundation.

## Copyright

This document has been placed in the public domain.
