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

int main(int argc, char *argv[])
{
    SynaesthetiQ Syn;

	ws2811_led_t colour = 0x0000000f;

	Syn.setBigLEDColour(colour);
    Syn.setMatrixColour(colour);
    Syn.render();

	getchar();

	// colour = 0x000f000f;

	// Syn->setBigLEDColour(colour);
    // Syn->setMatrixColour(colour);
    // Syn->render();

	// getchar();

	// colour = 0x00ffffff;

	// Syn->setBigLEDColour(colour);
    // Syn->setMatrixColour(colour);
    // Syn->render();

	// getchar();

    printf ("\n");
    // Syn->~SynaesthetiQ();
    delete Syn; 
}
