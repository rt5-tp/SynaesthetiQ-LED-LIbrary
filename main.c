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

#define TARGET_FREQ             WS2811_TARGET_FREQ
#define GPIO_PIN                10
#define DMA                     10
#define STRIP_TYPE              WS2811_STRIP_RGB		// WS2812/SK6812RGB integrated chip+leds
// #define STRIP_TYPE              WS2811_STRIP_GBR		// WS2812/SK6812RGB integrated chip+leds
//#define STRIP_TYPE            SK6812_STRIP_RGBW		// SK6812RGBW (NOT SK6812RGB)

#define WIDTH                   513
#define HEIGHT                  1
#define LED_COUNT               (WIDTH * HEIGHT)

int width = WIDTH;
int height = HEIGHT;
int led_count = LED_COUNT;

int clear_on_exit = 1;


int rgbtobgr(int ColourIn) {
	int outColour = 0x00000000;

//	printf("In: %#010X\t",ColourIn);

	int r = (0x00ff0000 & ColourIn)>>16;
	int g = (0x0000ff00 & ColourIn)>>8;
	int b = (0x000000ff & ColourIn);

//	printf("R: %X G: %X B: %X\t\t\n",r,g,b);

	//printf("%#010X\t%#010X\t%#010X\t\n",(b<<8),(b<<8) | g,((b<<8)|g)<<8);

	outColour = (( (g<<8) | r) << 8 ) | b;

//	printf("Out: %#010X\n",outColour);

	return outColour;
}

int matrixLimit(int ColourIn) {
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

ws2811_t ledstring =
{
    .freq = TARGET_FREQ,
    .dmanum = DMA,
    .channel =
    {
        [0] =
        {
            .gpionum = GPIO_PIN,
            .invert = 0,
            .count = LED_COUNT,
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

// ws2811_led_t *matrix;

static uint8_t running = 1;

int main(int argc, char *argv[])
{
    ws2811_return_t ret;

    // sprintf(VERSION, "%d.%d.%d", VERSION_MAJOR, VERSION_MINOR, VERSION_MICRO);

    // parseargs(argc, argv, &ledstring);

    // matrix = malloc(sizeof(ws2811_led_t) * width * height);

    // setup_handlers();

    if ((ret = ws2811_init(&ledstring)) != WS2811_SUCCESS)
    {
        fprintf(stderr, "ws2811_init failed: %s\n", ws2811_get_return_t_str(ret));
        return ret;
    }

    //while (running)
    //{
        // matrix_raise();
        // matrix_bottom();
        // matrix_render();

	int chain = 512;
	int i;
	int colour = 0x0000000f;

	ledstring.channel[0].leds[0] = colour;

	for (i = 1; i < chain+1; i++) {
		ledstring.channel[0].leds[i] = matrixLimit(rgbtobgr(colour));
	}


	if ((ret = ws2811_render(&ledstring)) != WS2811_SUCCESS)
	{
	    fprintf(stderr, "ws2811_render failed: %s\n", ws2811_get_return_t_str(ret));
	    return ret;
	}

	getchar();

	colour = 0x000f000f;

	ledstring.channel[0].leds[0] = colour;

	for (i = 1; i < chain+1; i++) {
            ledstring.channel[0].leds[i] = matrixLimit(rgbtobgr(colour));
    }

    if ((ret = ws2811_render(&ledstring)) != WS2811_SUCCESS)
    {
        fprintf(stderr, "ws2811_render failed: %s\n", ws2811_get_return_t_str(ret));
        return ret;
    }

	getchar();

	colour = 0x00ffffff;

	ledstring.channel[0].leds[0] = colour;

	for (i = 1; i < chain+1; i++) {
                ledstring.channel[0].leds[i] = matrixLimit(rgbtobgr(colour));
        }

        if ((ret = ws2811_render(&ledstring)) != WS2811_SUCCESS)
        {
            fprintf(stderr, "ws2811_render failed: %s\n", ws2811_get_return_t_str(ret));
            return ret;
        }

	getchar();


        // 15 frames /sec
        //usleep(1000000 / 15);
    //}

    if (clear_on_exit) {
        // matrix_clear();
        // matrix_render();
	    colour = 0x00000000;

        ledstring.channel[0].leds[0] = colour;

        for (i = 1; i < chain+1; i++) {
                ledstring.channel[0].leds[i] = colour;
        }
        //ledstring.channel[0].leds[0] = 0x00000000;
        ws2811_render(&ledstring);
    }

    ws2811_fini(&ledstring);

    printf ("\n");
    return ret;
}
