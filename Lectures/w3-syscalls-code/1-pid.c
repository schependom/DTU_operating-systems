#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{

    int i = 0;

    while (1)
    {
        printf("[%d] My PID is %d\n", i++, getpid());
        sleep(1);
    }

    return 0;
}
