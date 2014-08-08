#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main()
{	FILE *fp;
	char *filename = "test.c";
	errno = 0;

	if((fp = fopen(filename, "r")) == NULL) {
		printf("open file %s failed, errno is %d\n", filename, errno);
	} else {
		printf("open file %s successfully\n", filename);
	}
}
