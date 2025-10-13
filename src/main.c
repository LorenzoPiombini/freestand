#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <freestand.h>

int main(void)
{
	char mem[32];
	set_memory(mem,0x06,32);

	if(create_arena(200) == -1) return -1;

	struct arena a;
	set_memory(&a,0,sizeof(struct arena));

	a.p = get_arena(NULL);
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
	copy_memory(&((char*)a.p)[a.bwritten],buffer,strlen(buffer));
	a.bwritten += strlen(buffer);
	set_memory(m,0,10);
	printf("%s",s);
	return 0;
}
