#ifndef _FREESTAND_H
#define _FREESTAND_H

#include <types.h>

/*SYSTEM CALLS*/
#if defined(__linux__)

	#if defined(__x86_64__)
		#define SYS_WRITE 1
		#define SYS_EXIT 60
	#elif defined(__aarch64__)
		#define SYS_WRITE 64 
	#endif /* arch detection */

#elif defined(__APPLE__)

	#if defined(__x86_64__)
	#elif defined(__aarch64__)
		#define SYS_WRITE 4 
	#endif /* arch detection */

#endif /* os detection*/

/* I/O */
#if defined(__linux__)
#define std_in 0
#define std_out 1
#define std_err 2
#elif defined(_WIN32)

#endif


/*ERRNOs*/

#define INVALID_VALUE 22 /*EINVAL*/

#define DEF_STR 1024

struct String{
	char base[DEF_STR];
	char *str;	
	size_t size;
	int (*append)(struct String*,char *);
	uint8_t (*is_empty)(struct String*);
	void (*close)(struct String*);
};

int init(struct String *str,char *val);
void string_copy(char *dest, char *src, size_t size);
size_t string_length(char *str);
char *find_needle(char *haystack,char *needle);				/*strstr*/
int extract_numbers_from_string(char *buff,size_t size,char *format,...);	/*sscanf*/

/* memset alias */
void set_memory(void *ptr,int value, size_t size);

/* memcpy alias */
int copy_memory(void *dest, void* src, size_t size);

/*conversions */
int long_to_string(long n, char *buff);
int copy_to_string(char *buff,size_t size,char *format,...); /*snprintf*/
int double_to_string(double d, char *buff);


/*helpers*/
size_t number_of_digit(long n);

/*SYS CALLS*/
long sys_write(int fd, void *buffer, size_t size);
void sys_exit(long exit_code);

/*print to screen*/
void display_to_stdout(char *format_str,...);

/*ctype.h style func*/
int is_digit(int c);
int is_space(int c);
int is_alpha(int c);

/*math.h style func */
uint32_t power(uint32_t n, int pow);
#endif

