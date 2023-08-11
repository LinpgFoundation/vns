from typing import Final


# 角色立绘名称预处理模块
class CharacterNamingMetaData:
    # 立绘配置信息数据库
    __DATABASE: Final[dict] = {}
    # 是否立绘配置信息数据库
    __IS_DATABASE_ENABLED: bool = len(__DATABASE) > 0

    @classmethod
    def get_database(cls) -> dict:
        return cls.__DATABASE

    def __init__(self, _name: str) -> None:
        _name_data: list[str] = _name.split("&")
        self.__name: str = _name_data[0]
        self.__tags: set[str] = set(_name_data[1:])
        self.__silent: bool = False
        if "silent" in self.__tags:
            self.__silent = True
            self.__tags.remove("silent")

    @property
    def name(self) -> str:
        return self.__name

    @property
    def tags(self) -> set[str]:
        return self.__tags

    # 根据文件名判断是否是同一角色名下的图片
    def equal(
        self, otherNameData: "CharacterNamingMetaData", must_be_the_same: bool = False
    ) -> bool:
        if self.__name == otherNameData.name:
            return True
        elif self.__IS_DATABASE_ENABLED and not must_be_the_same:
            for key in self.__DATABASE:
                if self.__name in self.__DATABASE[key]:
                    return otherNameData.name in self.__DATABASE[key]
                elif otherNameData.name in self.__DATABASE[key]:
                    return self.__name in self.__DATABASE[key]
        return False

    # 是否有tag
    def has_tag(self, _tag: str) -> bool:
        if _tag == "silent":
            return self.__silent
        else:
            return _tag in self.__tags

    # 移除tag
    def remove_tag(self, _tag: str) -> None:
        if _tag == "silent":
            self.__silent = False
        else:
            new_tags: list[str] = []
            for original_tag in self.__tags:
                if original_tag != _tag:
                    new_tags.append(original_tag)
            self.__tags = set(new_tags)

    # 增加tag
    def add_tag(self, _tag: str) -> None:
        if _tag == "silent":
            self.__silent = True
        else:
            new_tags: list[str] = []
            for original_tag in self.__tags:
                if original_tag != _tag:
                    new_tags.append(original_tag)
            new_tags.append(_tag)
            self.__tags = set(new_tags)

    # 获取tag和名称结合后的数据名称
    def get_raw_name(self) -> str:
        raw_name: str = self.__name
        if self.__silent is True:
            raw_name += "&silent"
        for tag in self.__tags:
            raw_name += "&" + tag
        return raw_name
