#include "tests.hpp"
#include <cassert>
#include "dialogueNext.hpp"
#include "naming.hpp"
#include <iostream>
#include "scriptProcessor.hpp"
#include "compiler.hpp"

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
    const DialogueNext single_target_next({{"type",   "default"},
                                           {"target", "~1"}});
    assert(!single_target_next.is_null());
    assert(!single_target_next.has_multi_targets());
    assert(single_target_next.has_type("default"));
    assert(single_target_next.get_target() == "~1");
}

void TestMultiTargetsNext()
{
    const std::unordered_map<std::string, std::string> t1 = {{"hello1", "world1"}};
    const std::unordered_map<std::string, std::string> t2 = {{"hello2", "world2"}};
    const MultiTargetsType target_v = {t1, t2};
    const DialogueNext multi_targets_next({{"type",   "default"},
                                           {"target", target_v}});
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
    Compiler::compile(EXAMPLE_VNS_TEST_FILE, EXAMPLE_VNS_TEST_FILE_OUTPUT_DIR);
}

void TestDialoguesManager()
{
    DialoguesManager test_dialogues_manager;
    test_dialogues_manager.load(EXAMPLE_VNS_TEST_FILE);
    test_dialogues_manager.set_section("dialog_example");
    test_dialogues_manager.next();
    assert(test_dialogues_manager.get_current()->id == "~01");
    assert(std::get<bool>(test_dialogues_manager.get_variable("section1_end")) == true);
    assert(test_dialogues_manager.get_variable<int>("chapter_passed") == 1);
    test_dialogues_manager.next();
    test_dialogues_manager.next();
    assert(test_dialogues_manager.get_current()->id == "~03");
    assert(test_dialogues_manager.get_variable<int>("chapter_passed") == 1);
    assert(test_dialogues_manager.get_variable<int>("mod_n") == 2);
    assert(test_dialogues_manager.get_variable<int>("test_multi") == 8);
    assert(test_dialogues_manager.get_variable<int>("mod_result") ==
           test_dialogues_manager.get_variable<int>("chapter_passed"));
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
    // std::cout << "- Test compiler\n";
    // TestCompiler();
    std::cout << "- Test Dialogues Manager\n";
    TestDialoguesManager();
    std::cout << "> Done\n";
}
