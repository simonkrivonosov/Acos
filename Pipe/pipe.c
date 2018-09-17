#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

#define STANDART_LENGTH 128
int fd[2][2];

void son(char *cmd, int in, int out)
{
    if (dup2(in, 0) == -1)
    {
        perror("dup2");
        exit(-1);
    }
    if (dup2(out, 1) == -1)
    {
        perror("dup2");
        exit(-1);
    }
    if (fd[0][0] != -1 && fd[0][0] != 0)
        close(fd[0][0]);
    if (fd[0][1] != -1 && fd[0][1] != 1)
        close(fd[0][1]);
    if (fd[1][0] != -1 && fd[1][0] != 0)
        close(fd[1][0]);
    if (fd[1][1] != -1 && fd[1][1] != 1)
        close(fd[1][1]);
    char** buff = (char**)malloc(STANDART_LENGTH * sizeof(char*));
    if(buff == NULL)
        perror("error to allocate memory");
    int buff_size = 0;
    int position = 0;
    buff[buff_size] = (char*)malloc(STANDART_LENGTH * sizeof(char));
    if(buff[buff_size] == NULL)
        perror("error to allocate memory");
    while(*cmd != '\0') {
        if(*cmd == ' ') {
            buff[buff_size][position] = '\0';
            position = 0;
            buff_size++;
            buff[buff_size] = (char*)malloc(STANDART_LENGTH * sizeof(char));
            if(buff[buff_size] == NULL)
                perror("error to allocate memory");
            cmd++;
            continue;
        }
        buff[buff_size][position] = *cmd;
        position++;
        cmd++;
    }
    buff[buff_size][position] = '\0';
    buff[buff_size + 1] = (char*)malloc(STANDART_LENGTH * sizeof(char));
    if(buff[buff_size] == NULL)
        perror("error to allocate memory");
    buff[buff_size + 1] = (char *) NULL;
    execvp(buff[0], buff);
    perror(cmd);
    exit(-1);
}

int main(int argc, char **argv)
{
    int i;
    int *status;
    if (argc < 3)
    {
        fprintf(stderr, "Few arguments\n");
        exit(-1);
    }
    fd[0][0] = 0;
    fd[0][1] = -1;
    for (i = 1; i < argc; i++)
    {
        if (i + 1 == argc)
        {
            fd[1][0] = -1;
            fd[1][1] = 1;
        }
        else
            if (pipe(fd[1]) == -1)
            {
                perror("pipe");
                exit(-1);
            }
        int q = fork();
        if (q == 0)
            son(argv[i], fd[0][0], fd[1][1]);
        else
        {
            if (fd[0][0] != -1 && fd[0][0] != 0)
                close(fd[0][0]);
            if (fd[0][1] != -1 && fd[0][1] != 1)
                close(fd[0][1]);
            fd[0][0] = fd[1][0];
            fd[0][1] = fd[1][1];
        }
    }
    for (i = 1; i < argc; i++)
        wait(status);
    if (fd[0][0] != -1 && fd[0][0] != 0)
        close(fd[0][0]);
    if (fd[0][1] != -1 && fd[0][1] != 1)
        close(fd[0][1]);
    if (fd[1][0] != -1 && fd[1][0] != 0)
        close(fd[1][0]);
    if (fd[1][1] != -1 && fd[1][1] != 1)
        close(fd[1][1]);
    return 0;
}

