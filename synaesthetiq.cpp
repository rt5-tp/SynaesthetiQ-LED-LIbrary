#include <string>
#include "ws2811.h"

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
        int clear_on_exit = 1;
        ws2811_led_t bigLEDColour;           // Big LED Colour 0x00FF0000 R 0x0000FF00 G 0x000000FF B
        ws2811_led_t RGBToBGR(ws2811_led_t ColourIn);
        ws2811_led_t basicMatrixLimit(ws2811_led_t ColourIn);
        ws2811_led_t bigLEDLimit(ws2811_led_t ColourIn);

        void limitMatrixCurrent();
        AMPS calculateMatrixCurrent();        // Matrix Current in amps.
        MILLIAMPS calculateLEDCurrent(MILLIAMPS LEDMaxCurrentPerChannel,ws2811_led_t colour);           // LED Current in milli amps
        void applyFactorToMatrix(double factor);
        ws2811_led_t applyFactorToLED(double factor, ws2811_led_t colour);

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
        ws2811_led_t getColour(uint8_t r,uint8_t g, uint8_t b);
        void setBigLEDColour(ws2811_led_t Colour);
        void setMatrixColour(ws2811_led_t Colour);
        void setMatrixPixelColour(int x,int y,int Colour);
        // void setMatrix();           // Takes a matrix format as input. Uses an assosiative array under the hood.
        void clearOutput();
        ws2811_return_t render();
};

ws2811_led_t SynaesthetiQ::RGBToBGR(ws2811_led_t ColourIn) {
	ws2811_led_t outColour = 0x00000000;

	int r = (0x00ff0000 & ColourIn)>>16;
	int g = (0x0000ff00 & ColourIn)>>8;
	int b = (0x000000ff & ColourIn);

	outColour = (( (g<<8) | r) << 8 ) | b;

	return outColour;
}

ws2811_led_t SynaesthetiQ::bigLEDLimit(ws2811_led_t ColourIn) { 
    return ColourIn;
}

ws2811_led_t SynaesthetiQ::basicMatrixLimit(ws2811_led_t ColourIn) {
	int r = (0x00ff0000 & ColourIn)>>16;
    int g = (0x0000ff00 & ColourIn)>>8;
    int b = (0x000000ff & ColourIn);

	double currentPerColour = 0.02;

	double rp = (((double) r)/255)*currentPerColour;
	double gp = (((double) g)/255)*currentPerColour;
	double bp = (((double) b)/255)*currentPerColour;

	printf("%f %f %f \n",rp,gp,bp);

	double tp = rp+gp+bp;

	printf("%f\n",tp);

	double maxCurrentPerChip = 0.03;

	if (tp > maxCurrentPerChip) {
		double factor = (maxCurrentPerChip/tp);

		int ro = (int) ((float) r*factor);
		int go = (int) ((float) g*factor);
		int bo = (int) ((float) b*factor);

		printf("%f\n", factor);

		return (( (ro<<8) | go) << 8 ) | bo;
	}
	return ColourIn;
}

SynaesthetiQ::SynaesthetiQ(){
    ledstring = {
        .freq = WS2811_TARGET_FREQ,
        .dmanum = DMA,
        .channel =
        {
            [0] =
            {
                .gpionum = GPIO_PIN,
                .invert = 0,
                .count = matrixPixels+bigLEDCount,
                .strip_type = STRIP_TYPE,
                .brightness = 255,
            },
            [1] =
            {
                .gpionum = 0,
                .invert = 0,
                .count = 0,
                .brightness = 0,
            },
        },
    };
    if ((ret = ws2811_init(&ledstring)) != WS2811_SUCCESS)
    {
        fprintf(stderr, "ws2811_init failed: %s\n", ws2811_get_return_t_str(ret));
        exit(ret);
    }
    // rawLEDs = &ledstring.channel[0];
}

SynaesthetiQ::~SynaesthetiQ() {
    if (clear_on_exit) {
        clearOutput();
    }
    ws2811_fini(&ledstring);
};

void SynaesthetiQ::setBigLEDColour(ws2811_led_t Colour) {
    bigLEDColour = bigLEDLimit(Colour);
};

void SynaesthetiQ::setMatrixColour(ws2811_led_t Colour) {
    int start;
    int end;

    if (bigLEDFirst) {
        start = bigLEDCount;
        end = matrixPixels+bigLEDCount;
    } else {
        start = 0;
        end = matrixPixels;
    }

    for (int i = start; i < end; i++) {
        ledstring.channel[0].leds[i] = RGBToBGR(Colour);
    }
};

void SynaesthetiQ::setMatrixPixelColour(int x,int y,int Colour) {
    int matrixStart;
    if (bigLEDFirst) {
        matrixStart = bigLEDCount;
    } else {
        matrixStart = 0;
    }
    int chainPos = matrixStart + XYtoChainPos(XYPos{x,y});

    ledstring.channel[0].leds[chainPos] = RGBToBGR(Colour);
};

void SynaesthetiQ::clearOutput() {
    ws2811_led_t Colour = colourOff;

    bigLEDColour = Colour;
    setMatrixColour(Colour);
    
    render();
}

void SynaesthetiQ::limitMatrixCurrent() {
    double current = calculateMatrixCurrent();
    if (current > maxMatrixCurrent) {
        double factor = maxMatrixCurrent/current;
        applyFactorToMatrix(factor);
    }
};

AMPS SynaesthetiQ::calculateMatrixCurrent() {
    int start;
    int end;

    if (bigLEDFirst) {
        start = bigLEDCount;
        end = matrixPixels+bigLEDCount;
    } else {
        start = 0;
        end = matrixPixels;
    }

    MILLIAMPS current;

    for (int i = start; i < end; i++) {
        current += calculateLEDCurrent(matrixPixelCurrentPerChannel,ledstring.channel[0].leds[i]);
    }

    return current/1000;
};

MILLIAMPS SynaesthetiQ::calculateLEDCurrent(MILLIAMPS LEDMaxCurrentPerChannel,ws2811_led_t colour) {
    uint8_t r = (0x00ff0000 & colour)>>16;
    uint8_t g = (0x0000ff00 & colour)>>8;
    uint8_t b = (0x000000ff & colour);

    MILLIAMPS current = 0;

    current += ((double) r/255)*LEDMaxCurrentPerChannel;
    current += ((double) g/255)*LEDMaxCurrentPerChannel;
    current += ((double) b/255)*LEDMaxCurrentPerChannel;

    return current;
}

ws2811_led_t SynaesthetiQ::applyFactorToLED(double factor, ws2811_led_t colour) {
    uint8_t r = (0x00ff0000 & colour)>>16;
    uint8_t g = (0x0000ff00 & colour)>>8;
    uint8_t b = (0x000000ff & colour);

    uint8_t ro = (uint8_t) ((float) r*factor);
    uint8_t go = (uint8_t) ((float) g*factor);
    uint8_t bo = (uint8_t) ((float) b*factor);

    return (( (ro<<8) | go) << 8 ) | bo;
}

void SynaesthetiQ::applyFactorToMatrix(double factor) {
    int start;
    int end;

    if (bigLEDFirst) {
        start = bigLEDCount;
        end = matrixPixels+bigLEDCount;
    } else {
        start = 0;
        end = matrixPixels;
    }

    for (int i = start; i < end; i++) {
        ledstring.channel[0].leds[i] = applyFactorToLED(factor,ledstring.channel[0].leds[i]);
    }
};

int XYtoChainPos(XYPos XY) {
    int x = XY.x;
    int y = XY.y;
    int pos = 0;
    
    int matrixOffset = 0 + (256*(y<8));

    pos += matrixOffset;
    int inverseX = (31-x);
    pos += inverseX*8;
    int usefulY = y % 8;
    int invusefulY = 7-usefulY;
    int oddCol = inverseX%2;
    pos += usefulY*oddCol + invusefulY*(! oddCol);
    return pos;
};

XYPos ChainPostoXY(int ChainPos) {
    int x = 0;
    int y = 0;
    
    y += 8*(ChainPos<256);
    ChainPos = ChainPos-256*(ChainPos>=256);

    int yRel = ChainPos % 8;
    int inverseX = ChainPos; // 8
    x = (31-inverseX);
    int oddCol = inverseX%2;
    int invYRel = (7-yRel);
    y = y + (yRel*oddCol+ invYRel*(! oddCol));
    return XYPos{x,y};
};

ws2811_led_t SynaesthetiQ::getColour(uint8_t r,uint8_t g, uint8_t b) {
    return (( (r<<8) | g) << 8 ) | b;
};

ws2811_return_t SynaesthetiQ::render() {
    if (bigLEDFirst) {
        for (int i = 0; i < bigLEDCount; i++) {
            ledstring.channel[0].leds[i] = bigLEDColour;
        }
    } else {
        for (int i = matrixPixels; i < matrixPixels+bigLEDCount; i++) {
            ledstring.channel[0].leds[i] = bigLEDColour;
        }
    }

    limitMatrixCurrent();
    
    if ((ret = ws2811_render(&ledstring)) != WS2811_SUCCESS)
	{
	    fprintf(stderr, "ws2811_render failed: %s\n", ws2811_get_return_t_str(ret));
	    return ret;
	}
};