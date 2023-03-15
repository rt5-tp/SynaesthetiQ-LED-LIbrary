#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <signal.h>
#include <stdarg.h>
#include <getopt.h>

//#include "clk.h"
//#include "gpio.h"
//#include "dma.h"
//#include "pwm.h"
//#include "version.h"

#include "ws2811.h"
#include "synaesthetiq.hpp"

#define TARGET_FREQ             WS2811_TARGET_FREQ
#define GPIO_PIN                10
#define DMA                     10
#define STRIP_TYPE              WS2811_STRIP_RGB		// WS2812/SK6812RGB integrated chip+leds
// #define STRIP_TYPE              WS2811_STRIP_GBR		// WS2812/SK6812RGB integrated chip+leds
//#define STRIP_TYPE            SK6812_STRIP_RGBW		// SK6812RGBW (NOT SK6812RGB)

#define WIDTH                   513


int main()
{
    SynaesthetiQ visualiser;
    
    // pass in color as RGB
    visualiser.setBigLEDColour(0x0000000f);
    visualiser.render();

    getchar();

    visualiser.setBigLEDColour(0x000f000f);
    visualiser.render();

    getchar();

    visualiser.setBigLEDColour(0x00ffffff);
    visualiser.render();

    getchar();

    printf ("\n");
}
