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
        this->red = (uint8_t)((0x00ff0000 & XRGBIn)>>16);
        this->green = (uint8_t)((0x0000ff00 & XRGBIn)>>8);
        this->blue = (uint8_t)((0x000000ff & XRGBIn));
    }

    uint32_t getGRB() {
        return (uint32_t) (( (this->green<<8) | this->red) << 8 ) | this->blue;
    }

    uint32_t getRGB() {
        return (uint32_t) (( (this->red<<8) | this->green) << 8 ) | this->blue;
    }

    uint8_t getRed(){
        return this->red;
    }

    uint8_t getGreen(){
        return this->green;
    }

    uint8_t getBlue(){
        return this->blue;
    }

    void setRed(uint8_t redIn){
        this->red = redIn;
    }

    void setGreen(uint8_t greenIn){
        this->green = greenIn;
    }

    void setBlue(uint8_t blueIn){
        this->blue = blueIn;
    }

};

#endif