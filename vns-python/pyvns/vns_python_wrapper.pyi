"""
Python bindings for the VNS C++ library
"""

from __future__ import annotations

import typing

__all__ = ["Compiler", "Content", "ContentManager", "ContentNext", "Naming"]

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

class Content:
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
    next: ContentNext
    previous: str
    def __init__(
        self,
        arg0: dict[str, str | list[str] | dict[str, str | list[dict[str, str]]]],
        arg1: str,
    ) -> None: ...
    def has_next(self) -> bool:
        """
        Check if the content has next
        """

    def to_map(
        self,
    ) -> dict[str, str | list[str] | dict[str, str | list[dict[str, str]]]]:
        """
        Convert the content object to a map
        """

class ContentManager:
    """
    Class for managing dialogue content
    """

    def __init__(self) -> None: ...
    def clear(self) -> None:
        """
        Clear data
        """

    def contains_content(self, arg0: str, arg1: str) -> bool:
        """
        Check if dialogue with given section contain given ID
        """

    def contains_section(self, arg0: str) -> bool:
        """
        Check if dialogue has given section name
        """

    def empty(self) -> bool:
        """
        Check if data is empty
        """

    def get_content(
        self, arg0: str, arg1: str
    ) -> dict[str, str | list[str] | dict[str, str | list[dict[str, str]]]]:
        """
        Get content data by ID
        """

    def get_current(self) -> Content:
        """
        Get current content
        """

    def get_current_content(
        self,
    ) -> dict[str, str | list[str] | dict[str, str | list[dict[str, str]]]]:
        """
        Get current content data
        """

    def get_current_section_contents(
        self,
    ) -> dict[str, dict[str, str | list[str] | dict[str, str | list[dict[str, str]]]]]:
        """
        Get current section content
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

    def get_id(self) -> str:
        """
        Get current dialogue ID
        """

    def get_last(self) -> Content:
        """
        Get last content
        """

    def get_previous(self) -> Content:
        """
        Get previous content
        """

    def get_section(self) -> str:
        """
        Get current section name
        """

    def get_section_contents(
        self, arg0: str
    ) -> dict[str, dict[str, str | list[str] | dict[str, str | list[dict[str, str]]]]]:
        """
        Get section content by name
        """

    def remove_content(self, arg0: str, arg1: str) -> None:
        """
        Remove content data by ID
        """

    def remove_current_content(self) -> None:
        """
        Remove current content data
        """

    def remove_section(self, arg0: str) -> None:
        """
        Remove section
        """

    def save(self) -> None:
        """
        Save modifications to the current dialog interface
        """

    def set_content(
        self,
        arg0: str,
        arg1: str,
        arg2: dict[str, str | list[str] | dict[str, str | list[dict[str, str]]]],
    ) -> None:
        """
        Set content data by ID
        """

    def set_current_content(
        self, arg0: dict[str, str | list[str] | dict[str, str | list[dict[str, str]]]]
    ) -> None:
        """
        Set current content data
        """

    def set_current_section_contents(
        self,
        arg0: dict[
            str, dict[str, str | list[str] | dict[str, str | list[dict[str, str]]]]
        ],
    ) -> None:
        """
        Set current section content
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

    def set_id(self, arg0: str) -> None:
        """
        Update current dialogue ID
        """

    def set_section(self, arg0: str) -> None:
        """
        Set current section name
        """

    def set_section_contents(
        self,
        arg0: str,
        arg1: dict[
            str, dict[str, str | list[str] | dict[str, str | list[dict[str, str]]]]
        ],
    ) -> None:
        """
        Set section content by name
        """

class ContentNext:
    """
    Class for representing the next content in dialogue
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
        Get the target of the next content
        """

    def get_targets(self) -> list[dict[str, str]]:
        """
        Get the targets of the next content
        """

    def get_type(self) -> str:
        """
        Get the type of the next content
        """

    def has_multi_targets(self) -> bool:
        """
        Check if the next content has multiple targets
        """

    def has_single_target(self) -> bool:
        """
        Check if the next content has a single target
        """

    def is_null(self) -> bool:
        """
        Check if the next content is null
        """

    def to_map(self) -> dict[str, str | list[dict[str, str]]]:
        """
        Convert the next object to a map
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
