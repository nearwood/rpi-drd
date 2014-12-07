/*
 *
 *
 *
 */

#include <bcm2835.h>

#include <stdio.h>
#include <stdlib.h>
//#include <stdint.h>

int showusage(char* arg0, int errcode)
{
	printf("Usage: %s [options]\n", arg0);
	printf("\n");
	printf("  With no arguments, it simply ouputs the current status of\n");
	printf("   any existing instance and exits, or complains there\n");
	printf("   aren't any and exits.\n");
	printf("\n");
	printf("Options:\n");
	printf("  -b Fork into the background\n");
	printf("  -c Connect to an existing instance, creating one if -b is also specified\n");
	printf("  -d Enable debug output\n");
	printf("  -h Display this information and exit\n");
	printf("  -v Display version information and exit\n");
	printf("\n");
	printf("For bugs, comments, or complaints, please see:\n");
	printf("<https://www.nearwood.net/>\n");
	return errcode;
}

int comparse(int argc, char** argv)
{
	if (argc < 2)
	{
		fprintf(stderr, "Insufficient command line arguments\n");
		return EXIT_FAILURE;
	}
}

int main(int argc, char** argv)
{
	if (comparse(argc, argv) == EXIT_FAILURE) return showusage(argv[0], EXIT_FAILURE);

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
