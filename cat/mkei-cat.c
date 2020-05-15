#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#define NULL ((void *)0)

void output(char *s, off_t len) {
    ssize_t n = 0;
    do {
        n += write(STDOUT_FILENO, s + n, len);
    } while(n < len);
}

char *read_file(int fd, off_t *len) {
    struct stat st;
    if(fd < 0) {
        return NULL;
    }
    fstat(fd, &st);
    if(S_ISDIR(st.st_mode)) {
        return NULL;
    }
    *len = st.st_size;

    return mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
}

int cat_main_stdin() {
    ;
}

int cat_main(int argc, char **argv) {
    int exitcode = 0;
    off_t len;
    for(int i = 0; i < argc; ++i) {
        int fd = open(argv[i + 1], O_RDONLY);
        char *p = read_file(fd, &len);
        if(!p) {
            exitcode = 1;
            continue;
        }
        output(p, len);
        close(fd);
        munmap(p, len);
    }

    return exitcode;
}

int main(int argc, char **argv) {
    if(argc == 1) {
        return cat_main_stdin();
    }
    else {
        return cat_main(argc, argv);
    }
}
