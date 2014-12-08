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

#include <ncurses/ncurses.h>

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

//Ick, globals
uint8_t aLast = 0, bLast = 0, aCount = 0, bCount = 0;
uint16_t timeSpan = 1;

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

//poll encoder, returning 1 if the state has switched, 0 otherwise
uint8_t encoderTick(uint8_t pin, uint8_t last)
{
	return (bcm2835_gpio_lev(pin) != last);
}

//TODO use ref of ints instead of globals
float motorSpeedA()
{
	if (encoderTick(A_ENC, aLast))
	{
		aLast = !aLast;
		++aCount;
	}

	return aCount / timeSpan;
}

//float motorSpeed(uint8_t pin, uint8_t* last, uint8_t* count)
float motorSpeedB()
{
	if (encoderTick(B_ENC, bLast))
	{
		bLast = !bLast;
		++bCount;
	}

	return bCount / timeSpan;
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

	float aSpeed = 0.0, bSpeed = 0.0;

	debug("Initializing BCM2835\n");
	if (!bcm2835_init()) return 1;

	debug("Setting up encoders on pins: %d, %d\n", A_ENC, B_ENC);
	//Setup encoders for input (SDA and SCL pins already have a pullup!)
	bcm2835_gpio_fsel(A_ENC, BCM2835_GPIO_FSEL_INPT);
	//bcm2835_gpio_set_pud(A_ENC, BCM2835_GPIO_PUD_UP);

	bcm2835_gpio_fsel(B_ENC, BCM2835_GPIO_FSEL_INPT);
	//bcm2835_gpio_set_pud(B_ENC, BCM2835_GPIO_PUD_UP);

	bcm2835_gpio_fsel(A_PWM, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(A_IN1, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(A_IN2, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(B_PWM, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(B_IN1, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(B_IN2, BCM2835_GPIO_FSEL_OUTP);

	bcm2835_gpio_fsel(STBY, BCM2835_GPIO_FSEL_OUTP);
	//bcm2835_gpio_clr(STBY); //make sure it's low


	bcm2835_gpio_write(A_IN1, HIGH);
	bcm2835_gpio_write(A_IN2, LOW);
	bcm2835_gpio_write(B_IN1, LOW);
	bcm2835_gpio_write(B_IN2, HIGH);

	bcm2835_gpio_write(A_PWM, HIGH);
	bcm2835_gpio_write(B_PWM, HIGH);

	debug("Standby off");
	bcm2835_gpio_set(STBY);

	//Setup ncurses
	initscr();
	noecho();

	while (quit == 0)
	{
		aSpeed = motorSpeedA();
		bSpeed = motorSpeedB();

		mvprintw(0, 0, "MOTOR A: %03d/%05d %03.3f", aCount, timeSpan, aSpeed);
		mvprintw(1, 0, "MOTOR B: %03d/%05d %03.3f", bCount, timeSpan, bSpeed);

		delay(100);
		timeSpan += 100;

		if (timeSpan >= 1000)
		{
			timeSpan = 1;
			aCount = 0;
			bCount = 0;
		}

		refresh();

		if (signaled == 1) quit = 1;
	}

	bcm2835_gpio_clr(STBY);

	endwin();

	debug("Closing BCM2835\n");
	if (!bcm2835_close()) return 2;

	return 0;
}
