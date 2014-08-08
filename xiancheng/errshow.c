#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main()
{	FILE *fp;
	char *filename = "test.c";
	
	if((fp = fopen(filename, "r")) == NULL) {
		//printf("cann't open the file %s. reason: %s\n", filename, strerror(errno));
		perror("fopen");
		exit(-1);
	} else {
		printf("open the %s successfully!\n", filename);
	}

	return 0;
}
