#include "freestand.h"

void set_memory(void *ptr,int value, size_t size)
{
	int word = 1;
	unsigned char v = (unsigned char)value;

	if (size % sizeof(long) == 0)
		word = sizeof(long);
	else if (size % sizeof(int) == 0)
		word = sizeof(int);
	else if (size % sizeof(short) == 0)
		word = sizeof(short);

	unsigned char 	*p_char = 0x0;
	short 			*p_short = 0x0;
	int   			*p_int = 0x0;
	long 			*p_long= 0x0;

	switch(word){
	case 1:
	{
		p_char = (unsigned char*)ptr; 
		size_t i;
		for(i = 0; i < size;*p_char = v, i++, p_char++);	
		break;
	}
	case 2:
	{
		p_short = (short*)ptr; 
		short sw = (short)(v << 8 | v);
		size_t i;
		for(i = 0; i < (size / word); *p_short = sw, i++, p_short++);	
		break;
	}
	case 4:
	{
		p_int = (int *)ptr; 
		int iw = (int)(v << 24 | v << 16 | v << 8 | v);
		size_t i;
		for(i = 0; i < (size / word);*p_int = iw, i++, p_int++);	
		break;
	}
	case 8:
	{
		p_long = (long *)ptr; 
		long iw = (long)( (long)v << 56 | (long)v << 48 | (long)v << 32 | v << 24 | v << 16 | v << 8 | v);
		size_t i;
		for(i = 0; i < (size / word);*p_long = iw, i++, p_long++);	
		break;
	}
	default:
	break;
	}
}

int copy_memory(void *dest, void *src, size_t size)
{
	int word = 1;
	if(!dest || !src) return -1;

	if (size % sizeof(long) == 0)
		word = sizeof(long);
	else if (size % sizeof(int) == 0)
		word = sizeof(int);
	else if (size % sizeof(short) == 0)
		word = sizeof(short);

	switch(word){
		case 1:
			{
				unsigned char *d = (unsigned char *) dest;
				unsigned char *s = (unsigned char *) src;
				size_t i;
				for(i = 0; i < size; *d = *s, i++, d++, s++);
				break;
			}
		case 2:
			{
				short *d = (short *) dest;
				short *s = (short*) src;
				size_t i;
				for(i = 0; i < (size / word); *d = *s, i++, d++, s++);
				break;
			}
		case 4:
			{
				int *d = (int *) dest;
				int *s = (int *) src;
				size_t i;
				for(i = 0; i < (size / word); *d = *s, i++, d++, s++);
				break;
			}
		case 8:
			{
				long *d = (long *) dest;
				long *s = (long *) src;
				size_t i;
				for(i = 0; i < (size / word); *d = *s, i++, d++, s++);
				break;
			}
		default:
			break;
	}


	return 0;
}



