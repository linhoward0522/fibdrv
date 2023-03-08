#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define FIB_DEV "/dev/fibonacci"

int main()
{
    char write_buf[] = "testing writing";
    int offset = 100; /* TODO: try test something bigger than the limit */

    int fd = open(FIB_DEV, O_RDWR);
    if (fd < 0) {
        perror("Failed to open character device");
        exit(1);
    }
    for (int i = 0; i <= offset; i++) {
        long long t1, t2, t3;
        lseek(fd, i, SEEK_SET);
        /*iterative*/
        t1 = write(fd, write_buf, 0);
        /*fast_doubling*/
        t2 = write(fd, write_buf, 1);
        /*fast_doubling_clz*/
        t3 = write(fd, write_buf, 2);

        printf("%lld %lld% lld\n", t1, t2, t3);
    }

    close(fd);
    return 0;
}
