#include <sys/time.h>
#include <stdio.h>

// gettimeofday(struct timeval *restrict tp, void *restrict tzp);

long time_diff_ms(struct timeval *start, struct timeval *end) {
    return (end->tv_sec - start->tv_sec) * 1000
         + (end->tv_usec - start->tv_usec) / 1000;
}

int main()
{
    struct timeval start;
    struct timeval end;
    long result = 0;

    gettimeofday(&start, NULL);
    long i = 0;
    while (i < 3000000000)
    {
        i++;
    }
    gettimeofday(&end, NULL);
    result = time_diff_ms(&start, &end);
    printf ("%ld\n", result);
    return (0);
}