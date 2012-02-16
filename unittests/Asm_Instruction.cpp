#include "gtest/gtest.h"

#include <string>
#include "assembler/Instruction.h"
#include "assembler/Argument.h"
#include "common/Opcode.h"
#include "common/Registers.h"

/*
int main(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
*/


TEST(Argument, HasConstant) {
	std::string s("1");
	Argument arg(s);
	ASSERT_TRUE(arg.HasConstant());

	s = "ebx";
	arg.Init(s);
	ASSERT_FALSE(arg.HasConstant());

	s = "0xABCD";
	arg.Init(s);
	ASSERT_TRUE(arg.HasConstant());

	s = "[ebx + 1]";
	arg.Init(s);
	ASSERT_TRUE(arg.HasConstant());

	s = "[10]";
	arg.Init(s);
	ASSERT_TRUE(arg.HasConstant());

	s = "[ebx + eax]";
	arg.Init(s);
	ASSERT_FALSE(arg.HasConstant());

	s = "[10 + eax]";
	arg.Init(s);
	ASSERT_TRUE(arg.HasConstant());

	s = "@data";
	arg.Init(s);
	ASSERT_TRUE(arg.HasConstant());

	s = ":label";
	arg.Init(s);
	ASSERT_TRUE(arg.HasConstant());

	s = "error";
	arg.Init(s);
	ASSERT_FALSE(arg.HasConstant());

}

TEST(Argument, OtherStuff)
{
	std::string s("1");
	Argument arg(s);
	ASSERT_FALSE(arg.HasData());
	ASSERT_FALSE(arg.NeedsLabel());
	ASSERT_TRUE(arg.IsValid());
	ASSERT_EQ(SC_CONST, arg.GetType());
	ASSERT_EQ(Argument::ARG_NONE, arg.GetInternalType());
	ASSERT_EQ("1", arg.GetText());
	ASSERT_EQ(1, arg.GetVal());
	s = "ebx";
	arg.Init(s);
	ASSERT_FALSE(arg.HasData());
}
