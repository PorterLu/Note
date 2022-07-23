#include <stdio.h>
#include <errno.h>
#include <string.h>

#define debug(M, ...) fprintf(stderr,"DEBUG %s:%d: " M "\n",__FILE__,__LINE__,##__VA_ARGS__)
#define clean_errno() (errno == 0 ? "None" : strerror(errno))
#define log_err(M,...) fprintf(stderr, "[ERROR] (%s:%d errno: %s)" M "\n",__FILE__,__LINE__,clean_errno())


//编译时也可以定义宏，用-D指定
int main(void)
{
	int a = 1, b = 2, c = 3;
	debug("%d %d %d",a ,b ,c);
	printf("%d\n",a/0);
	log_err();
	goto error;
error:
	return 0;
}
