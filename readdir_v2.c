#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

char *filetype(unsigned char type) {
    char *str;

    switch(type) {
        case DT_BLK: str = "block device"; break;
        case DT_CHR: str = "character device"; break;
        case DT_DIR: str = "directory"; break;
        case DT_FIFO: str = "named pipe (FIFO)"; break;
        case DT_LNK: str = "symbolic link"; break;
        case DT_REG: str = "regular file"; break;
        case DT_SOCK: str = "UNIX domain socket"; break;
        case DT_UNKNOWN: str = "unknown file type"; break;
        default: str = "UNKNOWN";
    }

    return str;
}

void traverseDirectory(char *dirname) {
    DIR *parentDir;
    struct dirent *dirent;

    parentDir = opendir(dirname);

    if (parentDir == NULL) {
        printf("Error opening directory '%s'\n", dirname);
        return;
    }

    while ((dirent = readdir(parentDir)) != NULL) {

        // Skip current and parent directory to prevent infinite recursion
        if (strcmp(dirent->d_name, ".") == 0 || strcmp(dirent->d_name, "..") == 0) {
            continue;
        }

        char path[1024];
        snprintf(path, sizeof(path), "%s/%s", dirname, dirent->d_name);

        printf("%s (%s)\n", path, filetype(dirent->d_type));

        // Recursive call if this entry is a directory
        if (dirent->d_type == DT_DIR) {
            traverseDirectory(path);
        }
    }

    closedir(parentDir);
}

int main(int argc, char **argv) {

    if (argc < 2) {
        printf("Usage: %s <dirname>\n", argv[0]);
        exit(-1);
    }

    traverseDirectory(argv[1]);

    return 0;
}