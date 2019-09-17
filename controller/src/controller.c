/*
 * controller.c
 *
 * Copyright 2014-2019 Nick Earwood <http://www.nearwood.net/>
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

#include <bcm2835.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <signal.h>
#include <time.h>

#include <stdint.h>
#include <inttypes.h>

#include <ncurses/ncurses.h>

#define DRD_VERSION "0.1a"

#define DEBUG_OFF 0
#define DEBUG_ON 1

//Pins for RPi Model B+

//Motor controller
#define A_ENC	RPI_BPLUS_GPIO_J8_03 //BCM2 (SDA)
#define A_IN1	RPI_BPLUS_GPIO_J8_18 //BCM24
#define A_IN2	RPI_BPLUS_GPIO_J8_16 //BCM23
#define A_PWM	RPI_BPLUS_GPIO_J8_12 //BCM18

#define B_ENC	RPI_BPLUS_GPIO_J8_05 //BCM3 (SCL)
#define B_IN1	RPI_BPLUS_GPIO_J8_13 //BCM27
#define B_IN2	RPI_BPLUS_GPIO_J8_15 //BCM22
#define B_PWM	RPI_BPLUS_GPIO_J8_35 //BCM19

#define STBY	RPI_BPLUS_GPIO_J8_07 //BCM4

#define PWM_RANGE 1024
#define MOTOR_TICK_DELAY  1000000000LL //1000ms //100000000LL //100ms
#define REFRESH_SPEED      250000000LL  //250ms

// Sonar/Range sensors
#define SONAR1  RPI_BPLUS_GPIO_J8_29 //BCM5
#define SONAR2  RPI_BPLUS_GPIO_J8_31 //BCM6
#define SONAR_TRIGGER RPI_BPLUS_GPIO_J8_36 //BCM16
#define SONAR_TRIGGER_TIME 10000000LL //10mS (28uS < t < 48mS)

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
				printf("Copyright (C) 2014-2016 Nick Earwood <http://www.nearwood.net/>\n");
				printf("This is free software; see the source for copying conditions.\n");
				printf("There is NO warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n");
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
uint8_t encoderTick(uint8_t pin)
{
	if (bcm2835_gpio_eds(pin))
	{
		bcm2835_gpio_set_eds(pin);
		return 1;
	}

	return 0;
}


//If rising, mark time
//If falling, subtract time to get pulse width in microseconds
//sonarUpdate(SONAR1, &cTime, &sonarTimeA);
void sonarUpdate(uint8_t pin, uint64_t* cTime, uint64_t* sTime, float* distance)
{
  //TODO Consider enabling afen only after a trigger with aren.
  static uint8_t pin1High = 0, pin2High = 0;
  uint8_t event = encoderTick(pin);

  if (pin == SONAR1) {//HACK
    if (pin1High) {
      uint64_t dt = (uint64_t)(*cTime) - (uint64_t)(*sTime);
      //get distance via formula: 147uS per inch
      *distance = (float)dt / 147000.0f;
      pin1High = 0;
      debug("%llu: SONAR1 FALLING\n", cTime);
    } else {
      //mark time
      *sTime = *cTime;
      pin1High = 1;
      debug("%llu: SONAR1 RISING\n", cTime);
    }
  } else if (pin == SONAR2) {
    if (pin2High) {
      uint64_t dt = (uint64_t)(*cTime) - (uint64_t)(*sTime);
      //get distance via formula: 147uS per inch
      *distance = (float)dt; //ms
      pin2High = 0;
    } else {
      //mark time
      *sTime = *cTime;
      pin2High = 1;
    }
  }
}



struct Motor
{
	uint8_t pwmChannel, encoderPin; //const
	int32_t pwm;
	uint32_t encoderCount;
	float target, actual; //time between ticks
	double error, lastError, dError, iError;
	float speed;
} motorA, motorB;

/**
 * Check for rising/falling edge and update encoder count.
 */
void motorUpdate(struct Motor* motor, uint64_t* cTime, uint64_t* lTime)
{
	if (encoderTick(motor->encoderPin)) ++motor->encoderCount;

	uint64_t dt = (uint64_t)(*cTime) - (uint64_t)(*lTime);

	if (dt > MOTOR_TICK_DELAY)
	{
		//debug("ctime: %llu\n", *cTime);
		//debug("ltime: %llu\n", *lTime);
		//debug("diff: %llu\n", dt);

		//actual speed = tick count / (dt in ???seconds)
		motor->actual = (float)motor->encoderCount / (dt / 100000000LL);
		motor->encoderCount = 0; //TODO need rolling average
		*lTime = *cTime; //reset motor last tick time

		//calculate raw PWM
		motor->lastError = motor->error;
		motor->error = (motor->target - motor->actual);
		motor->dError = motor->error - motor->lastError;
		motor->iError += motor->error;
		motor->pwm = (motor->error * 75 + motor->iError * 120 + motor->dError * 5);
		//motor->pwm = (motor->error * 50 + motor->iError * 100 + motor->dError * 10); //pretty smooth, takes a while
		//motor->pwm = (motor->error * 60 + motor->iError * 120 + motor->dError * 10); //a bit jerky, but doesn't overshoot

		//clamp PWM
		if (motor->pwm > PWM_RANGE) motor->pwm = PWM_RANGE;
		else if (motor->pwm < 0) motor->pwm = 0;

		//set PWM
		bcm2835_pwm_set_data(motor->pwmChannel, motor->pwm);
	}
}

void motorPrint(uint8_t x, uint8_t y, const struct Motor* motor)
{
	mvprintw(x, y, "%04d\t%05.3f\t%05.3f\t%04d", motor->pwm, motor->actual, motor->error, motor->encoderCount);
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

	if (debug_mode != DEBUG_ON)
	{
		initscr();
		noecho();

		mvprintw(0, 0, " PWM\tSPEED\tERROR\tTICKS\tSONAR");
		refresh();
	}

	debug("Initializing BCM2835\n");
	if (!bcm2835_init()) return 1;

	debug("Setting up encoders on pins: %d, %d\n", A_ENC, B_ENC);
	//Setup encoders for input (SDA and SCL pins already have a pullup!)
	bcm2835_gpio_fsel(A_ENC, BCM2835_GPIO_FSEL_INPT);
  bcm2835_gpio_fsel(B_ENC, BCM2835_GPIO_FSEL_INPT);

	//Set pull up
	//bcm2835_gpio_set_pud(A_ENC, BCM2835_GPIO_PUD_UP);
	//bcm2835_gpio_set_pud(B_ENC, BCM2835_GPIO_PUD_UP);

	//set event detect mode, rising edge, and falling edge.
	bcm2835_gpio_aren(A_ENC);
	bcm2835_gpio_aren(B_ENC);
	bcm2835_gpio_afen(A_ENC);
	bcm2835_gpio_afen(B_ENC);

	bcm2835_gpio_fsel(A_PWM, BCM2835_GPIO_FSEL_ALT5); //Enable PWM0
	bcm2835_gpio_fsel(B_PWM, BCM2835_GPIO_FSEL_ALT5); //Enable PWM1
	bcm2835_pwm_set_clock(BCM2835_PWM_CLOCK_DIVIDER_16);

	//Set PWM Channel X, balanced mode, enabled
  debug("Setting PWM channels\n");
	bcm2835_pwm_set_mode(0, 1, 1);
	bcm2835_pwm_set_mode(1, 1, 1);

	//Set range, where pulse freq. is 1.2MHz/range
	bcm2835_pwm_set_range(0, PWM_RANGE);
	bcm2835_pwm_set_range(1, PWM_RANGE);

	//Set motor polarity pins as ouput
  debug("Setting motor output pins\n");
	bcm2835_gpio_fsel(A_IN1, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(A_IN2, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(B_IN1, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(B_IN2, BCM2835_GPIO_FSEL_OUTP);

	bcm2835_gpio_fsel(STBY, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_clr(STBY); //make sure it's low

	//Setup motor polarity (direction)
	bcm2835_gpio_write(A_IN1, LOW);
	bcm2835_gpio_write(A_IN2, HIGH);
	bcm2835_gpio_write(B_IN1, LOW);
	bcm2835_gpio_write(B_IN2, HIGH);

	//Setup motor struct "consts"
	motorA.pwmChannel = 0;
	motorB.pwmChannel = 1;
	motorA.encoderPin = A_ENC;
	motorB.encoderPin = B_ENC;

  debug("Initializing motor PWM to 0\n");
	bcm2835_pwm_set_data(motorA.pwmChannel, 0);
	bcm2835_pwm_set_data(motorB.pwmChannel, 0);

	//Setup test target speeds (RPM)
	motorA.target = 3.0; //max 4.9-5.1
	motorB.target = 3.0; //max 4.5-4.6

  ///Setup sonar sensors to receive pulse input

  //Set Sonar RX pin low to prevent ranging.
  debug("Initializing sonar pins\n");
  bcm2835_gpio_fsel(SONAR_TRIGGER, BCM2835_GPIO_FSEL_OUTP);
  bcm2835_gpio_clr(SONAR_TRIGGER);

  //Setup edge detection for sonar PW inputs
	bcm2835_gpio_fsel(SONAR1, BCM2835_GPIO_FSEL_INPT);
	bcm2835_gpio_aren(SONAR1);
	bcm2835_gpio_afen(SONAR1);

	bcm2835_gpio_fsel(SONAR2, BCM2835_GPIO_FSEL_INPT);
	bcm2835_gpio_aren(SONAR2);
	bcm2835_gpio_afen(SONAR2);

	debug("Standby off.\n");
	bcm2835_gpio_set(STBY);

	struct timespec time;
	uint64_t cTime = 0, lTime = 0, sTime = 0,
           lTimeA = 0, lTimeB = 0, sonarTimeA = 0, sonarTimeB = 0;

	clock_gettime(CLOCK_MONOTONIC_RAW, &time);
	lTime = time.tv_sec * MOTOR_TICK_DELAY + time.tv_nsec;

  float sonarDistanceA = 0, sonarDistanceB = 0;

	while (quit == 0)
	{
		clock_gettime(CLOCK_MONOTONIC_RAW, &time);
		cTime = time.tv_sec * MOTOR_TICK_DELAY + time.tv_nsec;
    //debug("%llu: step\n", cTime);

		motorUpdate(&motorA, &cTime, &lTimeA);
		motorUpdate(&motorB, &cTime, &lTimeB);

    //Hold the RX pin for at least SONAR_TRIGGER_TIME to trigger a range operation
    if (bcm2835_gpio_lev(SONAR_TRIGGER) == HIGH && cTime - sTime > SONAR_TRIGGER_TIME) {
      debug("%llu: Clearing RX pin\n", cTime);
      bcm2835_gpio_clr(SONAR_TRIGGER);
      sTime = cTime;
    }

    //Check rising/falling edges for reading responses (ugh)
    sonarUpdate(SONAR1, &cTime, &sonarTimeA, &sonarDistanceA);
    sonarUpdate(SONAR2, &cTime, &sonarTimeB, &sonarDistanceB);

		if (cTime - lTime > REFRESH_SPEED)
		{//Refresh the ncurses output
      lTime = cTime;

      //Temp place to set sonar range command every REFRESH_SPEED
      debug("%llu: Setting RX pin\n", cTime);
      bcm2835_gpio_set(SONAR_TRIGGER);
      sTime = cTime;

			if (debug_mode != DEBUG_ON)
			{
				motorPrint(1, 0, &motorA);
				motorPrint(2, 0, &motorB);
	      mvprintw(1, 32, "%04f", sonarDistanceA);
	      mvprintw(2, 32, "%04f", sonarDistanceB);
				refresh();
			}

			//bcm2835_pwm_set_data(motorA.pwmChannel, 800);
			//bcm2835_pwm_set_data(motorB.pwmChannel, 1024);
      //delay(100);

			if (signaled == 1) quit = 1;
		}
	}

	debug("Standby motors.\n");
	bcm2835_gpio_clr(STBY);

  debug("Standby sonar.\n");
  bcm2835_gpio_clr(SONAR_TRIGGER);

	debug("Closing BCM2835.\n");
	if (!bcm2835_close()) return 2;

	if (debug_mode != DEBUG_ON)
	{
		debug("Cleanup ncurses.\n");
		endwin();
	}

	return 0;
}
