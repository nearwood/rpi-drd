#include "fcgi_config.h"

#include <stdlib.h>
#include <signal.h>
#include <string.h>

#include "fcgi_stdio.h"

sig_atomic_t signaled = 0;
int count;
extern char **environ;

/* Request Environment:
FCGI_ROLE=RESPONDER
SERVER_SOFTWARE=lighttpd/1.4.35
SERVER_NAME=192.168.0.116
GATEWAY_INTERFACE=CGI/1.1
SERVER_PORT=80
SERVER_ADDR=192.168.0.116
REMOTE_PORT=42670
REMOTE_ADDR=192.168.0.111
SCRIPT_NAME=/cgi-bin/test
PATH_INFO=
SCRIPT_FILENAME=/srv/http/cgi-bin/test
DOCUMENT_ROOT=/srv/http
REQUEST_URI=/cgi-bin/test
REQUEST_URI=/cgi-bin/test?test=2&hamburger=salad
QUERY_STRING=
QUERY_STRING=test=2&hamburger=salad
REQUEST_METHOD=GET
REDIRECT_STATUS=200
SERVER_PROTOCOL=HTTP/1.1
HTTP_HOST=192.168.0.116
HTTP_CONNECTION=keep-alive
HTTP_ACCEPT=*\* <--slash actually went the other way
HTTP_X_REQUESTED_WITH=XMLHttpRequest
HTTP_USER_AGENT=Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/43.0.2357.130 Safari/537.36
HTTP_DNT=1
HTTP_REFERER=http://192.168.0.116/?
HTTP_ACCEPT_ENCODING=gzip, deflate, sdch
HTTP_ACCEPT_LANGUAGE=en-US,en;q=0.8

Initial environment:
XDG_SESSION_ID=c66
TERM=xterm
SHELL=/bin/bash
SSH_CLIENT=192.168.0.111 39264 22
SSH_TTY=/dev/pts/0
USER=root
MAIL=/var/spool/mail/root
PATH=/usr/local/sbin:/usr/local/bin:/usr/bin:/usr/bin/site_perl:/usr/bin/vendor_perl:/usr/bin/core_perl
PWD=/root
LANG=C
SHLVL=1
HOME=/root
LOGNAME=root
SSH_CONNECTION=192.168.0.111 39264 192.168.0.116 22
XDG_RUNTIME_DIR=/run/user/0
_=/usr/bin/spawn-fcgi
*/

void sigHandler(int sig)
{
	printf("\nInterrupt signal.\n");
	signaled = 1;
}

void initialize()
{
	count=0;
}

//QUERY_STRING=test=2&hamburger=salad
char** parseQuery(char *query)
{
	char q[256];
	strncpy(q, query, 255);
	char* token = strtok(q, "=");
	if (token != NULL)
	{
		//add token to array...
	}

	return NULL;
}

//find what=xyz in array of strings. return xyz or null. xyz may be null.
char* findVar(const char *what, char **envp)
{
	while (*envp != NULL)
	{
		char t[256];
		strncpy(t, *envp, 255);
		char* token = strtok(t, "=");
		if (token != NULL)
		{
			int r = strncmp(what, token, 255);
			if (r == 0)
			{
				return strtok(NULL, "");
			}
		}
		envp++;
	}

	return NULL;
}

int main()
{
	signal(SIGINT, sigHandler);

	//char **initialEnv = environ;

	initialize();

	//Response loop
	while (FCGI_Accept() >= 0)
	{
		printf("Content-type: text/json\r\n\r\n{\"response\": \"ACK\", \"status\": \"OK\"}");

		//PrintEnv("Request environment", environ);

		if (signaled) break;
	}

	return 0;
}
