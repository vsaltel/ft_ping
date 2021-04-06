#include "ping.h"

unsigned short	checksum(void *b, int len) 
{
	unsigned short	*buf; 
	unsigned int	sum; 
	unsigned short	result; 
	
	buf = b;
	sum = 0;
	for ( sum = 0; len > 1; len -= 2 ) 
		sum += *buf++; 
	if ( len == 1 ) 
		sum += *(unsigned char*)buf; 
	sum = (sum >> 16) + (sum & 0xFFFF); 
	sum += (sum >> 16); 
	result = ~sum; 
	return result; 
}
