from typing import Final, Sequence


class _Naming:
    def __init__(self, _name: str) -> None:
        _name_data: list[str] = _name.strip().split("&")
        self.__name: str = _name_data[0]
        self.__tags: set[str] = set(_name_data[1:])

    # 获取tag和名称结合后的数据名称
    def __str__(self) -> str:
        return self.__name + "".join(f"&{_tag}" for _tag in self.__tags)

    @property
    def name(self) -> str:
        return self.__name

    @property
    def tags(self) -> set[str]:
        return self.__tags


# 角色立绘名称预处理模块
class Naming(_Naming):
    # 立绘配置信息数据库
    __DATABASE: Final[dict[str, Sequence[str]]] = {}

    @classmethod
    def get_database(cls) -> dict[str, Sequence[str]]:
        return cls.__DATABASE

    # 根据文件名判断是否是同一角色名下的图片
    def equal(
        self, otherNameData: _Naming | str, must_be_the_same: bool = False
    ) -> bool:
        if isinstance(otherNameData, str):
            otherNameData = Naming(otherNameData)
        if self.name == otherNameData.name:
            return True
        elif not must_be_the_same:
            for key in self.__DATABASE:
                if self.name in self.__DATABASE[key]:
                    return otherNameData.name in self.__DATABASE[key]
                elif otherNameData.name in self.__DATABASE[key]:
                    return self.name in self.__DATABASE[key]
        return False
