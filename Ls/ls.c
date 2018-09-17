#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <pwd.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#define NRM  "\x1B[0m"
#define BLU  "\x1B[34m"
#define CYN  "\x1B[36m"

struct s_queue {
    int reserved;
    int sz;
    char** strings;
};

void init(struct s_queue* queue) {
    queue->sz = 0;
    queue->reserved = 100;
    queue->strings = (char**)malloc(queue->reserved * sizeof(char*));
}

char* pop(struct s_queue* queue) {
    queue->sz--;
    char* ret = (char*)malloc(strlen(queue->strings[queue->sz]) + 1);
    strcpy(ret, queue->strings[queue->sz]);
    free(queue->strings[queue->sz]);
    return ret;
}

void push(struct s_queue* queue, char* string) {
    if(queue->sz == queue->reserved) {
        queue->reserved *= 2;
        queue->strings = (char**)realloc(queue->strings, queue->reserved * sizeof(char*));
    }

    queue->strings[queue->sz] = (char*)malloc(strlen(string) + 1);
    strcpy(queue->strings[queue->sz], string);
    queue->sz++;
}

bool is_empty(struct s_queue* queue) {
    if(queue->sz == 0) return true;
    else return false;
}
struct s_queue queue;

char* get_mode_string(int mode) {
    char* mode_string = (char*)malloc(11);
    for(int i = 0; i < 11; ++i) mode_string[i] = '-';

    if(S_ISLNK(mode)) mode_string[0] = 'l';
    else if(S_ISDIR(mode)) mode_string[0] = 'd';

    if(mode & S_IRUSR) mode_string[1] = 'r';
    if(mode & S_IWUSR) mode_string[2] = 'w';
    if(mode & S_IXUSR) mode_string[3] = 'x';
    if(mode & S_IRGRP) mode_string[4] = 'r';
    if(mode & S_IWGRP) mode_string[5] = 'w';
    if(mode & S_IXGRP) mode_string[6] = 'x';
    if(mode & S_IROTH) mode_string[7] = 'r';
    if(mode & S_IWOTH) mode_string[8] = 'w';
    if(mode & S_IXOTH) mode_string[9] = 'x';
    mode_string[10] = '\0';

    return mode_string;
}

char* get_time_string(unsigned long seconds) {
    const int size = strlen("16-03-2016 18:47:45.123456789") + 1;
    char* time_string = (char*)malloc(size);
    struct tm lt;
    localtime_r(&seconds, &lt);
    strftime(time_string, size, "%c", &lt);

    return time_string;
}

void list(char* dir_name) {
    DIR* directory = NULL;
    struct dirent* dirent_struct = NULL;
    struct stat st;
    directory = opendir(dir_name);
    char* cur_filename = (char*)malloc(strlen(dir_name) + 257);
    strcpy(cur_filename, dir_name);
    printf("%s%s%s\n", CYN, cur_filename, NRM);

    if(directory) {
        while((dirent_struct = readdir(directory)) != NULL) {
            if(strcmp(dirent_struct->d_name, ".") == 0) continue;
            if(strcmp(dirent_struct->d_name, "..") == 0) continue;

            strcat(cur_filename, dirent_struct->d_name);
            strcat(cur_filename, "\0");
            int ret = lstat(cur_filename, &st);
            if(ret == 0) {
                char* time_str = get_time_string(st.st_mtime);
                char* mode_str = get_mode_string(st.st_mode);

                printf("%s\t", mode_str);

                struct passwd* pwd_res = getpwuid(st.st_uid);
                if (pwd_res == NULL) {
                    printf("unknown_user:");
                }
                else {
                    printf("%s:", getpwuid(st.st_uid)->pw_name);
                }
                pwd_res = getpwuid(st.st_gid);
                if (pwd_res == NULL) {
                    printf("unknown_group\t");
                }
                else {
                    printf("%s\t", getpwuid(st.st_gid)->pw_name);
                }
                printf("%ld\t", st.st_size);
                printf("%s\t", time_str);
                if(S_ISDIR(st.st_mode)) {
                    push(&queue, cur_filename);
                    printf("%s", BLU);
                }
                printf("%s%s\t", dirent_struct->d_name, NRM);

                free(time_str);
                free(mode_str);
            }
            else {
                printf("stat = %d, errno = %d\n", ret, errno);
            }
            printf("\n");
            strcpy(cur_filename, dir_name);
        }
        closedir(directory);
    }
    free(cur_filename);
    printf("\n");
}

int main(int argc, char** argv) {
    char* directory;
    if(argv[1] != NULL)
        directory = argv[1];
    else
        directory = (char*)get_current_dir_name();

    init(&queue);
    push(&queue, directory);
    while(!is_empty(&queue)) {
        char* path = pop(&queue);
        if(path[strlen(path) - 1] != '/') {
            char* tmpBuff = (char*)realloc(path, strlen(path) + 4);
            if(tmpBuff != NULL) path = tmpBuff;
            else printf("\nEE: error in realloc()\n");
            strcat(path, "/\0");
        }
        list(path);
        free(path);
    }

    return 0;
}

