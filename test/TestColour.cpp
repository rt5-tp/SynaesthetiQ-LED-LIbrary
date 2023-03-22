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

// Tests the returned GRB.
TEST(getRGBTest, ReturnsCorrectGRB){
    Colour colour(0x000e53b8);
    EXPECT_EQ(colour.getGRB(), 0x00530eb8);
}

// Tests the returned RGB.
TEST(getRGBTest, ReturnsCorrectRGB){
    Colour colour(0x000e53b8);
    EXPECT_EQ(colour.getRGB(), 0x000e53b8);
}

TEST(getRGBTest, ConstructorRGBWorks){

    Colour colour(0x000e35b8);
    EXPECT_EQ(colour.getRed(), 14);
    EXPECT_EQ(colour.getGreen(), 53);
    EXPECT_EQ(colour.getBlue(), 184);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

