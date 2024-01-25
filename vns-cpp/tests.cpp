#include "tests.h"
#include <cassert>
#include "contentNext.h"
#include "naming.h"
#include <iostream>
#include "processor.h"
#include "compiler.h"

void TestNameWithoutTag()
{
	const std::string test_name_str = "maria.png";
	const Naming test_name(test_name_str);
	assert(test_name.GetName() == test_name_str);
	assert(test_name.GetTags().empty());
	assert(test_name.ToString() == test_name_str);
}

void TestNameWithTags()
{
	const std::string test_name_str = "maria.png&silent&hide";
	const std::string test_name_img = "maria.png";
	const Naming test_name(test_name_str);
	assert(test_name.GetName() == test_name_img);
	const std::unordered_set<std::string> expected_tags = {"silent", "hide"};
	assert(test_name.GetTags() == expected_tags);
	assert(test_name.ToString() == test_name_str);
	assert(test_name.Equal(test_name_img));
	assert(test_name.Equal(Naming(test_name_img)));
}

void TestNullNext()
{
	const ContentNext test_null_next;
	assert(test_null_next.is_null());
	assert(!test_null_next.has_multi_targets());
	assert(test_null_next.get_type() == "null");
	assert(test_null_next.get_target().empty());
}

void TestSingleTargetNext()
{
	const ContentNext single_target_next({{"type", "default"}, {"target", "~1"}});
	assert(!single_target_next.is_null());
	assert(!single_target_next.has_multi_targets());
	assert(single_target_next.get_type() == "default");
	assert(single_target_next.get_target() == "~1");
}

void TestMultiTargetsNext()
{
	const std::unordered_map<std::string, std::string> t1 = {{"hello1", "world1"}};
	const std::unordered_map<std::string, std::string> t2 = {{"hello2", "world2"}};
	const MultiTargetsType target_v = {t1, t2};
	const ContentNext multi_targets_next({{"type", "default"}, {"target", target_v}});
	assert(!multi_targets_next.is_null());
	assert(multi_targets_next.has_multi_targets());
	assert(multi_targets_next.get_type() == "default");
	assert(multi_targets_next.get_targets().size() == 2);
}

void TestScriptProcessor()
{
	auto test_processor = Processor();
	test_processor.process("C:/Users/yudon/Documents/GitHub/vns/examples/chapter_example.vns");
	assert(test_processor.get_id() == "1");
	assert(test_processor.get_language() == "English");
}

void TestCompiler()
{
	Compiler::compile("C:/Users/yudon/Documents/GitHub/vns/examples/chapter_example.vns",
	                  "C:/Users/yudon/Documents/GitHub/vns/examples/out");
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
	std::cout << "> Done\n";
}
