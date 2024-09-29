# Label

The `[label]` tag is used to create a reference point within the dialogue script. This label can be targeted by branching commands, allowing the flow of the conversation to jump to this specific point.

### Syntax

```
[label]label_name
```

*label_name* represents the unique identifier for this point in the dialogue.

### Example

```vns
[label]jump_point1
```

In this example, `jump_point1` serves as a reference label that can be used later to direct the conversation flow to this specific part of the script.
