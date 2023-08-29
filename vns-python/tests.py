from pyvns import Compiler
import os

EXAMPLES_DIR = os.path.join(os.path.dirname(__file__), "..", "examples")

Compiler.compile(os.path.join(EXAMPLES_DIR, "chapter_example.vns"))
