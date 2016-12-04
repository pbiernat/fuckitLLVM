#include<stdlib.h>

int
main(void)
{
	free((void*)1);
	return 0;  /* unreachable */
}
