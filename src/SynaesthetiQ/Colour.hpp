#ifndef COLOUR_HPP
#define COLOUR_HPP

#include <stdint.h>

/**
 * @brief A class to manage colours for the SynaesthetiQ
 *
 * Provides an abstraction on top of the ws2811 LED colour, that allows easy conversion between colour formats
 */
class Colour {
    private:

    uint8_t red;
    uint8_t green;
    uint8_t blue;

    public:

    /**
     * @brief Default constructor, which set all values to 0
     */
    Colour() : red(0), green(0), blue(0){
    }

    /**
     * @brief Constructor that takes in red, green, blue values separately
     */
    Colour(uint8_t redIn, uint8_t greenIn, uint8_t blueIn) : red(redIn), green(greenIn), blue(blueIn) {
    }

    /**
     * @brief Constructor that takes in a 32 bit number of form XRGB, where X is an empty byte
     */
    Colour(uint32_t XRGBIn){
        this->red = (uint8_t)((0x00ff0000 & XRGBIn)>>16);
        this->green = (uint8_t)((0x0000ff00 & XRGBIn)>>8);
        this->blue = (uint8_t)((0x000000ff & XRGBIn));
    }

    /**
     * @brief Returns 32 bit-format of order Green, Red, Blue
     */
    uint32_t getGRB() {
        return (uint32_t) (( (((uint32_t)green)<<8) | ((uint32_t)red)) << 8 ) | ((uint32_t)blue);
    }

    /**
     * @brief Returns 32 bit-format of order Red, Green, Blue
     */
    uint32_t getRGB() {
        return (uint32_t) (( (((uint32_t)red)<<8) | ((uint32_t)green)) << 8 ) | ((uint32_t)blue);
    }

    /**
     * @brief Returns red channel
     */
    uint8_t getRed(){
        return red;
    }

    /**
     * @brief Returns green channel
     */
    uint8_t getGreen(){
        return green;
    }

    /**
     * @brief Returns blue channel
     */
    uint8_t getBlue(){
        return blue;
    }

    /**
     * @brief Sets red channel
     */
    void setRed(uint8_t redIn){
        red = redIn;
    }

    /**
     * @brief Sets green channel
     */
    void setGreen(uint8_t greenIn){
        green = greenIn;
    }

    /**
     * @brief Sets blue channel
     */
    void setBlue(uint8_t blueIn){
        blue = blueIn;
    }

};

#endif