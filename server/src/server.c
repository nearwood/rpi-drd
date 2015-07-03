#include "fcgi_config.h"

#include <stdlib.h>
#include <signal.h>

#include "fcgi_stdio.h" // fcgi lib must be first fsr

sig_atomic_t signaled = 0;
int count;

void sigHandler(int sig)
{
	printf("\nInterrupt signal.\n");
	signaled = 1;
}

void initialize()
{
	count=0;
}

int main()
{
	signal(SIGINT, sigHandler);

	initialize();

	//Response loop
	while (FCGI_Accept() >= 0)
	{
		printf("Content-type: text/xml\r\n\r\n<drd><response>ACK</response><status>OK</status></drd>");

		if (signaled) break;
	}

	return 0;
}
