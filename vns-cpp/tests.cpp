#include "tests.hpp"
#include <cassert>
#include "dialogueNext.hpp"
#include "naming.hpp"
#include <iostream>
#include "scriptProcessor.hpp"
#include "compiler.hpp"
#include "validator.hpp"
#include "functions.hpp"
#include "version.hpp"

void Tests::SetTestFolderPath(const std::filesystem::path &dir)
{
    EXAMPLE_VNS_TEST_FILES_DIR = dir;
    EXAMPLE_VNS_TEST_FILE = EXAMPLE_VNS_TEST_FILES_DIR / "chapter_example.vns";
    EXAMPLE_VNS_BRANCHES_TESTS_FILE = EXAMPLE_VNS_TEST_FILES_DIR / "chapter_branches_tests.vns";
    EXAMPLE_DUMMY_TEST_FILE = EXAMPLE_VNS_TEST_FILES_DIR / "dummy.txt";
}

void Tests::TestBasicFunctions()
{
    // test version compatible feature
    assert(is_version_compatible("", VERSION, REVISION));
    assert(is_version_compatible("<=", VERSION, REVISION));
    assert(is_version_compatible(">=", VERSION, REVISION));
    assert(is_version_compatible(">=", VERSION, REVISION + 1));
    assert(!is_version_compatible("<=", VERSION - 1, REVISION));
    assert(!is_version_compatible(">=", VERSION + 1, REVISION));
    assert(is_version_compatible("!<=", VERSION, REVISION));
    assert(is_version_compatible("!>=", VERSION, REVISION));
    assert(is_version_compatible("!<=", VERSION - 1, REVISION + 1));
    assert(is_version_compatible("!>=", VERSION + 1, REVISION));
}

void Tests::TestNameWithoutTag()
{
    const std::string test_name_str = "maria.png";
    const Naming test_name(test_name_str);
    assert(test_name.get_name() == test_name_str);
    assert(test_name.get_tags().empty());
    assert(!test_name.contains_tag("silent"));
    assert(test_name.to_string() == test_name_str);
}

void Tests::TestNameWithTags()
{
    const std::string test_name_str = "maria.png&silent&hide";
    const std::string test_name_img = "maria.png";
    const Naming test_name(test_name_str);
    assert(test_name.get_name() == test_name_img);
    assert(test_name.equal(test_name_img));
    assert(test_name.equal(Naming(test_name_img)));
    assert(test_name.equal(test_name_str));
    assert(test_name.equal(Naming(test_name_str)));
    const std::unordered_set<std::string> expected_tags = {"silent", "hide"};
    assert(test_name.get_tags() == expected_tags);
    assert(test_name.contains_tag("hide"));
    assert(test_name.contains_tag("silent"));
}

void Tests::TestNullNext()
{
    const DialogueNext test_null_next;
    assert(test_null_next.is_null());
    assert(!test_null_next.has_multi_targets());
    assert(test_null_next.has_type("default"));
    assert(test_null_next.get_target().empty());
}

void Tests::TestSingleTargetNext()
{
    const DialogueNext single_target_next(dialogue_next_t({{"type",   "default"},
                                                           {"target", "~1"}}));
    assert(!single_target_next.is_null());
    assert(!single_target_next.has_multi_targets());
    assert(single_target_next.has_type("default"));
    assert(single_target_next.get_target() == "~1");
}

void Tests::TestMultiTargetsNext()
{
    const std::unordered_map<std::string, std::string> t1 = {{"hello1", "world1"}};
    const std::unordered_map<std::string, std::string> t2 = {{"hello2", "world2"}};
    const multi_targets_t target_v = {t1, t2};
    const DialogueNext multi_targets_next(dialogue_next_t({{"type",   "default"},
                                                           {"target", target_v}}));
    assert(!multi_targets_next.is_null());
    assert(multi_targets_next.has_multi_targets());
    assert(multi_targets_next.has_type("default"));
    assert(multi_targets_next.get_targets().size() == 2);
}

void Tests::TestScriptProcessor()
{
    ScriptProcessor test_processor;
    test_processor.process(EXAMPLE_VNS_TEST_FILE);
    assert(test_processor.get_id() == "chapter1_dialogs_English");
    assert(test_processor.get_language() == "English");
    // test split and join to make sure they are working
    const std::vector<std::string> vns_lines = load_file_as_lines(EXAMPLE_VNS_TEST_FILE);
    const std::string vns_raw = join(vns_lines, "\n");
    const std::vector<std::string> vns_lines2 = split(vns_raw, '\n');
    assert(vns_lines.size() == vns_lines2.size());
    assert(vns_lines == vns_lines2);
    // reset test_processor
    test_processor = ScriptProcessor();
    // test processing raw vns string
    test_processor.process(vns_raw);
    assert(test_processor.get_id() == "chapter1_dialogs_English");
    assert(test_processor.get_language() == "English");
}

void Tests::TestCompiler()
{
    // compile the file
    Compiler::compile(EXAMPLE_VNS_TEST_FILE, EXAMPLE_VNS_TEST_FILES_DIR);
    // make sure the output file exists
    const std::filesystem::path outFileName = "chapter1_dialogs_English.json";
    const std::filesystem::path jsonPath = EXAMPLE_VNS_TEST_FILES_DIR / outFileName;
    assert(std::filesystem::exists(jsonPath));
    // make sure the file is in the correct format
    Validator::ensure(jsonPath);
    // try to load the JSON file
    DialoguesManager test_dialogues_manager;
    test_dialogues_manager.load(jsonPath);
    // test_dialogues_manager's default section will be set to dialog_example
    assert(test_dialogues_manager.get_section() == "dialog_example");
    // test_dialogues_manager's default current dialogue will be set to head
    assert(test_dialogues_manager.get_current()->id == "head");
    // test_dialogues_manager's head should have sound effects
    assert(test_dialogues_manager.get_current()->sound_effects.size() == 1);
    assert(test_dialogues_manager.get_current()->sound_effects[0] == "intro_chime.wav");
    // test_dialogues_manager's head should have a single target next
    assert(test_dialogues_manager.get_current()->has_next());
    assert(test_dialogues_manager.get_current()->next.has_single_target());
    // test_dialogues_manager's head should have ~01 as the next
    assert(test_dialogues_manager.get_current()->next.get_target() == "~01");
    // test next method
    test_dialogues_manager.next();
    assert(test_dialogues_manager.get_current()->id == "~01");
    assert(test_dialogues_manager.get_current()->has_previous());
    assert(test_dialogues_manager.get_current()->previous == "head");
    // sound effects should be cleared after each dialogue
    assert(test_dialogues_manager.get_current()->sound_effects.empty());
    // test previous method
    test_dialogues_manager.previous();
    assert(test_dialogues_manager.get_current()->id == "head");
    assert(!test_dialogues_manager.get_current()->has_previous());
    assert(test_dialogues_manager.get_current()->has_next());
    assert(test_dialogues_manager.get_current()->next.has_single_target());
    assert(test_dialogues_manager.get_current()->next.get_target() == "~01");
    // test multiple sound effects on ~03 (Test Character 1's dialogue after scene change)
    test_dialogues_manager.set_current_dialogue_id("~03");
    assert(test_dialogues_manager.get_current()->sound_effects.size() == 2);
    assert(test_dialogues_manager.get_current()->sound_effects[0] == "footsteps.wav");
    assert(test_dialogues_manager.get_current()->sound_effects[1] == "door_open.wav");
    // assume branches are compiled correctly
    test_dialogues_manager.set_current_dialogue_id("branch_choices");
    assert(test_dialogues_manager.get_current()->has_next());
    assert(test_dialogues_manager.get_current()->next.has_multi_targets());
    assert(test_dialogues_manager.get_current()->next.get_targets().size() == 2);
    // branch_choices should not have sound effects (they were only for ~03)
    assert(test_dialogues_manager.get_current()->sound_effects.empty());
    // Test that branches maintain correct previous dialogue references
    // Both "last_one" and "jumping_point1" should track back to "branch_choices"
    test_dialogues_manager.set_current_dialogue_id("last_one");
    assert(test_dialogues_manager.get_current()->previous == "branch_choices");
    test_dialogues_manager.set_current_dialogue_id("jumping_point1");
    assert(test_dialogues_manager.get_current()->previous == "branch_choices");
    test_dialogues_manager.next();
    assert(test_dialogues_manager.get_current()->previous == "jumping_point1");
    assert(!test_dialogues_manager.get_current()->has_next());
    // remove the output JSON file as it is no longer necessary
    std::filesystem::remove(jsonPath);

    // test folder compile
    const std::filesystem::path testFolderPath = EXAMPLE_VNS_TEST_FILES_DIR / "test_folder";
    const std::filesystem::path testChildFolderPath = testFolderPath / "child_folder";
    const std::filesystem::path testAnotherOutFolderPath = EXAMPLE_VNS_TEST_FILES_DIR / "test_out";
    // remove possible caches due to an error
    if (exists(testFolderPath))
        std::filesystem::remove_all(testFolderPath);
    if (exists(testAnotherOutFolderPath))
        std::filesystem::remove_all(testAnotherOutFolderPath);
    // create a new folder
    std::filesystem::create_directories(testChildFolderPath);
    // copy vns files
    std::filesystem::copy_file(EXAMPLE_VNS_TEST_FILE, testFolderPath / EXAMPLE_VNS_TEST_FILE.filename());
    std::filesystem::copy_file(EXAMPLE_VNS_TEST_FILE, testChildFolderPath / EXAMPLE_VNS_TEST_FILE.filename());
    // copy dummy files
    std::filesystem::copy_file(EXAMPLE_DUMMY_TEST_FILE, testFolderPath / EXAMPLE_DUMMY_TEST_FILE.filename());
    std::filesystem::copy_file(EXAMPLE_DUMMY_TEST_FILE, testChildFolderPath / EXAMPLE_DUMMY_TEST_FILE.filename());
    // try to compile the folder recursively
    Compiler::compile(testFolderPath);
    // make sure all vns files were compiled
    Validator::ensure(testFolderPath / "chapter1_dialogs_English.json");
    Validator::ensure(testChildFolderPath / "chapter1_dialogs_English.json");
    // once files existence check pass, remove the files before the next test
    std::filesystem::remove(testFolderPath / "chapter1_dialogs_English.json");
    std::filesystem::remove(testChildFolderPath / "chapter1_dialogs_English.json");

    // try to compile the scripts and save the result to another folder
    Compiler::compile(testFolderPath, testAnotherOutFolderPath);
    // make sure all vns files were compiled
    Validator::ensure(testAnotherOutFolderPath / "chapter1_dialogs_English.json");
    Validator::ensure(testAnotherOutFolderPath / testChildFolderPath.filename() / "chapter1_dialogs_English.json");
    // make sure the output is not saved to where it is not supposed to be saved
    assert(!exists(testFolderPath / "chapter1_dialogs_English.json"));
    assert(!exists(testChildFolderPath / "chapter1_dialogs_English.json"));
    // once files existence check pass, remove the files before the next test
    std::filesystem::remove_all(testAnotherOutFolderPath);

    // try to compile the folder recursively, with multi-threading
    Compiler::parallel_compile(testFolderPath);
    // make sure all vns files were compiled
    Validator::ensure(testFolderPath / "chapter1_dialogs_English.json");
    Validator::ensure(testChildFolderPath / "chapter1_dialogs_English.json");
    // once files existence check pass, remove the files before the next test
    std::filesystem::remove(testFolderPath / "chapter1_dialogs_English.json");
    std::filesystem::remove(testChildFolderPath / "chapter1_dialogs_English.json");

    // try to compile the scripts with multi-threading and save the result to another folder
    Compiler::parallel_compile(testFolderPath, testAnotherOutFolderPath);
    // make sure all vns files were compiled
    Validator::ensure(testAnotherOutFolderPath / "chapter1_dialogs_English.json");
    Validator::ensure(testAnotherOutFolderPath / testChildFolderPath.filename() / "chapter1_dialogs_English.json");
    // make sure output is not saved to where it is not supposed to be saved
    assert(!exists(testFolderPath / "chapter1_dialogs_English.json"));
    assert(!exists(testChildFolderPath / "chapter1_dialogs_English.json"));
    // once files existence check pass, remove the files before the next test
    std::filesystem::remove_all(testAnotherOutFolderPath);

    // remove test folder after finish testing
    std::filesystem::remove_all(testFolderPath);
}

void Tests::TestDialoguesManager()
{
    DialoguesManager test_dialogues_manager;
    // make sure the section has been init as an empty string
    assert(test_dialogues_manager.get_section().empty());
    // load test file
    test_dialogues_manager.load(EXAMPLE_VNS_TEST_FILE);
    // test_dialogues_manager's default section will be set to dialog_example
    assert(test_dialogues_manager.get_section() == "dialog_example");
    // test_dialogues_manager's default current dialogue will be set to head
    assert(test_dialogues_manager.get_current()->id == "head");
    // test set set_current_dialogue_id
    test_dialogues_manager.set_current_dialogue_id("~01");
    assert(test_dialogues_manager.get_current()->id == "~01");
    // set the section again and see whether the current dialogue id has been set back to head
    test_dialogues_manager.set_section("dialog_example");
    assert(test_dialogues_manager.get_current_dialogue_id() == "head");
    // test next
    test_dialogues_manager.next();
    assert(test_dialogues_manager.get_current()->id == "~01");
    // test events and variables
    assert(std::get<bool>(test_dialogues_manager.get_variable("section1_end")) == true);
    assert(test_dialogues_manager.get_variable<int>("chapter_passed") == 1);
    test_dialogues_manager.next();
    test_dialogues_manager.next();
    assert(test_dialogues_manager.get_current()->id == "~03");
    assert(test_dialogues_manager.get_variable<int>("chapter_passed") == 1);
    assert(test_dialogues_manager.get_variable<int>("mod_n") == 2);
    assert(test_dialogues_manager.get_variable<int>("test_multi") == 9);
    assert(test_dialogues_manager.get_variable<int>("mod_result") ==
           test_dialogues_manager.get_variable<int>("chapter_passed"));
    assert(test_dialogues_manager.get_current_section_dialogues().size() == 8);
    assert(test_dialogues_manager.get_current()->has_next() &&
           test_dialogues_manager.get_current()->next.has_single_target());
    assert(test_dialogues_manager.get_current()->next.has_single_target());
    assert(test_dialogues_manager.get_current()->next.get_target() == "branch_choices");
    // test remove section
    test_dialogues_manager.set_dialogues("test_remove_section", dialogue_section_t({{"head", {}}}));
    test_dialogues_manager.set_section("test_remove_section");
    test_dialogues_manager.remove_section("test_remove_section");
    test_dialogues_manager.set_section("dialog_example");
    assert(test_dialogues_manager.get_current()->id == "head");
    // test remove head
    assert(test_dialogues_manager.get_current()->has_next());
    assert(test_dialogues_manager.get_current()->next.has_single_target());
    assert(test_dialogues_manager.get_current()->next.get_target() == "~01");
    test_dialogues_manager.remove_current_dialogue();
    assert(!test_dialogues_manager.contains_dialogue(test_dialogues_manager.get_section(), "~01"));
    assert(test_dialogues_manager.get_current()->id == "head");
    assert(!test_dialogues_manager.get_current()->has_previous());
    assert(test_dialogues_manager.get_current()->has_next());
    assert(test_dialogues_manager.get_current()->next.has_single_target());
    assert(test_dialogues_manager.get_current()->next.get_target() == "~02");
    // check if the current head's next is correct
    test_dialogues_manager.next();
    assert(test_dialogues_manager.get_current()->id == "~02");
    assert(test_dialogues_manager.get_current()->previous == "head");
    assert(test_dialogues_manager.get_current()->next.has_single_target());
    assert(test_dialogues_manager.get_current()->next.get_target() == "~03");
    // remove ~02 and see whether everything is correct
    test_dialogues_manager.remove_current_dialogue();
    assert(!test_dialogues_manager.contains_dialogue(test_dialogues_manager.get_section(), "~02"));
    // not that current dialogue will be reset back to head
    assert(test_dialogues_manager.get_current()->id == "head");
    assert(!test_dialogues_manager.get_current()->has_previous());
    assert(test_dialogues_manager.get_current()->has_next());
    assert(test_dialogues_manager.get_current()->next.has_single_target());
    assert(test_dialogues_manager.get_current()->next.get_target() == "~03");
    // ~03 should be the new head.next
    test_dialogues_manager.next();
    assert(test_dialogues_manager.get_current()->id == "~03");
    assert(test_dialogues_manager.get_current()->previous == "head");
    assert(test_dialogues_manager.get_current()->next.has_single_target());
    assert(test_dialogues_manager.get_current()->next.get_target() == "branch_choices");
    // remove jumping_point1 and see if branch_choices got updated correctly
    test_dialogues_manager.remove_dialogue(test_dialogues_manager.get_section(), "jumping_point1");
    assert(!test_dialogues_manager.contains_dialogue(test_dialogues_manager.get_section(), "jumping_point1"));
    assert(test_dialogues_manager.get_current()->id == "~03");
    assert(test_dialogues_manager.get_current()->previous == "head");
    assert(test_dialogues_manager.get_current()->next.has_single_target());
    assert(test_dialogues_manager.get_current()->next.get_target() == "branch_choices");
    test_dialogues_manager.next();
    assert(test_dialogues_manager.get_current()->next.has_multi_targets());
    assert(!test_dialogues_manager.get_current()->next.contains_target("jumping_point1"));
}

void Tests::TestBranching()
{
    // try to compile the script
    // Compiler::compile(EXAMPLE_VNS_BRANCHES_TESTS_FILE);
    // init a manager for loading the script
    DialoguesManager test_dialogues_manager;
    // make sure the section has been init as an empty string
    assert(test_dialogues_manager.get_section().empty());
    // load test file
    test_dialogues_manager.load(EXAMPLE_VNS_BRANCHES_TESTS_FILE);
    // test_dialogues_manager's default section will be set to dialog_example
    assert(test_dialogues_manager.get_section() == "dialog_example");
    // test_dialogues_manager's default current dialogue will be set to head
    assert(test_dialogues_manager.get_current()->id == "head");
    // assert next is ~01
    assert(test_dialogues_manager.get_current()->has_next());
    assert(test_dialogues_manager.get_current()->next.has_single_target());
    assert(test_dialogues_manager.get_current()->next.get_target() == "~01");
    // go to next
    test_dialogues_manager.next();
    assert(test_dialogues_manager.get_current()->id == "~01");
    // make sure next is first_jump
    assert(test_dialogues_manager.get_current()->has_next());
    assert(test_dialogues_manager.get_current()->next.has_single_target());
    assert(test_dialogues_manager.get_current()->next.get_target() == "first_jump");
    // go to next - first_jump
    test_dialogues_manager.next();
    assert(test_dialogues_manager.get_current()->id == "first_jump");
    assert(test_dialogues_manager.get_current()->previous == "~01");
    // continue
    assert(test_dialogues_manager.get_current()->has_next());
    assert(test_dialogues_manager.get_current()->next.has_single_target());
    test_dialogues_manager.next();
    assert(test_dialogues_manager.get_current()->has_previous());
    assert(test_dialogues_manager.get_current()->previous == "first_jump");
    assert(test_dialogues_manager.get_current()->has_next());
    assert(test_dialogues_manager.get_current()->next.has_single_target());
    test_dialogues_manager.next();
    // make sure next is no_prev_jump
    assert(test_dialogues_manager.get_current()->has_next());
    assert(test_dialogues_manager.get_current()->next.has_single_target());
    assert(test_dialogues_manager.get_current()->next.get_target() == "no_prev_jump");
    // now we are at no_prev_jump
    test_dialogues_manager.next();
    assert(test_dialogues_manager.get_current()->id == "no_prev_jump");
    assert(!test_dialogues_manager.get_current()->has_previous());
    assert(test_dialogues_manager.get_current()->has_next());
    // jump into jump_prev
    test_dialogues_manager.next();
    assert(test_dialogues_manager.get_current()->id == "jump_prev");
    assert(test_dialogues_manager.get_current()->has_previous());
    assert(test_dialogues_manager.get_current()->has_next());
    // jump into the_end
    test_dialogues_manager.next();
    assert(test_dialogues_manager.get_current()->id == "the_end");
    assert(!test_dialogues_manager.get_current()->has_previous());
    assert(!test_dialogues_manager.get_current()->has_next());
    // check ~02, which is a void
    test_dialogues_manager.set_current_dialogue_id("~02");
    assert(test_dialogues_manager.get_current()->id == "~02");
    assert(!test_dialogues_manager.get_current()->has_previous());
    assert(!test_dialogues_manager.get_current()->has_next());
}

void Tests::TestAll()
{
    std::cout << "- Test basic functions\n";
    TestBasicFunctions();
    std::cout << "- Test naming and tags\n";
    TestNameWithoutTag();
    TestNameWithTags();
    std::cout << "- Test empty next\n";
    TestNullNext();
    std::cout << "- Test not empty single target next\n";
    TestSingleTargetNext();
    std::cout << "- Test not empty multiple targets next\n";
    TestMultiTargetsNext();
    std::cout << "- Test script processor\n";
    TestScriptProcessor();
    std::cout << "- Test compiler\n";
    TestCompiler();
    std::cout << "- Test branching\n";
    TestBranching();
    std::cout << "- Test Dialogues Manager\n";
    TestDialoguesManager();
    std::cout << "> Done\n";
}
