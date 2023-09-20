import json
import os

from pyvns import Compiler

EXAMPLES_DIR = os.path.join(os.path.dirname(__file__), "..", "examples")

# use compiler to load example 1 vns file
example1_result = Compiler.load(os.path.join(EXAMPLES_DIR, "chapter_example.vns"))

# load example 1 expected result
with open(
    os.path.join(EXAMPLES_DIR, "chapter1_dialogs_exp.json"), "r", encoding="utf-8"
) as f:
    example1_exp = json.load(f)

assert example1_exp["language"] == example1_result["language"]
assert example1_exp["id"] == example1_result["id"]
assert example1_exp["dialogs"] == example1_result["dialogs"]
