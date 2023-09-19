import json
import time
from glob import glob
from os import path as OS_PATH
from typing import Any

from ._processor import Processor


class Compiler:
    # 直接加载
    @staticmethod
    def load(path: str) -> dict[str, Any]:
        _processor: Processor = Processor()
        _processor.process(path)
        return {
            "dialogs": _processor.get_output(),
            "compiledAt": int(time.time()),
            "id": _processor.get_id(),
            "language": _processor.get_language(),
        }

    # 编译
    @classmethod
    def compile(cls, path: str, out_dir: str | None = None) -> None:
        if not OS_PATH.isdir(path) and path.endswith(Processor.SCRIPTS_FILE_EXTENSION):
            cls._save(
                cls.load(path),
                out_dir if out_dir is not None else OS_PATH.dirname(path),
            )
        else:
            for _file in glob(OS_PATH.join(path, "*")):
                cls.compile(_file, out_dir)

    # 保存（子类可重写）
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
