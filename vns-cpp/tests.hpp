#ifndef TESTS_HPP
#define TESTS_HPP

#include <string>
#include <filesystem>

const std::filesystem::path EXAMPLE_VNS_TEST_FILE_OUTPUT_DIR = "C:/Users/yudon/Documents/GitHub/vns/examples";
const std::filesystem::path EXAMPLE_VNS_TEST_FILE = EXAMPLE_VNS_TEST_FILE_OUTPUT_DIR / "chapter_example.vns";
const std::filesystem::path EXAMPLE_DUMMY_TEST_FILE = EXAMPLE_VNS_TEST_FILE_OUTPUT_DIR / "dummy.txt";

void TestNameWithoutTag();

void TestNameWithTags();

void TestNullNext();

void TestSingleTargetNext();

void TestMultiTargetsNext();

void TestAll();

void TestScriptProcessor();

void TestCompiler();

void TestDialoguesManager();

#endif
