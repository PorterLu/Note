#include <stdio.h>
#include <stdlib.h>
int main() {
	printf("\033[?25l");
	int color = 31;
	for(int i = 0; i < 60; i++) {
		printf("\r");
		printf("\e[1;%dm", color);
		printf("%d", i);
		fflush(stdout);
		sleep(1);
		color = color+1>37? 31: color+1;
	}
	printf("\e[1;0m");
	printf("\rone minute over\n");
	printf("\033[?25h");
	return 0;
}
