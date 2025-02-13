#ifndef TESTS_HPP
#define TESTS_HPP

#include <filesystem>

class Tests
{
    static std::filesystem::path inline EXAMPLE_VNS_TEST_FILES_DIR;
    static std::filesystem::path inline EXAMPLE_VNS_TEST_FILE;
    static std::filesystem::path inline EXAMPLE_VNS_BRANCHES_TESTS_FILE;
    static std::filesystem::path inline EXAMPLE_DUMMY_TEST_FILE;

    static void TestBasicFunctions();

    static void TestNameWithoutTag();

    static void TestNameWithTags();

    static void TestNullNext();

    static void TestSingleTargetNext();

    static void TestMultiTargetsNext();

    static void TestScriptProcessor();

    static void TestCompiler();

    static void TestDialoguesManager();

    static void TestBranching();

public:
    static void SetTestFolderPath(const std::filesystem::path &);

    static void TestAll();
};

#endif
