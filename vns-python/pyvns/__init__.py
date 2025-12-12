from .vns_python_wrapper import Compiler, Dialogue, DialoguesManager, Event, Naming

dialogue_t = dict[
    str,
    str
    | list[str]
    | dict[str, str | list[dict[str, str]]]
    | list[dict[str, bool | int | float | str]],
]

dialogues_t = dict[str, dialogue_t]
