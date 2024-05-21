#include "tests.hpp"
#include <cassert>
#include "dialogueNext.hpp"
#include "naming.hpp"
#include <iostream>
#include "scriptProcessor.hpp"
#include "compiler.hpp"
#include "validator.hpp"

void TestNameWithoutTag()
{
    const std::string test_name_str = "maria.png";
    const Naming test_name(test_name_str);
    assert(test_name.get_name() == test_name_str);
    assert(test_name.get_tags().empty());
    assert(!test_name.contains_tag("silent"));
    assert(test_name.to_string() == test_name_str);
}

void TestNameWithTags()
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

void TestNullNext()
{
    const DialogueNext test_null_next;
    assert(test_null_next.is_null());
    assert(!test_null_next.has_multi_targets());
    assert(test_null_next.has_type("default"));
    assert(test_null_next.get_target().empty());
}

void TestSingleTargetNext()
{
    const DialogueNext single_target_next(dialogue_next_t({{"type",   "default"},
                                                           {"target", "~1"}}));
    assert(!single_target_next.is_null());
    assert(!single_target_next.has_multi_targets());
    assert(single_target_next.has_type("default"));
    assert(single_target_next.get_target() == "~1");
}

void TestMultiTargetsNext()
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

void TestScriptProcessor()
{
    ScriptProcessor test_processor;
    test_processor.process(EXAMPLE_VNS_TEST_FILE);
    assert(test_processor.get_id() == "1");
    assert(test_processor.get_language() == "English");
}

void TestCompiler()
{
    // compile file
    Compiler::compile(EXAMPLE_VNS_TEST_FILE, EXAMPLE_VNS_TEST_FILE_OUTPUT_DIR);
    // make sure output file exists
    std::filesystem::path outFileName = "chapter1_dialogs_English.json";
    std::filesystem::path jsonPath = EXAMPLE_VNS_TEST_FILE_OUTPUT_DIR / outFileName;
    assert(std::filesystem::exists(jsonPath));
    // make sure file is in correct format
    Validator::ensure(jsonPath);
    // try load json file
    DialoguesManager test_dialogues_manager;
    test_dialogues_manager.load(jsonPath);
    // test_dialogues_manager's default section will be set to dialog_example
    assert(test_dialogues_manager.get_section() == "dialog_example");
    // test_dialogues_manager's default current dialogue will be set to head
    assert(test_dialogues_manager.get_current()->id == "head");
    // test_dialogues_manager's head should have single target next
    assert(test_dialogues_manager.get_current()->has_next());
    assert(test_dialogues_manager.get_current()->next.has_single_target());
    // test_dialogues_manager's head should have ~01 as the next
    assert(test_dialogues_manager.get_current()->next.get_target() == "~01");
    // remove output json file as it is no longer needed
    std::filesystem::remove(jsonPath);
}

void TestDialoguesManager()
{
    DialoguesManager test_dialogues_manager;
    // make sure section has been init as empty string
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
    // set section again and see whether current dialogue id has been set back to head
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
    assert(test_dialogues_manager.get_current()->next.get_target() == "~04");
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
    assert(test_dialogues_manager.get_current()->previous.empty());
    assert(test_dialogues_manager.get_current()->has_next());
    assert(test_dialogues_manager.get_current()->next.has_single_target());
    assert(test_dialogues_manager.get_current()->next.get_target() == "~02");
    // check if current head's next is correct
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
    assert(test_dialogues_manager.get_current()->previous.empty());
    assert(test_dialogues_manager.get_current()->has_next());
    assert(test_dialogues_manager.get_current()->next.has_single_target());
    assert(test_dialogues_manager.get_current()->next.get_target() == "~03");
    // ~03 should be the new head.next
    test_dialogues_manager.next();
    assert(test_dialogues_manager.get_current()->id == "~03");
    assert(test_dialogues_manager.get_current()->previous == "head");
    assert(test_dialogues_manager.get_current()->next.has_single_target());
    assert(test_dialogues_manager.get_current()->next.get_target() == "~04");
    // remove jumping_point1 and see if ~04 got updated correctly
    test_dialogues_manager.remove_dialogue(test_dialogues_manager.get_section(), "jumping_point1");
    assert(!test_dialogues_manager.contains_dialogue(test_dialogues_manager.get_section(), "jumping_point1"));
    assert(test_dialogues_manager.get_current()->id == "~03");
    assert(test_dialogues_manager.get_current()->previous == "head");
    assert(test_dialogues_manager.get_current()->next.has_single_target());
    assert(test_dialogues_manager.get_current()->next.get_target() == "~04");
    test_dialogues_manager.next();
    assert(test_dialogues_manager.get_current()->next.has_multi_targets());
    assert(!test_dialogues_manager.get_current()->next.contains_target("jumping_point1"));
}

void TestAll()
{
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
    std::cout << "- Test Dialogues Manager\n";
    TestDialoguesManager();
    std::cout << "> Done\n";
}
