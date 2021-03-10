/**********************************************************************
 *
 *  SLAB USAGE 
 *
 *
 *  HOW TO BUILD:
 *
 *   - unix:     gcc testmain.c -o testmain
 *   - windows:  cl testmain.c   
 *
 **********************************************************************/


#include "imemslab.c"
#include <stdio.h>

int main(void)
{
	int *ptr;

	/* init kmem interface */
	ikmem_init(0, 0, 0);

	ptr = ikmem_malloc(8);
	assert(ptr);
	*ptr = 1234;

	printf("sizeof(ptr)=%ld\n", ikmem_ptr_size(ptr));
	printf("%d\n", *ptr);

	ptr = ikmem_realloc(ptr, 40);
	assert(ptr);

	printf("sizeof(ptr)=%ld\n", ikmem_ptr_size(ptr));

	ikmem_free(ptr);

	/* clean environment */
	ikmem_destroy();

	return 0;
}


