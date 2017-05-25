#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main(void)
{
    string s = get_string();
    int flag = 0;
    for (int i = 0, n = strlen(s); i < n; i++)
    {
        if (i == 0 && s[i] != ' ')
        {
            printf("%c", toupper(s[i]));
        }
        else if (s[i] == ' ')
        {
            flag = 1;
        }
        else
        {
            if (flag == 1)
            {
                printf("%c", toupper(s[i]));
            }
            
            flag = 0;
        }
    }
    printf("\n");
}
