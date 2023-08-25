import copy
from typing import Final, NoReturn

from ._exception import ScriptCompilerException
from .content import Content
from .naming import Naming


# 视觉小说脚本编译器
class Processor:
    # 文件格式后缀
    SCRIPTS_FILE_EXTENSION: Final[str] = ".vns"

    def __init__(self) -> None:
        self.__path_in: str = ""
        self.__line_index: int = 0
        self.__output: dict = {}
        self.__current_data: Content = Content({}, "head")
        self.__id: int = -1
        self.__lang: str = ""
        self.__section: str | None = None
        self.__previous: str | None = None
        self.__lines: list[str] = []
        self.__branch_labels: dict[str, str] = {}
        self.__dialog_associate_key: dict[str, str] = {}
        self.__accumulated_comments: list[str] = []
        self.__blocked: bool = False

    # 获取id
    def get_id(self) -> int:
        return self.__id

    # 获取语言
    def get_language(self) -> str:
        return self.__lang

    # 生成一个标准id
    @staticmethod
    def __generate_id(index: int) -> str:
        if index >= 10:
            return "id_" + str(index)
        elif index > 0:
            return "id_0" + str(index)
        else:
            return "head"

    # 转换一个str
    @staticmethod
    def __ensure_not_null(text: str) -> str | None:
        return None if text.lower() == "null" or text.lower() == "none" else text

    # 将参数分离出来
    @classmethod
    def __extract_parameter(cls, text: str, prefix: str) -> str | None:
        return cls.__ensure_not_null(cls.__extract_string(text, prefix))

    # 将字符串内容分离出来
    @staticmethod
    def __extract_string(text: str, prefix: str) -> str:
        sharp_index: int = text.find("#")
        return (
            text.removeprefix(prefix)
            if sharp_index < 0
            else text[:sharp_index].removeprefix(prefix)
        )

    # 编译失败
    def __terminated(self, _reason: str) -> NoReturn:
        raise ScriptCompilerException(
            f'File "{self.__path_in}", line {self.__line_index + 1}\n  {self.__get_current_line()}\nFail to compile due to {_reason}'
        )

    # 获取当前行
    def __get_current_line(self) -> str:
        return self.__lines[self.__line_index].lstrip()

    # 获取输出
    def get_output(self) -> dict:
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
        else:
            last_label: str | None = None
            # 预处理文件
            for index in range(len(self.__lines)):
                self.__lines[index] = self.__lines[index].removesuffix("\n")
                if self.__lines[index].startswith("[label]"):
                    if last_label is not None:
                        self.__terminated("The label is overwriting the previous one")
                    last_label = self.__extract_parameter(
                        self.__lines[index], "[label]"
                    )
                if self.__lines[index].startswith("[section]"):
                    current_index = 0
                elif (
                    not self.__lines[index].startswith("[")
                    and ":" in self.__lines[index]
                ):
                    if current_index > 0:
                        self.__dialog_associate_key[str(index)] = self.__generate_id(
                            current_index
                        )
                    else:
                        self.__dialog_associate_key[str(index)] = "head"
                    current_index += 1
                    # 将id与label关联
                    if last_label is not None:
                        self.__branch_labels[last_label] = self.__dialog_associate_key[
                            str(index)
                        ]
                        last_label = None
            if last_label is not None:
                print(f"The last label call {last_label} is not necessary!")
        self.__convert(0)
        self.__lines.clear()
        # 确保重要参数已被初始化
        if self.__id < 0:
            ScriptCompilerException("You have to set a nonnegative id!")
        elif len(self.__lang) <= 0:
            ScriptCompilerException("You have to set lang!")
        elif self.__section is None:
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
            else:
                # 背景图片
                if _currentLine.startswith("[bgi]"):
                    self.__current_data.background_image = self.__extract_parameter(
                        _currentLine, "[bgi]"
                    )
                # 背景音乐
                elif _currentLine.startswith("[bgm]"):
                    self.__current_data.background_music = self.__extract_parameter(
                        _currentLine, "[bgm]"
                    )
                # 角色进场
                elif _currentLine.startswith("[show]"):
                    for _name in self.__extract_string(_currentLine, "[show]").split():
                        self.__current_data.character_images.append(_name)
                # 角色退场
                elif _currentLine.startswith("[hide]"):
                    for _name in self.__extract_string(_currentLine, "[hide]").split():
                        # 清空角色列表
                        if _name == "*":
                            self.__current_data.character_images.clear()
                            break
                        # 移除角色
                        for i in range(len(self.__current_data.character_images)):
                            if Naming(self.__current_data.character_images[i]).equal(
                                _name
                            ):
                                self.__current_data.character_images.pop(i)
                                break
                # 清空角色列表，然后让角色重新进场
                elif _currentLine.startswith("[display]"):
                    self.__current_data.character_images.clear()
                    for _name in self.__extract_string(
                        _currentLine, "[display]"
                    ).split():
                        self.__current_data.character_images.append(_name)
                # 章节id
                elif _currentLine.startswith("[id]"):
                    _id: str | None = self.__extract_parameter(_currentLine, "[id]")
                    if _id is not None:
                        self.__id = int(_id)
                    else:
                        self.__terminated("Chapter id cannot be None!")
                # 语言
                elif _currentLine.startswith("[lang]"):
                    self.__lang = self.__extract_string(_currentLine, "[lang]")
                # 部分
                elif _currentLine.startswith("[section]"):
                    if self.__previous is not None:
                        self.__output[self.__section][self.__previous]["next"] = None
                    self.__section = self.__extract_string(_currentLine, "[section]")
                    self.__output[self.__section] = {}
                    self.__output[self.__section]["head"] = {}
                    self.__current_data = Content({}, "head")
                    self.__previous = None
                # 结束符
                elif _currentLine.startswith("[end]"):
                    self.__output[self.__section][self.__previous]["next"] = None
                    break
                # 转换场景
                elif _currentLine.startswith("[scene]"):
                    self.__output[self.__section][self.__previous]["next"][
                        "type"
                    ] = "scene"
                    self.__current_data.background_image = self.__extract_parameter(
                        _currentLine, "[scene]"
                    )
                    self.__blocked = True
                # 终端
                elif _currentLine.startswith("[block]"):
                    if self.__previous is not None:
                        self.__output[self.__section][self.__previous]["next"] = None
                    self.__current_data = Content({}, "id_needed")
                    self.__previous = None
                # 选项
                elif _currentLine.startswith("[opt]"):
                    # 确认在接下来的一行有branch的label
                    if not self.__lines[self.__line_index + 1].startswith("[br]"):
                        self.__terminated(
                            f"For option on line {self.__line_index + 1}, a branch label is not found on the following line"
                        )
                    # 如果next没被初始化，则初始化
                    if (
                        self.__output[self.__section][self.__previous].get("next")
                        is None
                    ):
                        self.__output[self.__section][self.__previous]["next"] = {}
                    # 获取对应的下一个对话字典的指针
                    dialog_next: dict = self.__output[self.__section][self.__previous][
                        "next"
                    ]
                    if dialog_next.get("type") != "option":
                        dialog_next["type"] = "option"
                        dialog_next["target"] = []
                    dialog_next["target"].append(
                        {
                            "text": self.__extract_string(_currentLine, "[opt]"),
                            "id": self.__branch_labels[
                                self.__extract_string(
                                    self.__lines[self.__line_index + 1], "[br]"
                                )
                            ],
                        }
                    )
                    self.__line_index += 1
                elif not _currentLine.startswith("[") and ":" in _currentLine:
                    _narrator: str | None = self.__ensure_not_null(
                        _currentLine.removesuffix(" ").removesuffix(":")
                    )
                    self.__current_data.narrator = (
                        _narrator if _narrator is not None else ""
                    )
                    # 获取讲述人可能的立绘名称
                    narrator_possible_images: tuple = tuple()
                    if self.__current_data.narrator.lower() in Naming.get_database():
                        narrator_possible_images = tuple(
                            Naming.get_database()[self.__current_data.narrator.lower()]
                        )
                    # 检查名称列表，更新character_images以确保不在说话的人处于黑暗状态
                    for i in range(len(self.__current_data.character_images)):
                        _name_data: Naming = Naming(
                            self.__current_data.character_images[i]
                        )
                        if _name_data.name in narrator_possible_images:
                            _name_data.remove_tag("silent")
                        else:
                            _name_data.add_tag("silent")
                        self.__current_data.character_images[
                            i
                        ] = _name_data.get_raw_name()
                    # 更新对话内容
                    self.__current_data.contents.clear()
                    for sub_index in range(self.__line_index + 1, len(self.__lines)):
                        if self.__lines[sub_index].startswith("- "):
                            self.__current_data.contents.append(
                                self.__extract_string(self.__lines[sub_index], "- ")
                            )
                        else:
                            break
                    # 确认section不为None，如果为None，则警告
                    if self.__section is None:
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
                        last_ref: dict | None = self.__output[self.__section].get(
                            self.__previous
                        )
                        if last_ref is not None:
                            if last_ref.get("next") is not None:
                                last_ref["next"][
                                    "target"
                                ] = self.__dialog_associate_key[str(self.__line_index)]
                                if "type" not in last_ref["next"]:
                                    last_ref["next"]["type"] = "default"
                            else:
                                last_ref["next"] = {
                                    "target": self.__dialog_associate_key[
                                        str(self.__line_index)
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
                    self.__previous = self.__dialog_associate_key[
                        str(self.__line_index)
                    ]
                    # 更新缓存参数
                    self.__line_index += len(self.__current_data.contents)
                    self.__output[self.__section][self.__previous] = copy.deepcopy(
                        self.__current_data.to_dict()
                    )
                    # 移除注释
                    self.__current_data.comments.clear()
                else:
                    self.__terminated("unexpected reason")
            self.__line_index += 1
