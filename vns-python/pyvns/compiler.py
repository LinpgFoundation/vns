import json
from glob import glob
from os import path as OS_PATH
from typing import Any

from .vns_python_wrapper import Compiler as _Compiler


class Compiler:
    # load file directly
    @staticmethod
    def load(path: str) -> dict[str, Any]:
        return _Compiler.load(path)

    # compile
    @classmethod
    def compile(cls, path: str, out_dir: str | None = None) -> None:
        if not OS_PATH.isdir(path) and path.endswith(".vns"):
            cls._save(
                cls.load(path),
                out_dir if out_dir is not None else OS_PATH.dirname(path),
            )
        else:
            for _file in glob(OS_PATH.join(path, "*")):
                cls.compile(_file, out_dir)

    # save (can be overridden by child classes)
    @staticmethod
    def _save(_data: dict[str, Any], _dir: str) -> None:
        with open(
            OS_PATH.join(
                _dir, f"chapter{_data['id']}_dialogs_{_data['language']}.json"
            ),
            "w",
            encoding="utf-8",
        ) as f:
            json.dump(_data, f, indent=4, ensure_ascii=False, sort_keys=True)
