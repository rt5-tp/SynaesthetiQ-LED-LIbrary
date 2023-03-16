#include <string>
#include "ws2811.h"
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

class SynaesthetiQ {            // The class
    private:    // Access specifier
        double        maxMatrixCurrent = 10.0;   // Max Matrix Current expressed as amps.
        const int     matrixPixels     = 512;    // The Number of pixels in the matrix.
        double        matrixPixelCurrentPerChannel = 19.53;
        double        maxBigLEDCurrent = 3.0;    // Max Big LED Current expressed as amps.
        const int     bigLEDCount      = 1;      // Must be one.
        bool          bigLEDFirst      = true;

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
        void setSystemBrightness(double Brightness) {
            systemBrightness = Brightness;
        };
        double getSystemBrightness() {
            return systemBrightness;
        }
        SynaesthetiQ(); 
        ~SynaesthetiQ();
        void setBigLEDColour(Colour colour);
        void setMatrixColour(Colour colour);
        void setMatrixPixelColour(int x,int y,Colour colour);
        // void setMatrix();           // Takes a matrix format as input. Uses an assosiative array under the hood.
        void clearOutput();
        ws2811_return_t render();
};