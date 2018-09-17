#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>


int cmp_func(const void* a, const void* b)
{
    char *str_a = *(char **)a;
    char *str_b = *(char **)b;
    return strcmp(str_a, str_b);
}
int main()
{

    int c;
    int buff_size = 0;
    char** buff = (char**)malloc(buff_size*sizeof(char*));
    int str_size = 1;
    bool twice = false;
    char prev = '\0';
    int err = 0;
    while ((c = getchar()) != EOF)
    {
        if ((c != '&' && prev == '&') || (c != '|' && prev == '|'))
        {
            str_size += 2;
            buff[buff_size - 1] = (char*)realloc(buff[buff_size - 1], str_size*sizeof(char));
            if(buff[buff_size - 1] != NULL)
            {
                buff[buff_size - 1][str_size - 2] = '"';
                buff[buff_size - 1][str_size - 1] = '\0';
            }
            else
            {
                puts("Error to allocate memmory");
                return -1;
            }
        }

        if (buff_size > 0 && (isspace(c) || c == 255))
        {
            str_size += 2;
            buff[buff_size - 1] = (char*)realloc(buff[buff_size - 1], str_size*sizeof(char));
            if(buff[buff_size - 1] != NULL)
            {
                buff[buff_size - 1][str_size - 2] = '"';
                buff[buff_size - 1][str_size - 1] = '\0';
            }
            else
            {
                puts("Error to allocate memmory");
                return -1;
            }
            while ((isspace(c) || c == 255) && c != EOF)
            {
                prev = c;
                c = getchar();
            }
        }

        if (((c == '&' && prev != '&') || (c == '|' && prev != '|')) && (buff_size != 0 && buff[buff_size - 1][str_size - 1] != '\0'))
        {
            str_size += 2;
            buff[buff_size - 1] = (char*)realloc(buff[buff_size - 1], str_size*sizeof(char));
            if(buff[buff_size - 1] != NULL)
            {
                buff[buff_size - 1][str_size - 2] = '"';
                buff[buff_size - 1][str_size - 1] = '\0';
            }
            else
            {
                puts("Error to allocate memmory");
                return -1;
            }
        }
        if ((c == '&' && prev != '&') || (c == '|' && prev != '|'))
        {
            str_size = 2;
            buff_size++;
            buff = (char**)realloc(buff, buff_size*sizeof(char*));
            if(buff != NULL)
            {
                buff[buff_size - 1] = (char*)malloc(str_size*sizeof(char));
                buff[buff_size - 1][str_size - 2] = '"';
                buff[buff_size - 1][str_size - 1] = c;
            }
            else
            {
                puts("Error to allocate memmory");
                return -1;
            }
        }
        if ((c == '&' && prev == '&' && buff[buff_size - 1][str_size - 1] != '\0') || (c == '|' && prev == '|' && buff[buff_size - 1][str_size - 1] != '\0'))
        {
            str_size += 3;
            buff[buff_size - 1] = (char*)realloc(buff[buff_size - 1], str_size*sizeof(char));
            if(buff[buff_size - 1] != NULL)
            {
                buff[buff_size - 1][str_size - 3] = c;
                buff[buff_size - 1][str_size - 2] = '"';
                buff[buff_size - 1][str_size - 1] = '\0';
                twice = true;
            }
            else
            {
                puts("Error to allocate memmory");
                return -1;
            }
        }
        if (((c == '&' && prev == '&') || (c == '|' && prev == '|')) && buff[buff_size - 1][str_size - 1] == '\0' && twice == false)
        {
            str_size = 2;
            buff_size++;
            buff = (char**)realloc(buff, buff_size*sizeof(char*));
            if(buff != NULL)
            {
                buff[buff_size - 1] = (char*)malloc(str_size*sizeof(char));
                buff[buff_size - 1][str_size - 2] = '"';
                buff[buff_size - 1][str_size - 1] = c;
            }
            else
            {
                puts("Error to allocate memmory");
                return -1;
            }
        }
        if (c == '\'')
        {
            if (buff_size == 0 || buff[buff_size - 1][str_size - 1] == '\0')
            {
                str_size = 1;
                buff_size++;
                buff = (char**)realloc(buff, buff_size*sizeof(char*));
                if(buff != NULL)
                {
                    buff[buff_size - 1] = (char*)malloc(str_size*sizeof(char));
                    buff[buff_size - 1][str_size - 1] = '"';
                }
                else
                {
                    puts("Error to allocate memmory");
                    return -1;
                }
            }
            c = getchar();
            while (c != '\'' && c != EOF)
            {
                str_size++;
                buff[buff_size - 1] = (char*)realloc(buff[buff_size - 1], str_size*sizeof(char));
                if(buff[buff_size - 1] != NULL)
                {
                    buff[buff_size - 1][str_size - 1] = c;
                    c = getchar();
                }
                else
                {
                    puts("Error to allocate memmory");
                    return -1;
                }
            }
            if (c == EOF)
                err = -1;
        }

        if (c == '"')
        {

            if (buff_size == 0 || buff[buff_size - 1][str_size - 1] == '\0')
            {
                str_size = 1;
                buff_size++;
                buff = (char**)realloc(buff, buff_size*sizeof(char*));
                if(buff != NULL)
                {
                    buff[buff_size - 1] = (char*)malloc(str_size*sizeof(char));
                    buff[buff_size - 1][str_size - 1] = '"';
                }
                else
                {
                    puts("Error to allocate memmory");
                    return -1;
                }
            }
            c = getchar();
            while (c != '"' && c != EOF)
            {
                str_size++;
                buff[buff_size - 1] = (char*)realloc(buff[buff_size - 1], str_size*sizeof(char));
                if(buff[buff_size - 1] != NULL)
                {
                    buff[buff_size - 1][str_size - 1] = c;
                    c = getchar();
                }
                else
                {
                    puts("Error to allocate memmory");
                    return -1;
                }
            }
            if (c == EOF)
                err = -1;
        }

        if (c == EOF)
            break;
        if ((buff_size == 0 || buff[buff_size - 1][str_size - 1] == '\0') && c != '&' && c != '|' && c != '"' && c != '\'' && c != ';')
        {
            str_size = 1;
            buff_size++;
            buff = (char**)realloc(buff, buff_size*sizeof(char*));
            if(buff != NULL)
            {
                buff[buff_size - 1] = (char*)malloc(str_size*sizeof(char));
                buff[buff_size - 1][str_size - 1] = '"';
            }
            else
            {
                puts("Error to allocate memmory");
                return -1;
            }
        }
        if (c == ';')
        {
            if (buff_size > 0 && buff[buff_size - 1][str_size - 1] != '\0')
            {
                str_size += 2;
                buff[buff_size - 1] = (char*)realloc(buff[buff_size - 1], str_size*sizeof(char));
                if(buff[buff_size - 1] != NULL)
                {
                    buff[buff_size - 1][str_size - 2] = '"';
                    buff[buff_size - 1][str_size - 1] = '\0';
                }
                else
                {
                    puts("Error to allocate memmory");
                    return -1;
                }
            }
            str_size = 4;
            buff_size++;
            buff = (char**)realloc(buff, buff_size*sizeof(char*));
            if(buff != NULL)
            {
                buff[buff_size - 1] = (char*)malloc(str_size*sizeof(char));
                buff[buff_size - 1][str_size - 4] = '"';
                buff[buff_size - 1][str_size - 3] = c;
                buff[buff_size - 1][str_size - 2] = '"';
                buff[buff_size - 1][str_size - 1] = '\0';
            }
            else
            {
                puts("Error to allocate memmory");
                return -1;
            }

        }
        if (c != '\0' && c != '&'  && c != '|' && c != '\'' && c != '"' && c != ';')
        {
            if (buff[buff_size - 1][str_size - 1] == '\0')
            {
                str_size = 1;
                buff_size++;
                buff = (char**)realloc(buff, buff_size*sizeof(char*));
                if(buff != NULL)
                {
                    buff[buff_size - 1] = (char*)malloc(str_size*sizeof(char));
                    buff[buff_size - 1][str_size - 1] = '"';
                }
                else
                {
                    puts("Error to allocate memmory");
                    return -1;
                }
            }
            str_size++;
            buff[buff_size - 1] = (char*)realloc(buff[buff_size - 1], str_size*sizeof(char));
            if(buff[buff_size - 1] != NULL)
            {
                buff[buff_size - 1][str_size - 1] = c;
            }
            else
            {
                puts("Error to allocate memmory");
                return -1;
            }
        }
        prev = c;
        twice = false;
    }
    if (buff_size >0 && buff[buff_size - 1][str_size - 1] != '\0')
    {
        str_size += 2;
        buff[buff_size - 1] = (char*)realloc(buff[buff_size - 1], str_size*sizeof(char));
        if(buff[buff_size - 1] != NULL)
        {
            buff[buff_size - 1][str_size - 2] = '"';
            buff[buff_size - 1][str_size - 1] = '\0';
        }
        else
        {
            puts("Error to allocate memmory");
            return -1;
        }
    }
    qsort(buff, buff_size, sizeof(char*), cmp_func);
    putchar('\n');
    if (err == -1)
        puts("ERROR");
    else
    {
        for (int i = 0; i < buff_size; i++)
        {
            puts(buff[i]);
        }
    }
    for (int i = 0; i < buff_size; i++)
        free(buff[i]);
    free(buff);
    return 0;
}

