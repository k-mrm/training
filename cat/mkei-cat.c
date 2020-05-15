#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#define NULL ((void *)0)

char *read_file(int fd, off_t *len) {
    struct stat st;
    if(fd < 0) {
        return NULL;
    }
    fstat(fd, &st);
    *len = st.st_size;

    return mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
}

int cat_main(int argc, char **argv) {
    int exitcode;
    off_t len;
    for(int i = 0; i < argc; ++i) {
        int fd = open(argv[i + 1], O_RDONLY);
        char *p = read_file(fd, &len);
        if(!p) {
            exitcode = 1;
            continue;
        }
        write(STDOUT_FILENO, (void *)p, len);
        close(fd);
        munmap(p, len);
    }

    return exitcode;
}

int main(int argc, char **argv) {
    if(argc == 1) {
        /* TODO */
        return 0;
    }
    else {
        return cat_main(argc, argv);
    }
}
