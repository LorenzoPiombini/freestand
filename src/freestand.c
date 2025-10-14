#include <stdio.h> /*temporary*/
#include "freestand.h"
#include "memory.h"

/*static functions to manage String struct*/
static void free_str(struct String *str);
static uint8_t empty(struct String *str);
static int append(struct String *str,char *str_to_appen);

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

/* STRING HANDLEING */
int init(struct String *str,char *val)
{
	if(!str) return -1;
	if(str->base[0] != '\0' || str->str){
		fprintf(stderr,"string has been already initialized\n");
		return -1;
	}
	
	if(!val){
		set_memory(str->base,0,DEF_STR);
		str->str = 0x0;
		str->append = append;
		str->is_empty = empty;
		str->close = free_str;
		str->size = 0;
		return 0;
	}

	int i;
	char *p = val;
	for(i = 0; *p != '\0'; i++,p++);

	str->size = i;
	if(str->size >= DEF_STR){
		str->str = (char*)ask_mem((str->size)*sizeof(char));
		if(!str->str){
			fprintf(stderr,"ask_mem failed, %s:%d\n",__FILE__,__LINE__-2);	
			return -1;
		}

		set_memory(str->str,0,str->size);
		for(i = 0; i < str->size;i++)
			str->str[i] = val[i];

		str->append = append;
		str->is_empty = empty;
		str->close = free_str;
		set_memory(str->base,0,DEF_STR);
		return 0;
	}
	for(i = 0; i < str->size;i++)
		str->base[i] = val[i];
	str->append = append;
	str->is_empty = empty;
	str->close = free_str;
	return 0;
}

void string_copy(char *dest, char *src, size_t size)
{
	int i;
	for(i = 0; i < size;i++)
		dest[i] = src[i];
}

size_t string_length(char *str)
{
	size_t i;
	char *p = str;
	for(i = 0; *p != '\0'; i++,p++);

	return i;
}

static int append(struct String *str, char *str_to_appen)
{
	if(!str_to_appen) return -1;

	size_t nl = string_length(str_to_appen);
	if(str->size < DEF_STR){
		if((str->size + nl) < DEF_STR){
			string_copy(&str->base[str->size],str_to_appen,nl);	
			str->size += nl;
			return 0;
		}
		goto allocate_new_mem;
	}

allocate_new_mem:
	if(str->str){
		char *n = (char*)reask_mem(str->str,str->size,(str->size + nl) * sizeof(char));
		if(!n){
			fprintf(stderr,"reask_mem() failed, %s:%d\n",__FILE__,__LINE__-2);	
			return -1;
		}
		str->str = n;
		string_copy(&str->str[str->size],str_to_appen,nl);
			
		str->size += nl;
		return 0;
	}

	str->str = (char*)ask_mem((str->size + nl) * sizeof(char));
	if(!str->str){
		fprintf(stderr,"ask_mem failed, %s:%d\n",__FILE__,__LINE__-2);	
		return -1;
	}

	string_copy(str->str,str->base,str->size);
	string_copy(&str->str[str->size],str_to_appen,nl);
	set_memory(str->base,0,DEF_STR);

	str->size += nl;

	return 0;
}

static uint8_t empty(struct String *str)
{
	if(str->str) return *str->str == '\0';
	return str->base[0] == '\0';
}

static void free_str(struct String *str)
{
	if(str->str){
		cancel_memory(0x0,str->str,str->size);
		str->size = 0;
		return;
	}	

	set_memory(str->base,0,DEF_STR);
	str->size = 0;
}


size_t sys_write(int fd, void *buffer, size_t size)
{
	long r;
	asm volatile("syscall" 
			:"=a"(r)
			:"a"(1), "D"(fd), "S"(buffer), "d"(size)
			:"rcx","r11","memory");

}
