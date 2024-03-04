#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"

int getline(char* buf, int nbuf) {
    memset(buf, 0, nbuf);
    char c;
    int cc, i;
    for (i = 0; i+1 < nbuf; ) {
        cc = read(0, &c, 1);
        if (cc < 1)
            break;
        buf[i] = c;
        if (buf[i] == '\n')
            break;
        i++;
    }
    buf[i] = 0;
    if(buf[0] == 0)
        return -1;
    return 0;
}

int main(int argc, char* argv[]) {
    char buf[100];
    char* xargv[MAXARG] = {0};
    for (int i = 1; i < argc; ++i)
        xargv[i-1] = argv[i];
    while (getline(buf, sizeof(buf)) != -1) {
        if (fork() == 0) {
            char *p = buf, *ep;
            --argc;
            while (*p != 0) {
                if (*p == ' ')
                    continue;
                ep = p;
                while (*ep != ' ' && *ep != 0)
                    ++ep;
                xargv[argc++] = p;
                if (*ep == 0) {
                    break;
                }
                *ep = 0;
                
                p = ep + 1;
                
            }
            exec(xargv[0], xargv);
            exit(0);
        } else
            wait(0);
    }
    exit(0);
}