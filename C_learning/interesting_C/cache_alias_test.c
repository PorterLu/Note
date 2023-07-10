#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
    int fd = shm_open("test-shm", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        printf("Unable to open shm\n");
        exit(1);
    }

    size_t size = 8192;
    ftruncate(fd, size);
    int size_int = size / 4;
    int *a = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    int *b = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    printf("a=%lx\n",a);
    printf("b=%lx\n",b);
    // warmup
    for (int i=0;i<size_int;i++) {
        a[i] = b[i] = 0;
    }
    // test
    clock_t t1 = clock();
    for (int j=0;j<1024*1024;j++) for (int i=0;i<size_int;i++) a[i] = b[i];
    clock_t t2 = clock();
    for (int j=0;j<1024*1024;j++) for (int i=0;i<size_int;i++) a[i] = a[i];
    clock_t t3 = clock();
    printf("t2-t1 = %ld\n",t2-t1);
    printf("t3-t2 = %ld\n",t3-t2);
    shm_unlink("test-shm");
    return 0;
}

