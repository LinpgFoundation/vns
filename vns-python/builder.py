from os import path as OS_PATH

from linpgtoolbox.builder import Builder

# 需要额外包括的文件
additional_files: tuple[str, ...] = (
    OS_PATH.join(OS_PATH.dirname(__file__), r"../README.md"),
    OS_PATH.join(OS_PATH.dirname(__file__), r"../LICENSE"),
)

Builder.CACHE_NEED_REMOVE.append("cmake-build-debug")
Builder.CACHE_NEED_REMOVE.append("extern")
Builder.CACHE_NEED_REMOVE.append("wrapper.cpp")

# 编译源代码
Builder.compile(
    "pyvns",
    additional_files=additional_files,
    update_the_one_in_sitepackages=True,
    include_pyinstaller_program=True,
    ignore_key_words=("extern",),
    options={"enable_multiprocessing": True, "auto_cmake": True},
)

# 提示编译完成
for i in range(2):
    print("")
print("--------------------Done!--------------------")
for i in range(2):
    print("")

# 打包上传最新的文件
"""
action: str = input("Do you want to package and upload the latest build (Y/n):")
if action == "Y":
    Builder.copy(additional_files, OS_PATH.dirname(__file__))
    Builder.build()
    Builder.upload()
    for additional_file in additional_files:
        Builder.remove(
            OS_PATH.join(OS_PATH.dirname(__file__), OS_PATH.basename(additional_file))
        )
elif action != "N":
    Builder.remove("src")
"""
