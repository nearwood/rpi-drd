
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

	return 0;
}
