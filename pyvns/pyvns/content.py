from typing import Any, Final


class _Next:
    def __init__(self, _data: dict[str, Any] | None) -> None:
        if _data is None:
            _data = {}
        self.type: Final[str | None] = _data.get("type")
        self.target: Final[str | list[dict[str, str]] | None] = _data.get("target")

    def to_dict(self) -> dict[str, str | list[dict[str, str]]]:
        return {"type": self.type, "target": self.target} if self.is_not_null() else {}  # type: ignore

    def is_not_null(self) -> bool:
        return self.type is not None and self.target is not None


# 视觉小说数据操作接口
class Content:
    def __init__(self, _data: dict[str, Any], _id: str) -> None:
        # id
        self.__id: str = _id
        # 背景图片
        self.background_image: str | None = _data.get("background_image")
        # 背景音乐
        self.background_music: str | None = _data.get("background_music")
        # 角色立绘
        self.character_images: list[str] = _data.get("character_images", [])
        # 对话
        self.contents: list[str] = _data.get("contents", [])
        # 讲述人
        self.narrator: str = _data.get("narrator", "")
        # 上一个
        self.previous: str | None = _data.get("previous")
        # 下一个
        self.next: _Next = _Next(_data.get("next"))
        # 注释
        self.comments: list[str] = []

    @property
    def id(self) -> str:
        return self.__id

    # 当前对话是否带有合法的下一个对话对象的id
    def has_next(self) -> bool:
        return self.next.is_not_null()

    def to_dict(self) -> dict[str, Any]:
        _result: dict[str, Any] = {
            "background_image": self.background_image,
            "background_music": self.background_music,
            "character_images": self.character_images,
            "contents": self.contents,
            "previous": self.previous,
            "narrator": self.narrator,
            "next": self.next.to_dict(),
        }
        if len(self.comments) > 0:
            _result["comments"] = self.comments
        return _result


# 视觉小说数据管理模块
class ContentManager:
    def __init__(self) -> None:
        # 视觉小说数据
        self.__dialog_data: Final[dict[str, dict[str, dict[str, Any]]]] = {}
        # 当前部分
        self.__section: str = ""
        # 当前对话的id
        self.__id: str = "head"
        # 当前对话的接口模块
        self.__current: Content | None = None
        # 之前对话的接口模块
        self.__last: Content | None = None
        # 上一次对话的接口模块
        self.__previous: Content | None = None

    # 如果指向当前对话数据的指针不存在，则更新指针
    def __refresh_current(self) -> None:
        if self.__current is None:
            self.__previous = self.__current
            self.__current = Content(
                self.__dialog_data[self.__section][self.__id], self.__id
            )

    # 上一个对话的缓存
    @property
    def previous(self) -> Content | None:
        self.__refresh_current()
        return self.__previous

    # 指向当前对话数据的指针
    @property
    def current(self) -> Content:
        self.__refresh_current()
        return self.__current  # type: ignore

    # 指向之前对话数据的指针
    @property
    def last(self) -> Content | None:
        # 如果指针不存在，则更新接口
        if self.__last is None and self.current.previous is not None:
            self.__last = Content(
                self.__dialog_data[self.__section][self.current.previous],
                self.current.previous,
            )
        return self.__last

    # 保存对当前对话的接口的修改
    def save(self) -> None:
        self.__dialog_data[self.__section][self.__id] = self.current.to_dict()

    # 是否数据为空
    def is_empty(self) -> bool:
        return len(self.__dialog_data) <= 0

    # 清空数据
    def clear(self) -> None:
        self.__dialog_data.clear()

    # 更新数据
    def update(self, _data: dict[str, dict[str, dict[str, Any]]]) -> None:
        self.__dialog_data.update(_data)

    # 获取数据
    def get(self) -> dict[str, dict[str, dict[str, dict[str, Any]]]]:
        return self.__dialog_data

    # 获取当前id
    def get_id(self) -> str:
        return self.__id

    # 更新当前id
    def set_id(self, _id: str) -> None:
        self.__id = _id
        self.__current = None
        self.__last = None

    # 获取当前段落名称
    def get_section(self) -> str:
        return self.__section

    # 获取指定段落名称
    def __get_section(self, _section: str | None) -> str:
        return self.__section if _section is None else _section

    # 设置当前段落名称
    def set_section(self, _section: str) -> None:
        self.__section = _section
        self.__current = None
        self.__last = None

    # 移除段落
    def remove_section(self, _id: str | None = None) -> None:
        self.__dialog_data[self.__section].pop(self.__id if _id is None else _id)

    # 获取段落
    def get_section_content(
        self, _section: str | None = None
    ) -> dict[str, dict[str, Any]]:
        return self.__dialog_data[self.__get_section(_section)]

    # 设置段落
    def set_section_content(
        self, _data: dict[str, dict[str, Any]], _section: str | None = None
    ) -> None:
        self.__dialog_data[self.__get_section(_section)] = _data

    # 获取对话数据
    def get_dialog(
        self, _section: str | None = None, _id: str | None = None
    ) -> dict[str, Any]:
        return self.get_section_content(_section)[self.__id if _id is None else _id]

    # 移除对话数据
    def remove_dialog(
        self, _section: str | None = None, _id: str | None = None
    ) -> None:
        self.get_section_content(_section).pop(self.__id if _id is None else _id)
