#include "../Colour.hpp"
#include <gtest/gtest.h>

// Tests zero-input.
TEST(getRGBTest, ReturnsConstructorValues){

    uint8_t red = 14;
    uint8_t green = 53;
    uint8_t blue = 184;

    Colour colour(red, green, blue);
    
    EXPECT_EQ(colour.getRed(), red);
    EXPECT_EQ(colour.getGreen(), green);
    EXPECT_EQ(colour.getBlue(), blue);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

