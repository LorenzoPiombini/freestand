#include <stdarg.h>
#include "freestand.h"
#include "memory.h"

/*static functions to manage String struct*/
static void free_str(struct String *str);
static i8 empty(struct String *str);
static int append(struct String *str,char *str_to_appen);

i64 error_value = 0;
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
		long iw = (long)( (long)v << 56 | (long)v << 48 | (long)v << 40 | (long)v << 32 | v << 24 | v << 16 | v << 8 | v);
		size_t i;
		for(i = 0; i < (size / word);*p_long = iw, i++, p_long++);	
		break;
	}
	default:
	break;
	}
}

/*memcpy*/
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
	set_memory(str,0,sizeof(struct String));
	
	if(!val){
		set_memory(str->base,0,DEF_STR);
		str->str = 0x0;
		str->append = append;
		str->is_empty = empty;
		str->close = free_str;
		str->size = 0;
		return 0;
	}

	size_t i;
	char *p = val;
	for(i = 0; *p != '\0'; i++,p++);

	str->size = i;
	if(str->size >= DEF_STR){
		str->str = (char*)ask_mem((str->size)*sizeof(char));
		if(!str->str){
			display_to_stdout("ask_mem failed, %s:%d\n",__FILE__,__LINE__-2);	
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
	size_t i;
	for(i = 0; i < size;i++)
		dest[i] = src[i];
}

size_t string_length(const char *str)
{
	size_t i;
	char *p = (char*)str;
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
			display_to_stdout("reask_mem() failed, %s:%d\n",__FILE__,__LINE__-2);	
			return -1;
		}
		str->str = n;
		string_copy(&str->str[str->size],str_to_appen,nl);
			
		str->size += nl;
		return 0;
	}

	str->str = (char*)ask_mem((str->size + nl) * sizeof(char));
	if(!str->str){
		display_to_stdout("ask_mem failed, %s:%d\n",__FILE__,__LINE__-2);	
		return -1;
	}

	string_copy(str->str,str->base,str->size);
	string_copy(&str->str[str->size],str_to_appen,nl);
	set_memory(str->base,0,DEF_STR);

	str->size += nl;

	return 0;
}
static i8 empty(struct String *str)
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

size_t number_of_digit(long n)
{
	if (n < 10)
	{
		return 1;
	}
	else if (n >= 10 && n < 100)
	{
		return 2;
	}
	else if (n >= 100 && n < 1000)
	{
		return 3;
	}
	else if (n >= 1000 && n < 10000)
	{
		return 4;
	}
	else if (n >= 10000 && n < 100000)
	{
		return 5;
	}
	else if (n >= 100000 && n < 1000000)
	{
		return 6;
	}
	else if (n >= 1000000 && n < 1000000000)
	{
		return 7;
	}
	else if (n >= 1000000000)
	{
		return 10;
	}

	return -1;
}

long sys_write(int fd, void *buffer, size_t size)
{
#if defined(__x86_64)
	long r;
	__asm__ volatile ("syscall" 
			:"=a"(r)
			:"a"(SYS_WRITE), "D"(fd), "S"(buffer), "d"(size)
			:"rcx","r11","memory");
	return r;
#elif defined(__aarch64__)
	long ret = 0;
	register long x16 __asm__("x16") = 0x2000004;
	register long x0 __asm__("x0") = fd;
	register void *x1 __asm__("x1") = buffer;
	register unsigned long x2 __asm__("x2") = size;
	__asm__ volatile ("svc #0x80"
			:"+r"(x0)
			:"r"(x1), "r"(x2), "r"(x16)
			:"memory");
	ret = x0;
	return ret;
#endif
}

void sys_exit(long exit_code)
{
	__asm__ volatile("movq %0, %%rdi\n"
				     "movq %1, %%rax\n"	
					 "syscall\n" 
						:
						:"r"(exit_code), "i"(SYS_EXIT)
						:"rax","rdi","memory");
}

int double_to_string(double d, char *buff){
	
	long integer = (long)d;
	double fraction = d - (double)integer;

	long_to_string(integer,buff);

	buff[string_length(buff)] = '.';

	if (d < 0){
		fraction *= -1;
		integer *= -1;
	}

	int i;
	int j = string_length(buff);
	int zeros_after_point = 15;
	if(d < 0) zeros_after_point -= 1;

	for(i = 0; i < zeros_after_point; i++){
		fraction *= 10;
		buff[j] =  ((int)fraction + '0');
		fraction = (double)(fraction - (int)fraction);
		j++;
	}

	return 0;
}

int long_to_string(long n, char *buff){
	
	int pos = 0;
	if(n < 0){
		pos++;
		buff[0] = '-';
		n *= -1;
	}
	pos += number_of_digit(n)-1;
	if(n == 0){
		buff[pos] = (char)(n + (int)'0');
	}
	while(n > 0){
		int m = n % 10;
		if(pos >= 0)
			buff[pos] = (char)(m + (int)'0');

		n /= 10;	
		pos--;
	}
	return 0;
}

long string_to_long(char *str)
{
	size_t l = string_length(str);
	if(is_space(str[l-1])){
		str[l-1] = '\0';
		l--;
	}

	int at_the_power_of = l - 1;	
	long converted = 0;
	int negative = 0;
	for(; *str != '\0';str++){
		if(*str == '-'){
			at_the_power_of--;
			negative = 1;
			continue;
		}

		if(is_alpha(*str)) {
			error_value = INVALID_VALUE;
			return -1;
		}


		long n = (long)*str - '0';
		if(at_the_power_of >= 0)
			converted += (long)((int)power(10.00,at_the_power_of))*n;
		at_the_power_of--;
	}

	return negative == 0 ? converted : converted * (-1);
}
double string_to_double(char *str)
{
	int comma = 0;
	int position = 0;
	char num_buff[30];
	set_memory(num_buff,0,30);

	int i;
	for(i = 0; *str != '\0';str++){
		if (i == 30) break;
		if(*str == '.') {
			comma = 1;
			continue;
		}

		if(is_alpha(*str)){
			error_value = INVALID_VALUE;
			return -1;
		}

		if(i < 30)
			num_buff[i] = *str;

		if(comma) position++;
		i++;
	}

	long l = string_to_long(num_buff);
	if(comma){
		return ((double)l / power(10,position));
	}else{
		return (double) l;
	}
}

/*printf alike*/
void display_to_stdout(char *format_str,...)
{
	va_list first,second;
	va_start(first,format_str);
	va_start(second,format_str);
	
	char *p = format_str;
	/*count byte for buffer*/

	long count = 0, i;
	long spec = 0;
	/*in the loop i counts the bytes needed exept the format specifiers*/
	for(i = 0; *p != '\0';i++,p++){
		if(*p == '%'){
			p++;
			switch(*p){
			case 's':
			{
				char *s = va_arg(first,char*);
				count += string_length(s);
				spec++;
				break;				
			}
			case 'd':
			{
				
				char n_buff[100];
				set_memory(n_buff,0,100);
				long n = (long) va_arg(first,int);	
				long_to_string(n,n_buff);
				count += string_length(n_buff);
				spec++;
				break;
			}
			default:
				break;
			}
		}
	}	
	
	va_end(first);
	long buffer_bytes = string_length(format_str) - (spec*2) + count + 1;
	char buff[buffer_bytes];
	set_memory(buff,0,buffer_bytes);
	
	for(i = 0; *format_str != '\0';format_str++){
		if(*format_str == '%'){
			format_str++;		
			switch(*format_str){
			case 's':
			{
				char *s = (char *)va_arg(second,char *);
				string_copy(&buff[i],s,string_length(s));
				i += string_length(s);
				continue;
			}
			case 'd':
			{
				char n_buff[100];
				set_memory(n_buff,0,100);
				long n = (long) va_arg(second,int);	
				long_to_string(n,n_buff);
				string_copy(&buff[i],n_buff,string_length(n_buff));
				i += string_length(n_buff);
				continue;
			}
			default:
			break;
			}
		}
		buff[i] = *format_str;
		i++;
	}

	sys_write(1,buff,buffer_bytes-1);
	va_end(second);
}

int copy_to_string(char *buff,size_t size,char *format,...)
{
	if(string_length(format) > size){
		display_to_stdout("buffer is not big enough %s:%d.\n",__FILE__,__LINE__);
		return -1;
	}

	va_list list; 
	long l = 0;
	char *string = 0x0;
	int precision = 0;
	double d = 0;

	va_start(list,format);
	size_t j;
	for(j = 0; *format != '\0';format++){
		if(*format == '%'){
			format++;
			
			for(;;){
				switch(*format){
				case 's':
				{
					string = va_arg(list,char*);
					for(; *string != '\0'; string++){
						if(j < size)
							buff[j] = *string;
						j++;
					}
					break;
				}
				case 'l':
					format++;
					continue;
				case 'u':
				case 'd':
				{
					l = va_arg(list,int);
					size_t sz = number_of_digit(l);
					char num_str[sz+1];
					set_memory(num_str,0,sz+1);
					long_to_string(l,num_str);	
					int i;
					for(i = 0; num_str[i] != '\0'; i++){
						if(j < size)
							buff[j] = num_str[i];
						j++;
					}
					break;
				}
				case 'f':
				{
					d = va_arg(list,double);
					
					size_t sz = number_of_digit(d < 0 ? (int)d * -1 : (int)d);
					/* 1 for the sign 15 digits after the . ,the . and '\0'  at the end*/
					char num_str[sz+18];
					set_memory(num_str,0,sz+18);
					double_to_string(d,num_str);	
					int i,pc,com;
					for(i = 0, pc = 0, com = 0; num_str[i] != '\0'; i++){
						if(precision == pc){
							if(((int)num_str[i] - '0') > 5){
								if(((int)buff[j-1] - '0') == 9){
									buff[j-1] = '0';
								}else{
									int n = buff[j-1] - '0';
									buff[j-1] = (char)++n + '0';
								}
							}
							break;
						}
						if(com) pc++;
						if(num_str[i] == '.') com = 1;

						if(j < size)
							buff[j] = num_str[i];
						j++;
					}
					break;
				}
				case '.':
				{
					char num[4];
					set_memory(num,0,4);
					int k = 0;
					while(is_digit(*(++format))){
						num[k] = *format;	
						k++;
					}	

					precision = string_to_long(num);
					if(error_value == INVALID_VALUE){
						va_end(list);
						return -1;
					}

					continue;
				}
				default:
					break;
				}
				format++;
				break;
			}
		}	
		if(*format == '\0') break;
		if(j < size)
			buff[j] = *format;
		j++;
	}
	va_end(list);
	return 0;
}
/*sscanf*/
int extract_numbers_from_string(char *buff,size_t size,char *format,...)
{
	if(find_needle(format,"%s")){
		display_to_stdout("invalid format string, unexpected '%%s'\nformat specifier acceptted %%ld %%d %%u\n");	
		return -1;
	}

	va_list list;
	va_start(list,format);
	size_t i;
	i8 is_long = 0;
	for(i = 0; *format != '\0'; format++){
		if(*format == '%'){
			format++;
			if(*format == '\0')break;
			for(;;){
				switch(*format){
				case 's':
					va_end(list);
					return -1;
				case 'l':
					format++;
					is_long = 1;
					continue;
				case 'd':
					char number[15];
					set_memory(number,0,15);
					int j = 0;
				
					if(is_digit(buff[i])){
						while(is_digit(buff[i])){
							if(j < 15){
								number[j] = buff[i];
								j++;
							}else{
								va_end(list);
								display_to_stdout(
									"number is too large: %s, has more than 15 digit\n",
									number);
								return -1;
							}
							i++;
							
							if( i >= size){
								va_end(list);
								return -1;
							}
						}		
					}else{

						while(!is_digit(buff[i])){
							i++;
							if(i > size){
								va_end(list);
								return -1;
							}
						}

						while(is_digit(buff[i])){
							if(j < 15){
								number[j] = buff[i];
								j++;
							}else{
								va_end(list);
								display_to_stdout(
									"number is too large: %s, has more than 15 digit\n",
									number);
								return -1;
							}
							i++;
							
							if(i > size){
								va_end(list);
								return -1;
							}
						}
					}
					long l = string_to_long(number);
					if(error_value == INVALID_VALUE){
						va_end(list);
						display_to_stdout("string to number conversion failed, %s,%d.\n",
								__FILE__,__LINE__-4);
						return -1;
					}

					if(is_long){
						long *n = va_arg(list,long*);
						*n = l;
					}else{
						int *n = va_arg(list,int*);
						*n = l;
					}
					break;
				default:
					break;
				}
				format++;
				break;
			}
		}

		if(*format == '\0') break;
	}
	va_end(list);
	return 0;
}

int is_punct(int c)
{
	if(is_space(c)) return 0;
	if(is_digit(c)) return 0;
	if(is_alpha(c)) return 0;

	return c >= 0x20 && c <= 0x7e;
}

int is_space(int c)
{
	return c == 0x0b || c == 0x09 || c == 0x0a || c == 0x0c || c == 0x0d || c == 0x20;
}

int is_alpha(int c)
{
	return (c >= 0x41 && c <= 0x5a) || (c >= 0x61 && c <= 0x7a);
}
int is_digit(int c)
{
	return c >= 0x30 && c <= 0x39;
}

ui32 power(ui32 n, int pow)
{
	ui32 a = n;
	if(pow == 0) return 1;
	
	int i;
	for(i = 1; i < pow; i++){
		a *= n;
	}

	return a;
}


char *find_needle(const char *haystack,const char *needle)
{
	
	const char *hay = &haystack[0];
	int b = 0;
	int i;
	for(i = 0; hay[i] != '\0';i++){
		const char *h = &hay[i];
		const char *p = &needle[0];
		for(;*p != '\0';p++){
			if(*p != *h){
				b = 1;
				break;
			}
			h++;
		}

		if(b){
			b = 0;
			continue;
		}

		return (char *)&haystack[i];		
	}

	return 0x0;
}

int complementary_span(const char *s, const char *reject){
	
	char *f = find_needle(s,reject);
	if(!f) return -1;

	return (int)((unsigned char*)f -(unsigned char*)s);
}
int string_compare(char *src, char *dest, size_t size){

	size_t src_l = string_length(src);
	size_t dest_l = string_length(dest);
	if(src_l != dest_l) return 1;
	if(src_l != size || dest_l != size) return 1;

	size_t i;
	for(i = 0;i < size; i++)
		if(src[i] != dest[i]) return 1;

	return 0;
}
