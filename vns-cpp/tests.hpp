#ifndef TESTS_HPP
#define TESTS_HPP

#include <string>
#include <filesystem>

const std::filesystem::path EXAMPLE_VNS_TEST_FILE = "C:/Users/yudon/Documents/GitHub/vns/examples/chapter_example.vns";
const std::filesystem::path EXAMPLE_VNS_TEST_FILE_OUTPUT_DIR = "C:/Users/yudon/Documents/GitHub/vns/examples";

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
