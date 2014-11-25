
#include "gpio.h"

int main(int argc, char **argv)
{
	gpio_setup();
	//ALWAYS CALL INP_GPIO BEFORE USING OUT/SET!
	//7,11,12,13,15,16,18
	INP_GPIO(7);
	INP_GPIO(11);
	INP_GPIO(12);
	INP_GPIO(13);
	INP_GPIO(15);
	INP_GPIO(16);
	INP_GPIO(18);
	OUT_GPIO(7);
	OUT_GPIO(11);
	OUT_GPIO(12);
	OUT_GPIO(13);
	OUT_GPIO(15);
	OUT_GPIO(16);
	OUT_GPIO(18);

	//Motor A direction
	GPIO_SET = 1 << 12; //AIN1
	GPIO_SET = 0 << 11; //AIN2
	//Motor A speed
	GPIO_SET = 1 << 7; //PWMA (No PWM yet :( )

	//Motor B direction
	GPIO_SET = 1 << 15; //BIN1
	GPIO_SET = 0 << 16; //BIN2
	//Motor B speed
	GPIO_SET = 1 << 18; //PWMB (should change pin, this is HW PWM pin)

	//Turn off standby by setting pin 13 high
	GPIO_SET = 1 << 13;

	//sleep

	//TODO Camera, web access, sonar tests
	
	return 0;
}
