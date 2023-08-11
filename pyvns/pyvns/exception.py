# 编译错误
class ScriptsProcessingException(Exception):
    def __init__(self, *args: object):
        super().__init__(*args)
