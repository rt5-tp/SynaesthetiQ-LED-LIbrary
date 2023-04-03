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

#include "synaesthetiq.hpp"

#define WIDTH                   513
#define HEIGHT                  1

int width = WIDTH;
int height = HEIGHT;
int led_count = LED_COUNT;

ws2811_led_t *matrix;
static uint8_t running = 1;

int main(int argc, char *argv[])
{
    SynaesthetiQ *Syn = new SynaesthetiQ();

	ws2811_led_t colour = 0x0000000f;

	Syn->setBigLEDColour(colour);
    Syn->setMatrixColour(colour);
    Syn->setMatrixPixelColour(0,0,colourOff);
    Syn->render();

	getchar();

	colour = 0x000f000f;

	Syn->setBigLEDColour(colour);
    Syn->setMatrixColour(colour);
    Syn->render();

	getchar();

	colour = 0x00ffffff;

	Syn->setBigLEDColour(colour);
    Syn->setMatrixColour(colour);
    Syn->render();

	getchar();

    printf ("\n\n");
    delete Syn; 
}
