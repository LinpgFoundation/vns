"""
Python bindings for the VNS C++ library
"""

from __future__ import annotations

import typing

__all__ = [
    "Compiler",
    "Dialogue",
    "DialogueNext",
    "DialoguesManager",
    "Event",
    "Naming",
]

class Compiler:
    """
    Compiler for compiling vns files
    """

    @staticmethod
    def load(
        arg0: str,
    ) -> dict[
        str,
        dict[
            str,
            dict[
                str,
                dict[
                    str,
                    str
                    | list[str]
                    | dict[str, str | list[dict[str, str]]]
                    | list[dict[str, bool | int | float | str]],
                ],
            ],
        ]
        | dict[str, int]
        | str,
    ]:
        """
        Load a vns file
        """

class Dialogue:
    """
    Class representing dialogue content
    """

    background_image: str
    background_music: str
    character_images: list[str]
    contents: list[str]
    id: str
    narrator: str
    next: DialogueNext
    notes: list[str]
    previous: str
    def __init__(
        self,
        arg0: dict[
            str,
            str
            | list[str]
            | dict[str, str | list[dict[str, str]]]
            | list[dict[str, bool | int | float | str]],
        ],
        arg1: str,
    ) -> None: ...
    def has_next(self) -> bool:
        """
        Check if the dialogue has next
        """

    def remove_next(self) -> None:
        """
        Remove dialogue next
        """

    @typing.overload
    def set_next(self, arg0: str, arg1: str | list[dict[str, str]]) -> None:
        """
        Set dialogue next
        """

    @typing.overload
    def set_next(self, arg0: dict[str, str | list[dict[str, str]]]) -> None:
        """
        Set dialogue next
        """

    def to_dict(
        self,
    ) -> dict[
        str,
        str
        | list[str]
        | dict[str, str | list[dict[str, str]]]
        | list[dict[str, bool | int | float | str]],
    ]:
        """
        Convert the dialogue object to a dict
        """

    @property
    def events(self) -> list[Event]: ...

class DialogueNext:
    """
    Class for representing the dialogue next
    """

    @typing.overload
    def __init__(self, arg0: str, arg1: str | list[dict[str, str]]) -> None: ...
    @typing.overload
    def __init__(self, arg0: dict[str, str | list[dict[str, str]]]) -> None:
        """
        Initialize a ContentNext object from a dict
        """

    def get_target(self) -> str:
        """
        Get the target of the next
        """

    def get_targets(self) -> list[dict[str, str]]:
        """
        Get the targets of the next
        """

    def get_type(self) -> str:
        """
        Get the type of the next
        """

    def has_multi_targets(self) -> bool:
        """
        Check if the next has multiple targets
        """

    def has_single_target(self) -> bool:
        """
        Check if the next has a single target
        """

    def has_type(self, arg0: str) -> bool:
        """
        Whether the next has given type
        """

    def is_null(self) -> bool:
        """
        Check if the next is null
        """

    def to_dict(self) -> dict[str, str | list[dict[str, str]]]:
        """
        Convert the next object to a dict
        """

class DialoguesManager:
    """
    Class for managing dialogue contents
    """

    def __init__(self) -> None: ...
    def clear(self) -> None:
        """
        Clear data
        """

    def contains_dialogue(self, arg0: str, arg1: str) -> bool:
        """
        Check if section contains given dialogue ID
        """

    def contains_section(self, arg0: str) -> bool:
        """
        Check if dialogues have given section name
        """

    def contains_variable(self, arg0: str) -> bool:
        """
        Contains variable
        """

    def empty(self) -> bool:
        """
        Check if data is empty
        """

    def get_bool_variable(self, arg0: str) -> bool:
        """
        Get variable as bool
        """

    def get_current(self) -> Dialogue:
        """
        Get current dialogue
        """

    def get_current_dialogue_id(self) -> str:
        """
        Get current dialogue ID
        """

    def get_current_section_dialogues(self) -> dict[str, Dialogue]:
        """
        Get current section dialogue contents
        """

    def get_dialogue(self, arg0: str, arg1: str) -> Dialogue:
        """
        Get dialogue data by ID
        """

    def get_dialogues(self, arg0: str) -> dict[str, Dialogue]:
        """
        Get current dialogue data
        """

    def get_float_variable(self, arg0: str) -> float:
        """
        Get variable as float
        """

    def get_int_variable(self, arg0: str) -> int:
        """
        Get variable as int
        """

    def get_last(self) -> Dialogue:
        """
        Get last dialogue
        """

    def get_previous(self) -> Dialogue:
        """
        Get previous dialogue
        """

    def get_section(self) -> str:
        """
        Get current section name
        """

    def get_sections(self) -> set[str]:
        """
        Get the names of all sections
        """

    def get_str_variable(self, arg0: str) -> str:
        """
        Get variable as str
        """

    def get_variable(self, arg0: str) -> bool | int | float | str:
        """
        Get variable
        """

    def load(self, arg0: str) -> None:
        """
        load dialogue data from vns file
        """

    def next(self) -> None:
        """
        Go to next dialogue
        """

    def remove_current_dialogue(self) -> None:
        """
        Remove current dialogue
        """

    def remove_dialogue(self, arg0: str, arg1: str) -> None:
        """
        Remove dialogue by ID
        """

    def remove_section(self, arg0: str) -> None:
        """
        Remove section
        """

    def set_current_dialogue(
        self,
        arg0: dict[
            str,
            str
            | list[str]
            | dict[str, str | list[dict[str, str]]]
            | list[dict[str, bool | int | float | str]],
        ],
    ) -> None:
        """
        Set current dialogue data
        """

    def set_current_dialogue_id(self, arg0: str) -> None:
        """
        Set current dialogue ID
        """

    def set_current_section_dialogues(
        self,
        arg0: dict[
            str,
            dict[
                str,
                str
                | list[str]
                | dict[str, str | list[dict[str, str]]]
                | list[dict[str, bool | int | float | str]],
            ],
        ],
    ) -> None:
        """
        Set current section dialogue contents
        """

    def set_dialogue(
        self,
        arg0: str,
        arg1: str,
        arg2: dict[
            str,
            str
            | list[str]
            | dict[str, str | list[dict[str, str]]]
            | list[dict[str, bool | int | float | str]],
        ],
    ) -> None:
        """
        Set current dialogue data by ID
        """

    def set_dialogues(
        self,
        arg0: str,
        arg1: dict[
            str,
            dict[
                str,
                str
                | list[str]
                | dict[str, str | list[dict[str, str]]]
                | list[dict[str, bool | int | float | str]],
            ],
        ],
    ) -> None:
        """
        Set section dialogue contents by section name
        """

    def set_section(self, arg0: str) -> None:
        """
        Set current section name
        """

    def set_variable(self, arg0: str, arg1: bool | int | float | str) -> None:
        """
        Set variable
        """

    def to_dict(
        self,
    ) -> dict[
        str,
        dict[
            str,
            dict[
                str,
                str
                | list[str]
                | dict[str, str | list[dict[str, str]]]
                | list[dict[str, bool | int | float | str]],
            ],
        ],
    ]:
        """
        Get data in dict
        """

    def update(
        self,
        arg0: dict[
            str,
            dict[
                str,
                dict[
                    str,
                    str
                    | list[str]
                    | dict[str, str | list[dict[str, str]]]
                    | list[dict[str, bool | int | float | str]],
                ],
            ],
        ],
    ) -> None:
        """
        Update data
        """

class Event:
    """
    Class for representing the event struct
    """

    @typing.overload
    def __init__(
        self, arg0: str, arg1: str, arg2: bool | int | float | str
    ) -> None: ...
    @typing.overload
    def __init__(self, arg0: dict[str, bool | int | float | str]) -> None: ...
    def to_dict(self) -> dict[str, bool | int | float | str]: ...
    @property
    def target(self) -> str: ...
    @property
    def type(self) -> str: ...

class Naming:
    """
    Character name preprocessing module
    """

    @staticmethod
    def clear_database() -> None:
        """
        Clear the character naming database
        """

    @staticmethod
    def get_database() -> dict[str, set[str]]:
        """
        Get the character name database
        """

    @staticmethod
    @typing.overload
    def update_database(arg0: str) -> None:
        """
        Update the character naming database from a JSON string
        """

    @staticmethod
    @typing.overload
    def update_database(arg0: dict[str, set[str]]) -> None:
        """
        Update the character naming database from a dict of names and sets of tags
        """

    @staticmethod
    @typing.overload
    def update_database(arg0: dict[str, list[str]]) -> None:
        """
        Update the character naming database from a dict of names and vectors of tags
        """

    def __init__(self, arg0: str) -> None: ...
    def contains_tag(self, arg0: str) -> bool:
        """
        Check if the name contains a tag
        """

    @typing.overload
    def equal(self, arg0: str) -> bool:
        """
        Check if the name is equal to a given naming string
        """

    @typing.overload
    def equal(self, arg0: str, arg1: bool) -> bool:
        """
        Check if the name is equal to a given naming string, with an option to force absolute equality
        """

    @typing.overload
    def equal(self, arg0: Naming) -> bool:
        """
        Check if the name is equal to another Naming object
        """

    @typing.overload
    def equal(self, arg0: Naming, arg1: bool) -> bool:
        """
        Check if the name is equal to another Naming object, with an option to force absolute equality
        """

    def erase_tag(self, arg0: str) -> None:
        """
        Erase a tag from the name
        """

    def get_name(self) -> str:
        """
        Get the name
        """

    def get_tags(self) -> set[str]:
        """
        Get the tags associated with the name
        """

    def insert_tag(self, arg0: str) -> None:
        """
        Insert a tag to the name
        """

    def to_string(self) -> str:
        """
        Retrieve naming as string
        """
