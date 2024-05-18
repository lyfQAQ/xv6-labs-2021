#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void generate_prime(int p[2])
{
    close(p[1]);
    int n;
    read(p[0], &n, sizeof n);

    if (n == -1)
    {
        return;
    }

    printf("prime %d\n", n);
    int p2c[2];
    pipe(p2c);

    if (fork() == 0)
    {
        generate_prime(p2c);
        exit(0);
    }
    else
    {
        close(p2c[0]);
        int i;
        while (read(p[0], &i, sizeof i) && i != -1)
        {
            if (i % n == 0)
                continue;
            write(p2c[1], &i, sizeof i);
        }
        i = -1;
        write(p2c[1], &i, sizeof i);
        wait(0);
    }
}

int main()
{
    int p2c[2];
    pipe(p2c);

    if (fork() == 0)
    {
        // close(p2c[1]);
        generate_prime(p2c);
        exit(0);
    }
    else
    {
        close(p2c[0]);
        // main 线程发 2-35
        int i;
        for (i = 2; i <= 35; i++)
        {
            write(p2c[1], &i, sizeof i);
        }
        // -1 表示发送结束
        i = -1;
        write(p2c[1], &i, sizeof i);
        wait(0);
        exit(0);
    }
}