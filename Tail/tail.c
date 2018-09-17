#include "stdio.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "unistd.h"
#include "string.h"
#include "stdlib.h"

#define STANDART_LENGTH 64

void _realloc_str(char** str, int* size) {
    *size *= 2;
    *str = (char*)realloc(*str, (*size + 1));
    if (*str == NULL) {
        perror("Unable to allocate new string");
    }
}

void _realloc_arr(char*** arr, int* size) {
    *size *= 2;
    *arr = (char**)realloc(*arr, *size * sizeof(char*));
    if (*arr == NULL) {
        perror("Unable to allocate array of strings");
    }
}

int main(int argc, char *argv[])
{
    int symbol;
    FILE* input_fd;
    int i = 1;
    int counter = 0;
    int str_num = 0;
    int str_capacity = 10;
    int length = STANDART_LENGTH;
    if (argc > 1)
    {
        for(; i < argc; i++)
        {
            char* tmp_str = (char*)calloc(length, 1);
            char** result_str = (char**)malloc(str_capacity * sizeof(char*));
            if(result_str == NULL)
            {
                perror("error to allocate memory for reslut_str");
            }
            if(tmp_str == NULL)
            {
                perror("error to allocate memory for tmp_str");
            }
            if ((input_fd = fopen(argv[i],"r")) == NULL)
                perror(argv[i]);
            else
                printf("%s:\n\n", argv[i]);
            while (1) {
                symbol = fgetc(input_fd);
                if(counter >= length) {
                    _realloc_str(&tmp_str, &length);
                }
                if(str_num >= str_capacity) {
                    _realloc_arr(&result_str, &str_capacity);
                }
                if(symbol == '\n')
                {
                    tmp_str[counter] = symbol;
                    tmp_str[++counter] = '\0';
                    result_str[str_num] = (char*)malloc(counter + 1);
                    if(result_str == NULL)
                    {
                        perror("error to allocate memory for reslut_str");
                    }
                    strcpy(result_str[str_num], tmp_str);
                    length = STANDART_LENGTH;
                    free(tmp_str);
                    tmp_str = (char*)calloc(length, 1);
                    if(tmp_str == NULL)
                    {
                        perror("error to allocate memory for tmp_str");
                    }
                    str_num++;
                    counter = 0;
                    continue;
                }
                else if (symbol == EOF) {
                    if(tmp_str != NULL) {
                        if(strcmp(tmp_str, "") != 0) {
                            result_str[str_num] = (char*)malloc(counter + 1);
                            if(result_str == NULL)
                            {
                                perror("error to allocate memory for result_str");
                            }
                            strcpy(result_str[str_num], tmp_str);
                            str_num++;
                        }
                        free(tmp_str);
                    }
                    break;
                }
                tmp_str[counter] = symbol;
                counter++;
            }
            printf("\n\n");

            int i = 0;
            if(str_num > 10) {
                i = str_num - 10;
            }

            for(; i < str_num; ++i)
                printf("%s", result_str[i]);
            printf("\n");
            fclose(input_fd);
            i = 0;
            while (i < str_num)
            {
                free(result_str[i++]);
            }
            free(result_str);
            counter = 0;
            str_num = 0;
            str_capacity = 10;
            length = STANDART_LENGTH;
        }
    }
    else
    {
        char* tmp_str = (char*)calloc(length, 1);
        char** result_str = (char**)malloc(str_capacity * sizeof(char*));
        if(result_str == NULL)
        {
            perror("error to allocate memory for reslut_str");
        }
        if(tmp_str == NULL)
        {
            perror("error to allocate memory for tmp_str");
        }
        while (1) {
            symbol = getchar();
            if(counter >= length) {
                _realloc_str(&tmp_str, &length);
            }
            if(str_num >= str_capacity) {
                _realloc_arr(&result_str, &str_capacity);
            }
            if(symbol == '\n')
            {
                tmp_str[counter] = symbol;
                tmp_str[++counter] = '\0';
                result_str[str_num] = (char*)malloc(counter + 1);
                if(result_str == NULL)
                {
                    perror("error to allocate memory for reslut_str");
                }
                strcpy(result_str[str_num], tmp_str);
                length = STANDART_LENGTH;
                free(tmp_str);
                tmp_str = (char*)calloc(length, 1);
                if(tmp_str == NULL)
                {
                    perror("error to allocate memory for tmp_str");
                }
                str_num++;
                counter = 0;
                continue;
            }
            else if (symbol == EOF) {
                if(tmp_str != NULL) {
                    if(strcmp(tmp_str, "") != 0) {
                        result_str[str_num] = (char*)malloc(counter + 1);
                        if(result_str == NULL)
                        {
                            perror("error to allocate memory for result_str");
                        }

                        strcpy(result_str[str_num], tmp_str);
                        str_num++;
                    }
                    free(tmp_str);
                }
                break;
            }
            tmp_str[counter] = symbol;
            counter++;
        }

        printf("\n\n");

        int i = 0;
        if(str_num > 10) {
            i = str_num - 10;
        }

        for(; i < str_num; ++i)
            printf("%s", result_str[i]);

        i = 0;
        while (i < str_num)
        {
            free(result_str[i++]);
        }
        free(result_str);
    }
    printf("\n");
    return 0;
}

