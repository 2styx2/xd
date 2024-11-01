#include "xd.h"
#include <errno.h>
#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *file;

void addDirsFile() {
    char *dirName = "/.xd_dirs";
    file = (char *)malloc(strlen(getenv("HOME")) + strlen(dirName) + 1);
    strcpy(file, getenv("HOME"));
    strcat(file, dirName);
    FILE *fp = fopen(file, "a");
    if (fp == NULL) {
        fprintf(stderr, "Error opening file: %s\n", strerror(errno));
        exit(1);
    }
    fclose(fp);
}

void changeDir(const char *name) {
    FILE *fp = fopen(file, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error opening file: %s\n", strerror(errno));
        exit(1);
    }
    char line[256];
    while (fgets(line, sizeof(line), fp) != NULL) {
        char buf_name[256], buf_path[256];
        if (sscanf(line, "%s %s", buf_name, buf_path) == 2) {
            if (strcmp(buf_name, name) == 0) {
                printf("%s\n", buf_path);
                fclose(fp);
                return;
            }
        } else {
            fprintf(stderr, "Error reading file\n");
        }
    }
    fclose(fp);
    fprintf(stderr, "Directory '%s' not found\n", name);
}

void addDir(const char *name, const char *path) {
    FILE *fp = fopen(file, "a");
    if (fp == NULL) {
        fprintf(stderr, "Error opening file: %s\n", strerror(errno));
        exit(1);
    }
    fprintf(fp, "%s %s\n", name, path);
    fclose(fp);
    printf("Added directory %s with path %s\n", name, path);
}

void listDirs() {
    FILE *fp = fopen(file, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error opening file: %s\n", strerror(errno));
        exit(1);
    }
    char name[256], path[256];
    while (fscanf(fp, "%s %s\n", name, path) != EOF) {
        printf("%s: %s\n", name, path);
    }
    fclose(fp);
}

void static usage(const char *name) {
    fprintf(stderr, "Usage: %s <command>\n", name);
    fprintf(stderr, "Commands:\n");
    fprintf(stderr, "  add <name> <path> - add a directory\n");
    fprintf(stderr, "  list              - list all directories\n");
    fprintf(stderr, "  <name>            - change to a directory\n");
    fprintf(stderr, "  help              - show this help\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        usage(argv[0]);
        return 1;
    }
    addDirsFile();
    if (strcmp(argv[1], "add") == 0) {
        if (argc != 4) {
            fprintf(stderr, "Usage: %s add <name> <path>\n", argv[0]);
            return 1;
        }
        addDir(argv[2], argv[3]);
    } else if (strcmp(argv[1], "list") == 0) {
        listDirs();
    } else if (strcmp(argv[1], "help") == 0) {
        usage(argv[0]);
    } else {
        changeDir(argv[1]);
    }
    free(file);
    return 0;
}
