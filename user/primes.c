#include "kernel/types.h"
#include "user/user.h"

void childexec(int fd_r, int fd_w) {
    close(fd_w);
    int p;
    if (read(fd_r, &p, sizeof(int)) != sizeof(int)) {
        close(fd_r);
        exit(0);
    }
    printf("prime %d\n", p);
    int n;
    
    int p1[2];
    pipe(p1);
    
    
    if (fork() == 0) 
        childexec(p1[0], p1[1]);
    close(p1[0]);
    while (read(fd_r, &n, sizeof(int)) != 0) {
        if (n % p)
            write(p1[1], &n, sizeof(int));
    }
    close(fd_r);
    close(p1[1]);
    wait(0);
    exit(0);

}

int main() {
    int p[2];
    pipe(p);
    
    if (fork() == 0) {
        childexec(p[0], p[1]);
    }
    close(p[0]);
    int i;
    for (i = 2; i <= 35; ++i)
        write(p[1], &i, sizeof(int));
    
    close(p[1]);
    wait(0);
    exit(0);

}

