# VEP 3 - Introducing [jump] Tag
![Static Badge](https://img.shields.io/badge/vns-2.3-blue?style=for-the-badge) ![Static Badge](https://img.shields.io/badge/Status-Final-green?style=for-the-badge "Accepted and implementation complete, or no longer active")

[TOC]

## Abstract
This VEP proposes a new tag for branching dialogues, [jump], to enable developers to jump from the current dialog to another specified dialog within a scenario. This enhancement aims to facilitate more complex and nested dialog structures, offering enhanced narrative flexibility when combined with existing tags such as [end], [block], and labels.

## Motivation
In the development of Visual Novel Scripts (VNS), complex storytelling often requires dynamic transitions between dialogues that are not linear. The existing tags support basic flow control but lack the ability to efficiently implement more intricate dialogue pathways that can enhance player engagement and narrative depth. The introduction of the [jump] tag addresses this gap by allowing explicit and clear transitions, improving both the writing experience and game-play mechanics.

## Rationale
The [jump] tag offers explicit control over dialogue flow, crucial for developing non-linear narratives and complex interaction patterns in visual novels. By providing a clear, standardized method for script branching, developers can craft more engaging and intricate stories with greater ease.

## Specification
### Syntax
The [jump] tag will follow a straightforward syntax:
`[jump]/[jmp] <label>`
Where <label> is the identifier of the labeled point to which the flow should be redirected. This label must be predefined in the script using the `[label]` tag.

### Example Usage
Consider the following script snippet:
```
Alex:
- this is not true

[jump]alice_answer

Alex:
- or is it?

[end]

[label]alice_answer
Alice:
- why not?
```
In this scenario, once Alex says, "this is not true," the dialogue will immediately continue with Alice's response, "why not?" The line "or is it?" will not be reached unless accessed through a different [jump] command. The dialogue sequence will terminate at Alex's "or is it?" if reached without a preceding [jump] due to the [end] tag.
### Behavior with Other Tags
- [end]: Closes the current dialogue branch. If encountered after a [jump], the sequence does not continue past the [end].
- [block]: Prevents returning to a previous dialogue point once passed, ensuring linear progression post-block unless redirected by another [jump].

## Rejected Idea(s)
### Reusing [br] Tag
Initially considered, the [br] tag was deprecated in VNS 2.0 due to its limited utility and potential confusion with the new [jump] functionality. Its reintroduction could mislead developers about supported features in the current VNS standard.

## Backward Compatibility
The introduction of the [jump] tag will not affect existing scripts as it introduces new functionality without altering the behavior of current tags.

## Conclusion
The [jump] tag provides a standardized mechanism for non-linear dialogue flow in VNS scripts. By enabling explicit transitions between labeled points, developers gain greater control over narrative structure, allowing for more complex and engaging storytelling. This enhancement integrates seamlessly with existing tags while maintaining full backwards compatibility.

## VEP Development Process

A live draft for this VEP lives on GitHub. There is also an issue tracker, where much of the technical discussion takes place.
The draft on GitHub is updated regularly in small increments. The official VEPs repo is (usually) only updated when a new draft is posted to the Linpg Foundation.

## Copyright

This document has been placed in the public domain.