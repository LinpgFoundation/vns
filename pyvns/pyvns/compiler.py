import json
import os
import time
from glob import glob
from typing import Any

from .content import ContentManager
from .naming import Naming
from .processor import Processor


class Compiler:
    # 如果输入字符串为None，则将其转换为null
    @staticmethod
    def __to_str_in_case_null(text: str | None) -> str:
        return text if text is not None else "null"

    # 直接加载
    @staticmethod
    def load(path: str) -> dict:
        processor: Processor = Processor()
        processor.process(path)
        return processor.get_output()

    # 编译
    @classmethod
    def compile(cls, path: str, out_dir: str | None = None) -> None:
        if not os.path.isdir(path):
            processor: Processor = Processor()
            processor.process(path)
            cls._save_output(
                processor, out_dir if out_dir is not None else os.path.dirname(path)
            )
        else:
            for _file in glob(os.path.join(path, "*")):
                cls.compile(_file, out_dir)

    # 保存至
    @staticmethod
    def _save_output(_processor: Processor, out_dir: str) -> None:
        with open(
            os.path.join(
                out_dir,
                f"chapter{_processor.get_id()}_dialogs_{_processor.get_language()}.json",
            ),
            "w",
            encoding="utf-8",
        ) as f:
            json.dump(
                {"dialogs": _processor.get_output(), "compiledAt": int(time.time())},
                f,
                indent=4,
                ensure_ascii=False,
                sort_keys=True,
            )

    # 反编译
    @classmethod
    def decompile(cls, _data: dict[str, dict[str, dict[str, Any]]], out: str) -> None:
        # 如果数据为空，则直接返回
        if _data is None or len(_data) <= 0:
            return
        # 初始化视觉小说数据管理模块
        _content: ContentManager = ContentManager()
        # 把数据更新到管理模块中
        _content.update(_data)
        # 用于储存结果的列表
        _results: list[str] = [
            f"# Fundamental parameters\n[id]{_data['id']}\n[lang]{_data['language']}\n"
        ]
        # 历遍所有section
        for _section in _data:
            # 更新视觉小说数据管理模块的当前位置
            _content.set_id("head")
            _content.set_section(_section)
            # 写入当前部分的名称
            _results.append("\n[section]" + _section + "\n")

            while True:
                _current_dialog: dict[str, Any] = _content.get_dialog()
                # 处理注释
                comments: list[str] = _current_dialog.pop("comments", [])
                if len(comments) > 0:
                    _results.append("\n")
                    _results.extend("// " + _note + "\n" for _note in comments)
                # 写入讲话人名称
                _results.append(
                    "null:\n"
                    if len(_content.current.narrator) == 0
                    else _content.current.narrator + ":\n"
                )
                # 写入对话
                for _sentence in _content.current.contents:
                    _results.append("- " + _sentence + "\n")

                """如果下列内容有变化，则写入"""
                # 写入背景
                if (
                    _content.previous is None
                    or _content.current.background_image
                    != _content.previous.background_image
                ):
                    if (
                        _content.previous is None
                        or _content.previous.next.type != "scene"
                    ):
                        _results.append(
                            "[bgi]"
                            + cls.__to_str_in_case_null(
                                _content.current.background_image
                            )
                            + "\n"
                        )
                    else:
                        _results.append(
                            "[scene]"
                            + cls.__to_str_in_case_null(
                                _content.current.background_image
                            )
                            + "\n"
                        )
                # 写入背景音乐
                if (
                    _content.previous is None
                    or _content.previous.background_music
                    != _content.current.background_music
                ):
                    _results.append(
                        "[bgm]"
                        + cls.__to_str_in_case_null(_content.current.background_music)
                        + "\n"
                    )
                # 写入当前立绘
                if (
                    _content.previous is None
                    or _content.previous.character_images
                    != _content.current.character_images
                ):
                    if len(_content.current.character_images) == 0:
                        _results.append("[hide]*\n")
                    elif (
                        _content.previous is None
                        or len(_content.previous.character_images) == 0
                    ):
                        _line: str = "[display]"
                        for _characterName in _content.current.character_images:
                            _line += Naming(_characterName).name + " "
                        _results.append(_line.rstrip() + "\n")

                if _content.current.has_next():
                    _content.set_id(_content.current.next.target)  # type: ignore
                else:
                    break

        # 写入停止符
        _results.append("\n[end]\n")

        # 保存反编译好的脚本
        with open(out, "w+", encoding="utf-8") as f:
            f.writelines(_results)
