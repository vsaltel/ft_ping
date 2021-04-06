#include "ping.h"

unsigned short	checksum(void *b, int len) 
{
	unsigned short *buf = b; 
	unsigned int sum=0; 
	unsigned short result; 
	
	for ( sum = 0; len > 1; len -= 2 ) 
		sum += *buf++; 
	if ( len == 1 ) 
		sum += *(unsigned char*)buf; 
	sum = (sum >> 16) + (sum & 0xFFFF); 
	sum += (sum >> 16); 
	result = ~sum; 
	return result; 
}

void    tv_subtract(struct timeval *out, struct timeval *in)
{
	if ((out->tv_usec -= in->tv_usec) < 0)
	{
		--out->tv_sec;
		out->tv_usec += 1000000;
	}
	//out->tv_sec -= in->tv_sec;
}
