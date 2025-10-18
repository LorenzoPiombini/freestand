#include <stdio.h>
#include "memory.h"
#include <freestand.h>

int main(void)
{
	char mem[32];
	set_memory(mem,0x06,32);

	string_copy(mem,"hey! did you like venice?",string_length("hey! did you like venice?"));
	if(create_arena(200) == -1) return -1;
	
	sys_write(1,mem,string_length("hey! did you like venice?"));
	struct arena a;
	set_memory(&a,0,sizeof(struct arena));

	a.p = get_arena(200);
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
	printf("%s\n",s);
	return 0;
}
