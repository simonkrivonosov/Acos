#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "wchar.h"
#include <locale.h>

struct wc
{
    int str_num;
    int words_num;
    int chars_num;
};

int main(int argc, char *argv[])
{
    char *locale = setlocale(LC_ALL, "");
    wint_t prev;
    wint_t c;
    struct wc* arr = (struct wc *)malloc(argc * sizeof(struct wc));
    if (argc == 1)
    {
        prev = ' ';
        arr[0].chars_num = 0;
        arr[0].str_num = 0;
        arr[0].words_num = 0;
        while ((c = getwchar()) != WEOF)
        {
            arr[0].chars_num++;
            if (c == L'\n')
                arr[0].str_num++;
            if ((iswspace(c) || c == (wint_t)255 || c == L'\n') && !iswspace(prev) && prev != (wint_t)255 && prev != L'\n')
                arr[0].words_num++;
            prev = c;
        }
        if (!(arr[0].chars_num == 0 && arr[0].str_num == 0 && arr[0].words_num == 0) && prev != L'\n' && !iswspace(prev) && prev != (wint_t)255)
        arr[0].words_num++;
        printf("%d %d %d ", arr[0].str_num, arr[0].words_num, arr[0].chars_num);
        putchar('\n');
    }
    else
    {
        FILE* in;
        for (int i = 1; i < argc; i++)
        {
            in = fopen(argv[i], "r");
            if (in == NULL)
            {
                perror("wc: there is no such file or folder ");
                perror(argv[i]);
                arr[i].chars_num = -1;
            }
            else
            {
                prev = ' ';
                arr[i].chars_num = 0;
                arr[i].str_num = 0;
                arr[i].words_num = 0;
                while ((c = fgetwc(in)) != WEOF)
                {
                    arr[i].chars_num++;
                    if (c == L'\n')
                        arr[i].str_num++;
                    if ((iswspace(c) || c == (wint_t)255 || c == L'\n') && !iswspace(prev) && prev != (wint_t)255 && prev != L'\n')
                        arr[i].words_num++;
                    prev = c;
                }
                if (!(arr[i].chars_num == 0 && arr[i].str_num == 0 && arr[i].words_num == 0) && prev != L'\n' && !iswspace(prev) && prev != (wint_t)255)
                    arr[i].words_num++;
                fclose(in);
            }
        }

        for (int i = 1; i < argc; i++)
        {
            if (arr[i].chars_num != -1)
            {
                printf("%10d %10d %10d ", arr[i].str_num, arr[i].words_num, arr[i].chars_num);
                puts(argv[i]);
            }
        }
        if (argc > 2)
        {
            struct wc total;
            total.chars_num = 0;
            total.str_num = 0;
            total.words_num = 0;
            for (int i = 1; i < argc; i++)
            {
                if (arr[i].chars_num != -1)
                {
                    total.chars_num += arr[i].chars_num;
                    total.str_num += arr[i].str_num;
                    total.words_num += arr[i].words_num;
                }
            }
            printf("%10d %10d %10d ", total.str_num, total.words_num, total.chars_num);
            puts("total");
        }
    }
    free(arr);
    return 0;
}

