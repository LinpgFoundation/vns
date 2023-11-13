import copy
from typing import Any, Final, NoReturn

from ._exception import ScriptCompilerException
from .content import Content
from .naming import Naming


# 视觉小说脚本编译器
class Processor:
    # file extension for vns
    SCRIPTS_FILE_EXTENSION: Final[str] = ".vns"
    # short forms of words
    __ALTERNATIVES: dict[str, str] = {
        "lang": "language",
        "opt": "option",
        "disp": "display",
    }
    # reserved words that cannot be used as label
    __RESERVED_WORDS: tuple[str, ...] = ("null", "none", "head")
    # tag formate
    TAG_STARTS: str = "["
    TAG_ENDS: str = "]"

    def __init__(self) -> None:
        self.__path_in: str = ""
        self.__line_index: int = 0
        self.__output: dict[str, dict[str, dict[str, Any]]] = {}
        self.__current_data: Content = Content({}, "head")
        self.__id: int = -1
        self.__lang: str = ""
        self.__section: str = ""
        self.__previous: str | None = None
        self.__lines: list[str] = []
        self.__dialog_associate_key: dict[int, str] = {}
        self.__accumulated_comments: list[str] = []
        self.__blocked: bool = False

    # 获取id
    def get_id(self) -> int:
        return self.__id

    # 获取语言
    def get_language(self) -> str:
        return self.__lang

    # 转换一个str
    @staticmethod
    def __ensure_not_null(text: str) -> str | None:
        return None if text.lower() == "null" or text.lower() == "none" else text

    # 将参数分离出来
    @classmethod
    def __extract_parameter(cls, text: str) -> str | None:
        return cls.__ensure_not_null(cls.__extract_string(text))

    # extract label from given string
    @classmethod
    def __extract_tag(cls, text: str) -> str:
        return text[text.index(cls.TAG_STARTS) + 1 : text.index(cls.TAG_ENDS)]

    # 将字符串内容分离出来
    @classmethod
    def __extract_string(cls, text: str) -> str:
        return text[text.index(cls.TAG_ENDS) + 1 :].strip()

    # 编译失败
    def __terminated(self, _reason: str) -> NoReturn:
        raise ScriptCompilerException(
            f'File "{self.__path_in}", line {self.__line_index + 1}\n  {self.__get_current_line()}\nFail to compile due to {_reason}'
            if self.__line_index < len(self.__lines)
            else f'File "{self.__path_in}", Fail to compile due to {_reason}'
        )

    # 获取当前行
    def __get_current_line(self) -> str:
        return self.__lines[self.__line_index]

    # 获取输出
    def get_output(self) -> dict[str, dict[str, dict[str, Any]]]:
        return self.__output

    # 处理数据
    def process(self, _path: str) -> None:
        self.__path_in = _path
        current_index: int = 0
        if self.__path_in.endswith(self.SCRIPTS_FILE_EXTENSION):
            with open(self.__path_in, "r", encoding="utf-8") as f:
                self.__lines = f.readlines()
        # 如果文件为空
        if len(self.__lines) <= 0:
            self.__terminated("Cannot convert an empty script file!")
        # 上个label
        last_label: str | None = None
        # 预处理数据
        for index in range(len(self.__lines)):
            self.__lines[index] = self.__lines[index].removesuffix("\n").strip()
            if self.__lines[index].startswith(self.TAG_STARTS):
                match self.__extract_tag(self.__lines[index]):
                    case "label":
                        if last_label is not None:
                            self.__terminated(
                                "This label is overwriting the previous one"
                            )
                        last_label = self.__extract_parameter(self.__lines[index])
                        if last_label in self.__RESERVED_WORDS:
                            self.__terminated(
                                f"You cannot use reserved work '{last_label}' as a label"
                            )
                    case "section":
                        current_index = 0
            elif ":" in self.__lines[index]:
                self.__dialog_associate_key[index] = (
                    "head"
                    if current_index == 0
                    else (
                        f"~0{current_index}"
                        if current_index < 10
                        else f"~{current_index}"
                    )
                    if last_label is None
                    else last_label
                )
                last_label = None
                current_index += 1
        self.__convert(0)
        self.__lines.clear()
        # 确保重要参数已被初始化
        if self.__id < 0:
            ScriptCompilerException("You have to set a nonnegative id!")
        elif len(self.__lang) <= 0:
            ScriptCompilerException("You have to set lang!")
        elif len(self.__section) <= 0:
            ScriptCompilerException("You have to set section!")

    # 转换
    def __convert(self, staring_index: int) -> None:
        self.__line_index = staring_index
        while self.__line_index < len(self.__lines):
            _currentLine: str = self.__get_current_line()
            if len(_currentLine) <= 0 or self.__lines[self.__line_index].startswith(
                "#"
            ):
                pass
            elif _currentLine.startswith("//"):
                self.__accumulated_comments.append(_currentLine.lstrip("//").lstrip())
            elif _currentLine.startswith(self.TAG_STARTS):
                _tag: str = self.__extract_tag(_currentLine)
                match self.__ALTERNATIVES.get(_tag, _tag):
                    # 背景图片
                    case "bgi":
                        self.__current_data.background_image = self.__extract_parameter(
                            _currentLine
                        )
                    # 背景音乐
                    case "bgm":
                        self.__current_data.background_music = self.__extract_parameter(
                            _currentLine
                        )
                    # 角色进场
                    case "show":
                        for _name in self.__extract_string(_currentLine).split():
                            self.__current_data.character_images.append(_name)
                    # 角色退场
                    case "hide":
                        for _name in self.__extract_string(_currentLine).split():
                            # 清空角色列表
                            if _name == "*":
                                self.__current_data.character_images.clear()
                                break
                            # 移除角色
                            for i in range(len(self.__current_data.character_images)):
                                if Naming(
                                    self.__current_data.character_images[i]
                                ).equal(_name):
                                    self.__current_data.character_images.pop(i)
                                    break
                    # 清空角色列表，然后让角色重新进场
                    case "display":
                        self.__current_data.character_images.clear()
                        for _name in self.__extract_string(_currentLine).split():
                            self.__current_data.character_images.append(_name)
                    # 章节id
                    case "id":
                        _id: str | None = self.__extract_parameter(_currentLine)
                        if _id is not None:
                            self.__id = int(_id)
                        else:
                            self.__terminated("Chapter id cannot be None!")
                    # 语言
                    case "language":
                        self.__lang = self.__extract_string(_currentLine)
                    # 部分
                    case "section":
                        if self.__previous is not None:
                            self.__output[self.__section][self.__previous][
                                "next"
                            ] = None
                        self.__section = self.__extract_string(_currentLine)
                        self.__output[self.__section] = {}
                        self.__output[self.__section]["head"] = {}
                        self.__current_data = Content({}, "head")
                        self.__previous = None
                    # 结束符
                    case "end":
                        assert self.__previous is not None
                        self.__output[self.__section][self.__previous]["next"] = None
                        break
                    # 转换场景
                    case "scene":
                        assert self.__previous is not None
                        self.__output[self.__section][self.__previous]["next"][
                            "type"
                        ] = "scene"
                        self.__current_data.background_image = self.__extract_parameter(
                            _currentLine
                        )
                        if (
                            isinstance(self.__current_data.background_image, str)
                            and len(self.__current_data.background_image) == 0
                        ):
                            self.__current_data.background_image = None
                        self.__blocked = True
                    # 终端
                    case "block":
                        if self.__previous is not None:
                            self.__output[self.__section][self.__previous][
                                "next"
                            ] = None
                        self.__current_data = Content({}, "id_needed")
                        self.__previous = None
                    # 选项
                    case "option":
                        # 确认有目标
                        if "->" not in _currentLine:
                            self.__terminated(
                                "Invalid option syntax: '->' cannot be found!"
                            )
                        # 如果next没被初始化，则初始化
                        assert self.__previous is not None
                        if (
                            self.__output[self.__section][self.__previous].get("next")
                            is None
                        ):
                            self.__output[self.__section][self.__previous]["next"] = {}
                        # 获取对应的下一个对话字典的指针
                        next_ref: dict = self.__output[self.__section][self.__previous][
                            "next"
                        ]
                        if next_ref.get("type") != "options":
                            next_ref["type"] = "options"
                            next_ref["target"] = []
                        assert isinstance(next_ref["target"], list)
                        src_to_target: str = self.__extract_string(_currentLine)
                        next_ref["target"].append(
                            {
                                "text": src_to_target[
                                    : src_to_target.index("->")
                                ].strip(),
                                "id": self.__ensure_not_null(
                                    src_to_target[
                                        src_to_target.index("->") + 2 :
                                    ].strip()
                                ),
                            }
                        )
                    case "label":
                        pass
                    case _:
                        self.__terminated(f"invalid tag {_tag}")
            elif ":" in _currentLine:
                _narrator: str | None = self.__ensure_not_null(
                    _currentLine.removesuffix(":")
                )
                self.__current_data.narrator = (
                    _narrator if _narrator is not None else ""
                )
                # 获取讲述人可能的立绘名称
                narrator_possible_images: tuple[str, ...] = tuple()
                if self.__current_data.narrator.lower() in Naming.get_database():
                    narrator_possible_images = tuple(
                        Naming.get_database()[self.__current_data.narrator.lower()]
                    )
                # 检查名称列表，更新character_images以确保不在说话的人处于黑暗状态
                for i in range(len(self.__current_data.character_images)):
                    _name_data: Naming = Naming(self.__current_data.character_images[i])
                    if _name_data.name in narrator_possible_images:
                        if "silent" in _name_data.tags:
                            _name_data.tags.remove("silent")
                    else:
                        _name_data.tags.add("silent")
                    self.__current_data.character_images[i] = str(_name_data)
                # 更新对话内容
                self.__current_data.contents.clear()
                for sub_index in range(self.__line_index + 1, len(self.__lines)):
                    if self.__lines[sub_index].startswith("- "):
                        self.__current_data.contents.append(
                            self.__lines[sub_index].removeprefix("- ").strip()
                        )
                    else:
                        break
                # 确认section不为None，如果为None，则警告
                if len(self.__section) <= 0:
                    self.__terminated("You have to specify section before script")
                # 如果section未在字典中，则初始化对应section的数据
                elif self.__section not in self.__output:
                    self.__output[self.__section] = {}
                # 如果上个dialog存在（不一定非得能返回）
                if self.__previous is not None:
                    if not self.__blocked:
                        self.__current_data.previous = self.__previous
                    else:
                        self.__current_data.previous = None
                        self.__blocked = False
                    # 生成数据
                    last_ref: dict[str, Any] | None = self.__output[self.__section].get(
                        self.__previous
                    )
                    if last_ref is not None:
                        if last_ref.get("next") is not None:
                            if last_ref["next"].get("type") != "options":
                                last_ref["next"][
                                    "target"
                                ] = self.__dialog_associate_key[self.__line_index]
                                if "type" not in last_ref["next"]:
                                    last_ref["next"]["type"] = "default"
                        else:
                            last_ref["next"] = {
                                "target": self.__dialog_associate_key[
                                    self.__line_index
                                ],
                                "type": "default",
                            }
                    else:
                        self.__terminated(f"KeyError: {self.__previous}")
                else:
                    self.__current_data.previous = None
                # 添加注释
                if len(self.__accumulated_comments) > 0:
                    self.__current_data.comments = self.__accumulated_comments
                    self.__accumulated_comments = []
                # 更新key
                self.__previous = self.__dialog_associate_key[self.__line_index]
                # 更新缓存参数
                self.__line_index += len(self.__current_data.contents)
                self.__output[self.__section][self.__previous] = copy.deepcopy(
                    self.__current_data.to_dict()
                )
                # 移除注释
                self.__current_data.comments.clear()
            else:
                self.__terminated("Invalid code or content!")
            self.__line_index += 1
