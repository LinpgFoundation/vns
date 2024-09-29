# Option(s)

The `[option]` tag adds interactive choices to the current dialogue, allowing players to select an option and branch the story to a specific label based on their choice.

### Syntax

```
[option]option_text->label_name
```

- *option_text* is the message displayed as a choice for the player.
- *label_name* is the target label the script will jump to after the option is selected.

### Example

```vns
[option]Can you hear me?->yes_reply
```

In this example, the option "Can you hear me?" is presented to the player. If chosen, the story will branch to the label `yes_reply`.
