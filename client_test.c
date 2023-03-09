#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define FIB_DEV "/dev/fibonacci"

int main(int argc, char *argv[])
{
    char write_buf[] = "testing writing";
    int offset = 100; /* TODO: try test something bigger than the limit */

    int fd = open(FIB_DEV, O_RDWR);
    if (fd < 0) {
        perror("Failed to open character device");
        exit(1);
    }
    if (argv[1][0] == '4') { /*execution time*/
        for (int i = 0; i <= offset; i++) {
            long long t1, t2, t3;
            lseek(fd, i, SEEK_SET);
            /*iterative*/
            t1 = write(fd, write_buf, 1);
            /*fast_doubling*/
            t2 = write(fd, write_buf, 2);
            /*fast_doubling_clz*/
            t3 = write(fd, write_buf, 3);

            printf("%lld %lld %lld\n", t1, t2, t3);
        }
    } else {
        for (int i = 0; i <= offset; i++) {
            long long kernel_time, user_time;
            int x = atoi(argv[1]);
            struct timespec t1, t2;

            lseek(fd, i, SEEK_SET);
            clock_gettime(CLOCK_MONOTONIC, &t1);
            kernel_time = write(fd, write_buf, x);
            clock_gettime(CLOCK_MONOTONIC, &t2);

            user_time = (long long) (t2.tv_sec * 1000000000 + t2.tv_nsec) -
                        (t1.tv_sec * 1000000000 + t1.tv_nsec);

            printf("%lld %lld %lld\n", user_time, kernel_time,
                   user_time - kernel_time);
        }
    }

    close(fd);
    return 0;
}
