#include <stdio.h>

const char *Jim_SignalId(int sig)
{
	static const char buf[32];
	sprintf(buf, "%d", sig);
	return buf;
}