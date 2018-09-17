#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
int main(int argc, char *argv[])
{
    int BUFFER_SIZE = 64;
    int ret_in;
    int ret_out;
    int input_fd;
    int i = 1;
    if (argc > 1)
    {
        if(strcmp(argv[1],"-u") == 0)
        {
            BUFFER_SIZE = 1;
            ++i;
        }
        char buf[BUFFER_SIZE];
        for(; i < argc; i++)
        {
            if ((input_fd = open(argv[i], O_RDONLY)) == -1)
                perror(argv[i]);
            while ((ret_in = read(input_fd, buf, BUFFER_SIZE)) > 0){
                ret_out = write(1, buf, ret_in);
                if(ret_out != ret_in)
                    perror("error");
            }
            close(input_fd);
        }
    }
    else
    {
        char buf[BUFFER_SIZE];
        while ((ret_in = read(0, buf,BUFFER_SIZE )) > 0)
            write(1, buf, ret_in);
    }
    return 0;
}


