#ifndef _FREESTAND_H
#define _FREESTAND_H

/*SYSTEM CALLS*/
#if defined(__x86_64__)
	#define SYS_WRITE 1
#elif defined(__aarch64__)
	#define SYS_WRITE 4 
#endif

/* I/O */
#if defined(__linux__)
#define std_out 1
#define std_err 2
#elif defined(_WIN32)

#endif

/* TYPES */ 
#if defined(__linux__)
typedef 	char			int8_t;
typedef 	unsigned char	uint8_t;
typedef 	unsigned short	uint16_t;
typedef 	short			int16_t;
typedef 	unsigned int	uint32_t;
typedef 	int				int32_t;
typedef 	unsigned long	uint64_t;
typedef 	long			int64_t;
typedef 	unsigned long 	size_t;
typedef		long int		file_offset; /*OFFSET*/
typedef 	long long		process_id; /*PROCESS ID*/

/*signed short max and min*/
#  define SHRT_MIN	(-32768)
#  define SHRT_MAX	32767

/* unsigned short int max.  (Minimum is 0.)  */
#  define USHRT_MAX	65535

/* signed int min and max  */
#  define INT_MIN	(-INT_MAX - 1)
#  define INT_MAX	2147483647

/* unsigned char max (minimum is 0*/
#  define UCHAR_MAX 	255	

/* unsigned int' max  (Minimum is 0.)  */
#  define UINT_MAX	4294967295

/* signed long int min and max */
#  if __WORDSIZE == 64
#   define LONG_MAX	9223372036854775807L
#else
#   define LONG_MAX	2147483647L
#endif

#define LONG_MIN	(-LONG_MAX - 1L)

/* unsigned long max  (Minimum is 0.)  */
#  if __WORDSIZE == 64
#   define ULONG_MAX	18446744073709551615UL
#  else
#   define ULONG_MAX	4294967295UL
#  endif

#elif defined(__APPLE__)
typedef 	unsigned char	uint8_t;
#endif



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

/* memset alias */
void set_memory(void *ptr,int value, size_t size);

/* memcpy alias */
int copy_memory(void *dest, void* src, size_t size);

/*conversions */
int long_to_string(long n, char *buff);


/*helpers*/
size_t number_of_digit(long n);

/*SYS CALLS*/
long sys_write(int fd, void *buffer, size_t size);

/*print to screen*/
void display_to_stdout(char *format_str,...);
#endif
