import json
import os
import time
from glob import glob
from typing import Any

from .processor import Processor


class Compiler:
    # 直接加载
    @staticmethod
    def load(path: str) -> dict[str, dict[str, dict[str, Any]]]:
        processor: Processor = Processor()
        processor.process(path)
        return processor.get_output()

    # 编译
    @classmethod
    def compile(cls, path: str, out_dir: str | None = None) -> None:
        if not os.path.isdir(path) and path.rstrip().endswith(
            Processor.SCRIPTS_FILE_EXTENSION
        ):
            _processor: Processor = Processor()
            _processor.process(path)
            cls._save(
                {
                    "dialogs": _processor.get_output(),
                    "compiledAt": int(time.time()),
                    "id": _processor.get_id(),
                    "language": _processor.get_language(),
                },
                out_dir if out_dir is not None else os.path.dirname(path),
            )
        else:
            for _file in glob(os.path.join(path, "*")):
                cls.compile(_file, out_dir)

    # 保存（子类可重写）
    @staticmethod
    def _save(_data: dict[str, Any], _dir: str) -> None:
        with open(
            os.path.join(
                _dir, f"chapter{_data['id']}_dialogs_{_data['language']}.json"
            ),
            "w",
            encoding="utf-8",
        ) as f:
            json.dump(_data, f, indent=4, ensure_ascii=False, sort_keys=True)
