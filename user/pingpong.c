#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main()
{
    int p2c[2], c2p[2];
    pipe(p2c);
    pipe(c2p);

    if (fork() == 0)
    {
        close(p2c[1]);
        close(c2p[0]);
        char c;
        read(p2c[0], &c, sizeof c);
        printf("%d: received ping\n", getpid());
        write(c2p[1], &c, sizeof c);
        exit(0);
    }
    else
    {
        close(p2c[0]);
        close(c2p[1]);
        char c = 'c';
        write(p2c[1], &c, sizeof c);
        read(c2p[0], &c, sizeof c);
        printf("%d: received pong\n", getpid());
        wait(0);
        exit(0);
    }
}