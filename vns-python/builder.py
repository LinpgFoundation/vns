import shutil
from os import path as OS_PATH

from linpgtoolbox.builder import Builder

Builder.CACHE_NEED_REMOVE.append("cmake-build-debug")
Builder.CACHE_NEED_REMOVE.append("extern")
Builder.CACHE_NEED_REMOVE.append("wrapper.cpp")

# always use the latest version of cpp code
Builder.remove(OS_PATH.join(OS_PATH.dirname(__file__), "pyvns", "extern", "vns-cpp"))
shutil.copytree(
    OS_PATH.join(OS_PATH.dirname(__file__), "..", "vns-cpp"),
    OS_PATH.join(OS_PATH.dirname(__file__), "pyvns", "extern", "vns-cpp"),
)

# compile the code
Builder.compile("pyvns", upgrade=True)
