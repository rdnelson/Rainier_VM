#include "gtest/gtest.h"

#include "assembler/Instruction.h"
#include "assembler/Argument.h"

/*
int main(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
*/


TEST(Trial, Test1) {
	Argument("1");
	ASSERT_EQ(1, 1);
}
