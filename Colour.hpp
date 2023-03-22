#ifndef COLOUR_HPP
#define COLOUR_HPP

#include <stdint.h>

class Colour {
    private:

    uint8_t red;
    uint8_t green;
    uint8_t blue;

    public:

    // default constructor sets all values to 0
    Colour() : red(0), green(0), blue(0){
    }

    Colour(uint8_t redIn, uint8_t greenIn, uint8_t blueIn) : red(redIn), green(greenIn), blue(blueIn) {
    }

    // takes in a 32 bit number of form XRGB, where X is an empty byte
    Colour(uint32_t XRGBIn){
        red = (uint8_t)((0x00ff0000 & XRGBIn)>>16);
        green = (uint8_t)((0x0000ff00 & XRGBIn)>>8);
        blue = (uint8_t)(0x000000ff & XRGBIn);
    }

    uint32_t getGRB() {
        return (uint32_t) (( (((uint32_t)green)<<8) | ((uint32_t)red)) << 8 ) | ((uint32_t)blue);
    }

    uint32_t getRGB() {
        return (uint32_t) (( (((uint32_t)red)<<8) | ((uint32_t)green)) << 8 ) | ((uint32_t)blue);
    }

    uint8_t getRed(){
        return red;
    }

    uint8_t getGreen(){
        return green;
    }

    uint8_t getBlue(){
        return blue;
    }

    void setRed(uint8_t redIn){
        red = redIn;
    }

    void setGreen(uint8_t greenIn){
        green = greenIn;
    }

    void setBlue(uint8_t blueIn){
        blue = blueIn;
    }

};

#endif