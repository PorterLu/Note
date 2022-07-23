#include <stdio.h>
#include <string.h>

int duffs_device(char *from, char *to, int count)
{
	int n = (count + 7)/8; //向上取整
	if(n <= 0)
		return -1;
	switch(count%8){
		case 0:
			*to++ = *from++;
			do{
			case 7: *to++ = *from++;
			case 6: *to++ = *from++;
			case 5: *to++ = *from++;
			case 4: *to++ = *from++;
			case 3: *to++ = *from++;
			case 2: *to++ = *from++;
			case 1: *to++ = *from++;
			}while(--n>0);
	}

	//拷贝每一个字符的操作都是相同的，所以duffs将其每8个为一组进行展开，从而获得加速
	return count;
}

int main(void){
	char from[20] = "hello world\n";
	char to[20];
	duffs_device(from,to,20);
	printf("%s",to);
	return 0;
}
