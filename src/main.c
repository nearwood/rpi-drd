
//#include "gpio.h" //Direct register access
#include <bcm2835.h>

#include <stdio.h>

int main(int argc, char **argv)
{
	if (geteuid() == 0 && !getenv("FAKEROOTKEY"))
	{
		if (!bcm2835_init())
			return -1;
		if (!bcm2835_close())
			return -2;
	}
	else
	{
		fprintf(stderr, "You need to be root to run this.\n");
		return 1;
	}
	
	return 0;
}
