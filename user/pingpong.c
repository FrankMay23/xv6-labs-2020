#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char* argv[]) {
    int p1[2];
    int p2[2];
    char buf = 'a';
    pipe(p1);
    pipe(p2);

    int forkpid = fork();
    int exit_status = 0;

    if (forkpid < 0) {
        fprintf(2, "fork wrong!\n");
        close(p2[0]);
        close(p2[1]);
        close(p1[0]);
        close(p1[1]);
        exit(1);
    } else if(forkpid == 0) {
        close(p1[1]);
        close(p2[0]);
        
        if (read(p1[0], &buf, sizeof(char)) != sizeof(char)) {
            fprintf(2, "child read error!\n");
            exit_status = 1;
        } else {
            fprintf(1, "%d: received ping\n", getpid());
        }
        
        
        if (write(p2[1], &buf, sizeof(char)) != sizeof(char)) {
            fprintf(2, "child write error!\n");
            exit_status = 1;
        }
        close(p1[0]);
        close(p2[1]);
        exit(exit_status);
    } else {
        close(p1[0]);
        close(p2[1]);
        if (write(p1[1], &buf, sizeof(char)) != sizeof(char)) {
            fprintf(2, "parent write error!\n");
            exit_status = 1;
        } 
        
        if (read(p2[0], &buf, sizeof(char)) != sizeof(char)) {
            fprintf(2, "parent read error!\n");
            exit_status = 1;
        } else {
            fprintf(1, "%d: received pong\n", getpid());
        }
        close(p1[1]);
        close(p2[0]);
        wait(0);
        exit(exit_status);
    } 

}




