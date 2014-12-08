/*
 * main.c
 *
 * Copyright 2014 Nick Earwood <http://www.nearwood.net/>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

//07.12.2014

#include <bcm2835.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <signal.h>

#define DRD_VERSION "0.1a"

#define DEBUG_OFF 0
#define DEBUG_ON 1

//Pins for Model B, rev. 2
#define A_ENC	RPI_V2_GPIO_P1_03
#define A_IN1	RPI_V2_GPIO_P1_16
#define A_IN2	RPI_V2_GPIO_P1_18
#define A_PWM	RPI_V2_GPIO_P1_12

#define B_ENC	RPI_V2_GPIO_P1_05
#define B_IN1	RPI_V2_GPIO_P1_13
#define B_IN2	RPI_V2_GPIO_P1_15
#define B_PWM	RPI_V2_GPIO_P1_11

#define STBY	RPI_V2_GPIO_P1_07

sig_atomic_t signaled = 0;
uint8_t debug_mode = DEBUG_OFF;

int showusage(char* arg0)
{
	printf("Usage: %s -[b|c|d[d]|h|v]\n", arg0);
	printf("\n");
	printf("  With no arguments, it ouputs the current status of\n");
	printf("   any existing instance and exits, or complains there\n");
	printf("   aren't any and exits.\n");
	printf("\n");
	printf("Options:\n");
	printf("  -b   Fork into the background\n");
	printf("  -c   Connect to an existing instance, creating one if -b is also specified\n");
	printf("  -d   Enable debug output\n");
	printf("  -dd  Set bcm2835 library to debug mode (pin I/O disabled)\n");
	printf("  -h   Display this information and exit\n");
	printf("  -v   Display version information and exit\n");
	printf("\n");
	printf("For bugs, comments, or complaints, please see:\n");
	printf("<http://www.nearwood.net/>\n");
}

int comparse(int argc, char** argv)
{
	int argnum = 1;
	while (argnum < argc && argv[argnum][0] == '-')
	{
		switch (argv[argnum][1])
		{
			case 'v': //version
				printf("%s %s\n", argv[0], DRD_VERSION);
				printf("Copyright (C) 2014 Nick Earwood <http://www.nearwood.net/>\n");
				printf("This is free software; see the source for copying conditions.  There is NO\n");
				printf("warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n");
				exit(EXIT_SUCCESS);
				break;

			case 'h': //usage
				showusage(argv[0]);
				exit(EXIT_SUCCESS);
				break;

			case 'd': //debug
				debug_mode = DEBUG_ON;
				switch (argv[argnum][2])
				{
					//Set bcm lib to debug mode, disabling pin output and printing what it would do instead.
					case 'd': bcm2835_set_debug(1);
						break;
				}
				break;

			case 'c':
				//switch (argv[argnum][2])
				//{
					//TODO specify pid file here or something
				//}
				break;

			case 'b':
				break;

			default:
				fprintf(stderr, "%c is not a valid option\n", argv[argnum][1]);
				showusage(argv[0]);
				exit(EXIT_FAILURE);
				break;
		}

		argnum++;
	}

	if (argnum == argc)
		return EXIT_SUCCESS;
	else
		fprintf(stderr, "Invalid argument syntax.\n");
		return EXIT_FAILURE;
}

void debug(const char* format, ...)
{
	if (debug_mode == DEBUG_ON)
	{
		va_list args;
		va_start(args, format);
		vprintf(format, args);
		va_end(args);
	}
}

void sigHandler(int sig)
{
	debug("\nInterrupt signal.\n");
	signaled = 1;
}

int getInput(uint8_t pin)
{
	//If an input event was detected
	if (bcm2835_gpio_eds(pin))
	{
		//reset the event/pin
		debug("getInput() on pin: %d", pin);
		bcm2835_gpio_set_eds(pin);
		return 1;
	}

	return 0;
}

int main(int argc, char** argv)
{
	if (comparse(argc, argv) == EXIT_FAILURE) return EXIT_FAILURE;

	if (geteuid() != 0 || getenv("FAKEROOTKEY"))
	{
		fprintf(stderr, "You need to be root to run this.\n");
		return 1;
	}

	signal(SIGINT, sigHandler);

	uint8_t quit = 0;
	uint8_t a_count = 0, b_count = 0;

	debug("Initializing BCM2835\n");
	if (!bcm2835_init()) return 1;

	//Setup encoders for input (SDA and SCL pins already have a pullup!)
	bcm2835_gpio_fsel(A_ENC, BCM2835_GPIO_FSEL_INPT);
	bcm2835_gpio_fsel(B_ENC, BCM2835_GPIO_FSEL_INPT);

	//Setup High Detect Enable (High Event nnnnn?)
	bcm2835_gpio_hen(A_ENC);
	bcm2835_gpio_hen(B_ENC);

	while (quit == 0)
	{
		if (getInput(A_ENC) == 1) ++a_count;
		if (getInput(B_ENC) == 1) ++b_count;

		debug("MOTOR A: %d\n", a_count);
		debug("MOTOR B: %d\n", b_count);

		delay(500);

		if (signaled == 1) quit = 1;
	}

	debug("Closing BCM2835\n");
	if (!bcm2835_close()) return 2;

	return 0;
}
