#include <string>
#include "../ws2811/ws2811.h"
#include "Colour.hpp"

// #define TARGET_FREQ             WS2811_TARGET_FREQ
#define GPIO_PIN                10
#define DMA                     10
#define STRIP_TYPE              WS2811_STRIP_RGB		// WS2812/SK6812RGB integrated chip+leds
#define LED_COUNT               513
#define colourOff               0x00000000

struct XYPos
{
    int x;
    int y;
};

typedef double AMPS;
typedef double MILLIAMPS;

/**
 * @brief A class to manage the SynaesthetiQ and create visualisations
 */
class SynaesthetiQ {            // The class
    private:    // Access specifier
        double        maxMatrixCurrent = 10.0;   // Max Matrix Current expressed as amps.
        const int     matrixPixels     = 512;    // The Number of pixels in the matrix.
        double        matrixPixelCurrentPerChannel = 19.53;
        double        maxBigLEDCurrent = 3.0;    // Max Big LED Current expressed as amps.
        const int     bigLEDCount      = 1;      // Must be one.
        bool          bigLEDFirst      = false;

        double  systemBrightness = 50.0;     // System Brightness expressed as a percentage. Default 50%
        ws2811_t ledstring;
        ws2811_return_t ret;
        bool clear_on_exit = true;
        Colour bigLEDColour;           // Big LED Colour 0x00FF0000 R 0x0000FF00 G 0x000000FF B
        Colour basicMatrixLimit(Colour ColourIn);
        Colour bigLEDLimit(Colour colourIn);

        void limitMatrixCurrent();
        AMPS calculateMatrixCurrent();        // Matrix Current in amps.
        MILLIAMPS calculateLEDCurrent(MILLIAMPS LEDMaxCurrentPerChannel,Colour colour);           // LED Current in milli amps
        void applyFactorToMatrix(double factor);
        Colour applyFactorToLED(double factor, Colour colour);

        int XYtoChainPos(XYPos XY);
        XYPos ChainPostoXY(int ChainPos);
    public:

        /**
         * @brief Sets brightness of the SynaesthetiQ
         */
        void setSystemBrightness(double Brightness) {
            systemBrightness = Brightness;
        };

        /**
         * @brief Gets brightness of the SynaesthetiQ
         */
        double getSystemBrightness() {
            return systemBrightness;
        }
        SynaesthetiQ(); 
        ~SynaesthetiQ();

        /**
         * @brief Sets the colour of the top LED
         */
        void setBigLEDColour(Colour colour);

        /**
         * @brief Sets the colour of the matrix
         */
        void setMatrixColour(Colour colour);

        /**
         * @brief Sets the colour of an individual LED on the matrix using x, y coordinates
         * 
         * Coordinates range from 0<=x<32 and 0<=y<16
         */
        void setMatrixPixelColour(int x,int y,Colour colour);

        /**
         * @brief Turns off all the SynaesthetiQ LEDs
         */
        void clearOutput();

        /**
         * @brief Updates the SynasthetiQ with the colours set, while limiting current
         */
        int render();
};