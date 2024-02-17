"""
Python bindings for the VNS C++ library
"""

from __future__ import annotations

import typing

__all__ = ["Compiler", "Dialogue", "DialogueNext", "DialoguesManager", "Naming"]

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
                str, dict[str, str | list[str] | dict[str, str | list[dict[str, str]]]]
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
    comments: list[str]
    contents: list[str]
    id: str
    narrator: str
    next: DialogueNext
    previous: str
    def __init__(
        self,
        arg0: dict[str, str | list[str] | dict[str, str | list[dict[str, str]]]],
        arg1: str,
    ) -> None: ...
    def has_next(self) -> bool:
        """
        Check if the dialogue has next
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

    def to_map(
        self,
    ) -> dict[str, str | list[str] | dict[str, str | list[dict[str, str]]]]:
        """
        Convert the dialogue object to a map
        """

class DialogueNext:
    """
    Class for representing the dialogue next
    """

    @typing.overload
    def __init__(self, arg0: str, arg1: str | list[dict[str, str]]) -> None: ...
    @typing.overload
    def __init__(self, arg0: dict[str, str | list[dict[str, str]]]) -> None:
        """
        Initialize a ContentNext object from a map
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

    def is_null(self) -> bool:
        """
        Check if the next is null
        """

    def to_map(self) -> dict[str, str | list[dict[str, str]]]:
        """
        Convert the next object to a map
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

    def empty(self) -> bool:
        """
        Check if data is empty
        """

    def get_current(self) -> Dialogue:
        """
        Get current dialogue
        """

    def get_current_dialogue(
        self,
    ) -> dict[str, str | list[str] | dict[str, str | list[dict[str, str]]]]:
        """
        Get current dialogue data
        """

    def get_current_section_dialogues(
        self,
    ) -> dict[str, dict[str, str | list[str] | dict[str, str | list[dict[str, str]]]]]:
        """
        Get current section dialogue contents
        """

    def get_data(
        self,
    ) -> dict[
        str,
        dict[str, dict[str, str | list[str] | dict[str, str | list[dict[str, str]]]]],
    ]:
        """
        Get data
        """

    def get_dialogue(
        self, arg0: str, arg1: str
    ) -> dict[str, str | list[str] | dict[str, str | list[dict[str, str]]]]:
        """
        Get dialogue data by ID
        """

    def get_id(self) -> str:
        """
        Get current dialogue ID
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

    def get_section_dialogues(
        self, arg0: str
    ) -> dict[str, dict[str, str | list[str] | dict[str, str | list[dict[str, str]]]]]:
        """
        Get section dialogue contents by section name
        """

    def get_sections(self) -> set[str]:
        """
        Get the names of all sections
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

    def save(self) -> None:
        """
        Save modifications to the current dialogue interface
        """

    def set_current_dialogue(
        self, arg0: dict[str, str | list[str] | dict[str, str | list[dict[str, str]]]]
    ) -> None:
        """
        Set current dialogue data
        """

    def set_current_section_dialogues(
        self,
        arg0: dict[
            str, dict[str, str | list[str] | dict[str, str | list[dict[str, str]]]]
        ],
    ) -> None:
        """
        Set current section dialogue contents
        """

    def set_data(
        self,
        arg0: dict[
            str,
            dict[
                str, dict[str, str | list[str] | dict[str, str | list[dict[str, str]]]]
            ],
        ],
    ) -> None:
        """
        Update data
        """

    def set_dialogue(
        self,
        arg0: str,
        arg1: str,
        arg2: dict[str, str | list[str] | dict[str, str | list[dict[str, str]]]],
    ) -> None:
        """
        Set current dialogue data by ID
        """

    def set_id(self, arg0: str) -> None:
        """
        Set current dialogue ID
        """

    def set_section(self, arg0: str) -> None:
        """
        Set current section name
        """

    def set_section_dialogues(
        self,
        arg0: str,
        arg1: dict[
            str, dict[str, str | list[str] | dict[str, str | list[dict[str, str]]]]
        ],
    ) -> None:
        """
        Set section dialogue contents by section name
        """

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
        Update the character naming database from a map of names and sets of tags
        """

    @staticmethod
    @typing.overload
    def update_database(arg0: dict[str, list[str]]) -> None:
        """
        Update the character naming database from a map of names and vectors of tags
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
