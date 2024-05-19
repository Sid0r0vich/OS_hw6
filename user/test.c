#include "kernel/types.h"
#include "user.h"
#include "kernel/fcntl.h"
#include "kernel/param.h"

int main() {
	int fd = open("hello", O_CREATE);
	int res = symlink("hello", "link");
	printf("res: %d\n", res);
	printf("fd: %d\n", fd);
	
	char buf[MAXPATH];
	res = readlink("link", buf);
	printf("res: %d\n", res);
	printf("buf: %s\n", buf);
		
	return 0;
}
