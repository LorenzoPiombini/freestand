#include "memory.h"
#include <freestand.h>

int main(void)
{

	/*allocate memory*/
	if(create_arena(1) == -1) sys_exit(-1);
	struct Arena ar;
	ar.mem.p = get_arena(0x0);
		
    /*STRING USAGE EXAMPLES*/

	/*you must pass a buffer to string init*/
	struct Buffer buf = {(void*)ar.mem.p,string_length("this is a string with my library")};
	struct String p;
	if(init(&p,"this is a string with my library",&buf) == -1) sys_exit(-1);

	
	/*you can do it this way too*/
	char *message = "i like working on strings";
	struct String p2;
	int l = string_length(message);	
	char b[l];

	buf.p = (void*)&b[0];
	buf.size = l;

	if(init(&p2,message,&buf) == -1) sys_exit(-1);
	display_to_stdout("%S\n",p2);


	if(p.string_compare(&p2,&p) == 0)
		display_to_stdout("string '%S' and string '%S' are =\n",p2,p);
	else
		display_to_stdout("string '%S' and string '%S' are not =\n",p2,p);


	return 0;
}
