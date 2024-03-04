#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(const char* path, char* target) {
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;
    if ((fd = open(path, 0)) < 0) {
        fprintf(2, "find cannot open\n");
        exit(1);
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
        if(de.inum == 0)
        continue;
        if (strcmp(de.name, ".") == 0
                || strcmp(de.name, "..") == 0)
            continue;
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        if(stat(buf, &st) < 0){
            printf("ls: cannot stat %s\n", buf);
            continue;
        }
        if (strcmp(de.name, target) == 0) {
            printf("%s\n", buf);
        }
        if (st.type == T_DIR) {
            find(buf, target);
        }
    }
    close(fd);


}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(2, "find error\n");
        exit(1);
    }

    find(argv[1], argv[2]);
    exit(0);
}

