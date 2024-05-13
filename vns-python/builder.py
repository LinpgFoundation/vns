from os import path as OS_PATH

from linpgtoolbox.builder import Builder

# any additional files that needs to be included
additional_files: tuple[str, ...] = (
    OS_PATH.join(OS_PATH.dirname(__file__), "..", "README.md"),
    OS_PATH.join(OS_PATH.dirname(__file__), "..", "LICENSE"),
)

Builder.CACHE_NEED_REMOVE.append("cmake-build-debug")
Builder.CACHE_NEED_REMOVE.append("extern")
Builder.CACHE_NEED_REMOVE.append("wrapper.cpp")

# always use the latest version of cpp code
Builder.remove(OS_PATH.join(OS_PATH.dirname(__file__), "pyvns", "extern", "vns-cpp"))
Builder.copy(
    (OS_PATH.join(OS_PATH.dirname(__file__), "..", "vns-cpp"),),
    OS_PATH.join(OS_PATH.dirname(__file__), "pyvns", "extern"),
)

# compile the code
Builder.compile(
    "pyvns",
    additional_files=additional_files,
    update_the_one_in_sitepackages=True,
    include_pyinstaller_program=True,
    ignore_key_words=("extern",),
    options={"enable_multiprocessing": True, "auto_cmake": True},
)

# prompt user that the compiling process has completed
for i in range(2):
    print("")
print("--------------------Done!--------------------")
for i in range(2):
    print("")

# upload the last version of the package
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
