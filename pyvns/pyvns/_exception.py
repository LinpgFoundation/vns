# 编译错误
class ScriptCompilerException(Exception):
    def __init__(self, *args: object):
        super().__init__(*args)
