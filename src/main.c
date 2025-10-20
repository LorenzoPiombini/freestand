#include "memory.h"
#include <freestand.h>

int main(void)
{

	char *p = "this is my name";
	char mem[32];
	set_memory(mem,0,32);

	string_copy(mem,"hey! did you like venice?",string_length("hey! did you like venice?"));
	if(create_arena(200) == -1) sys_exit(-1);
	

	if(string_compare(mem,p,string_length(p)) == 0)
		display_to_stdout("string '%s' and string '%s' are =\n",mem,p);
	else
		display_to_stdout("string '%s' and string '%s' are not =\n",mem,p);


	if(!find_needle(mem,p)) 
		display_to_stdout("%s not found in %s\n",p,mem);
	else
		display_to_stdout("%s found in %s\n",p,mem);


	struct arena a;
	set_memory(&a,0,sizeof(struct arena));

	a.p = get_arena(0x0);
	a.size = 200;

	char *m = &((char*)a.p)[a.bwritten];
	set_memory(m,0,10);
	a.bwritten +=  10;

	char *s = &((char*)a.p)[a.bwritten];
	set_memory(s,0,10);
	a.bwritten +=  10;

	copy_memory(m,"hey!",5);
	copy_memory(s,m,10);
	
	struct arena ar;
	copy_memory(&ar,&a,sizeof(struct arena));
	
	char buffer[] = "Hey what is up\n I hope you are okay cause here is a little rainy\n";
	struct String st;
	init(&st,buffer);

	copy_memory(&((char*)a.p)[a.bwritten],buffer,string_length(buffer));
	a.bwritten += string_length(buffer);
	set_memory(m,0,10);
	display_to_stdout("%s\n",s);
	sys_exit(0);
	return 0;
}
