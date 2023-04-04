#include "synaesthetiq.hpp"
#include "Colour.hpp"
#include <vector>
#include <iostream>

Colour SynaesthetiQ::bigLEDLimit(Colour colourIn) { 
    return colourIn;
}

Colour SynaesthetiQ::basicMatrixLimit(Colour colourIn) {

	double currentPerColour = 0.02;

	double rp = (((double) colourIn.getRed())/255)*currentPerColour;
	double gp = (((double) colourIn.getGreen())/255)*currentPerColour;
	double bp = (((double) colourIn.getBlue())/255)*currentPerColour;

	printf("%f %f %f \n",rp,gp,bp);

	double tp = rp+gp+bp;

	printf("%f\n",tp);

	double maxCurrentPerChip = 0.03;

	if (tp > maxCurrentPerChip) {
		double factor = (maxCurrentPerChip/tp);

		int ro = (int) ((float) colourIn.getRed()*factor);
		int go = (int) ((float) colourIn.getGreen()*factor);
		int bo = (int) ((float) colourIn.getBlue()*factor);

		printf("%f\n", factor);
        Colour colour(ro, go, bo);

		return colour;
	}
	return colourIn;
}

SynaesthetiQ::SynaesthetiQ() {

    ws2811_channel_t chn = {
                .gpionum = GPIO_PIN,
                .invert = 0,
                .count = matrixPixels+bigLEDCount,
                .strip_type = STRIP_TYPE,
                .brightness = 255,
            };
    ws2811_channel_t nll = {
                .gpionum = 0,
                .invert = 0,
                .count = 0,
                .brightness = 0,
            };

    ws2811_t leds = {
        .freq = 800000,
        .dmanum = DMA,
        .channel = {chn, nll},
    };

    if ((ret = ws2811_init(&leds)) != WS2811_SUCCESS)
    {
        fprintf(stderr, "ws2811_init failed: %s\n", ws2811_get_return_t_str(ret));
        exit(ret);
    }

    ledstring = leds;
}

SynaesthetiQ::~SynaesthetiQ() {
    if (clear_on_exit) {
        clearOutput();
    }
    ws2811_fini(&ledstring);
};

void SynaesthetiQ::setBigLEDColour(Colour colour) {
    bigLEDColour = bigLEDLimit(colour);
};

void SynaesthetiQ::setMatrixColour(Colour colourIn) {
    int start;
    int end;

    if (bigLEDFirst) {
        start = bigLEDCount;
        end = matrixPixels+bigLEDCount;
    } else {
        start = 0;
        end = matrixPixels;
    }

    // printf("%u %u",start,end);

    auto colourInGRB = colourIn.getGRB();
    for (int i = start; i < end; i++) {
        // printf("%X ",colourInGRB);
        ledstring.channel[0].leds[i] = colourInGRB;
    }
};

void SynaesthetiQ::setMatrixPixelColour(int x,int y, Colour colourIn) {
    
    if(x>31 || x<0 || y>15 || y<0){
        std::cerr << "SynaesthetiQ pixel coordinate out of range\n";
        throw std::exception();
        return;
    }
    
    int matrixStart;
    if (bigLEDFirst) {
        matrixStart = bigLEDCount;
    } else {
        matrixStart = 0;
    }
    XYPos XY = {x,y}; 
    int chainPos = matrixStart + XYtoChainPos(XY);

    ledstring.channel[0].leds[chainPos] = colourIn.getGRB();
};

void SynaesthetiQ::clearOutput() {
    Colour colour(colourOff);
    setMatrixColour(colour);
    setBigLEDColour(colour);
    
    render();
}

void SynaesthetiQ::limitMatrixCurrent() {
    double current = calculateMatrixCurrent();
    printf("Current= %f\n",current);
    if (current > maxMatrixCurrent) {
        double factor = maxMatrixCurrent/current;
        applyFactorToMatrix(factor);
    }
    printf("PostLimitCurrent= %f\n",calculateMatrixCurrent());
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
        current += calculateLEDCurrent(matrixPixelCurrentPerChannel, Colour(ledstring.channel[0].leds[i]));
    }

    return current/1000;
};

MILLIAMPS SynaesthetiQ::calculateLEDCurrent(MILLIAMPS LEDMaxCurrentPerChannel,Colour colourIn) {
    MILLIAMPS current = 0;

    // printf("M=%f ", LEDMaxCurrentPerChannel);
    // printf("R=%d ",colourIn.getRed());
    current += ((double) colourIn.getRed()/255)*LEDMaxCurrentPerChannel;
    current += ((double) colourIn.getGreen()/255)*LEDMaxCurrentPerChannel;
    current += ((double) colourIn.getBlue()/255)*LEDMaxCurrentPerChannel;
    
    // printf("C=%f \t",current);
    return current;
}

Colour SynaesthetiQ::applyFactorToLED(double factor, Colour colourIn) {
    Colour factoredColour(colourIn.getRed()*factor, colourIn.getGreen()*factor, colourIn.getBlue()*factor);
    return factoredColour;
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
        ledstring.channel[0].leds[i] = applyFactorToLED(factor,Colour(ledstring.channel[0].leds[i])).getGRB();
    }
};

int SynaesthetiQ::XYtoChainPos(XYPos XY) {
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

XYPos SynaesthetiQ::ChainPostoXY(int ChainPos) {
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
    XYPos XY = {x,y}; 
    return XY;
};

int SynaesthetiQ::render() {

    limitMatrixCurrent();

    if ((ret = ws2811_render(&ledstring)) != WS2811_SUCCESS)
	{
	    fprintf(stderr, "ws2811_render failed: %s\n", ws2811_get_return_t_str(ret));
        return -1;
	}
    return 1;
};