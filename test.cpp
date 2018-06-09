#include <gtest/gtest.h>
#include "ArgumentList.h"



// Tests factorial of positive numbers.
TEST(ArgumentList, ArgumentsWorkProperly) {
    Config::ArgumentList opts(2, new char*[2]{
        "hello",
        "world"
    });
    bool valid;
    auto err = opts.OptionsAreValid(valid);
    EXPECT_TRUE(valid);
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}