#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#define LOWER 97
#define UPPER 65

int main(int argc, string argv[])
{
    // encrypt message only if a key is provided
    if (argc == 2)
    {
        // convert key from string to integer
        int k = atoi(argv[1]);
        printf("plaintext: ");
        string s = get_string();
        printf("ciphertext: ");
        // iterate over characters in plaintext message
        for (int i = 0, n = strlen(s); i < n; i++)
        {
            if (isalpha(s[i]))
            {
                if (islower(s[i]))
                {
                    // encrypt plaintext lowercase character by key value
                    int e = (s[i] - LOWER + k) % 26;
                    printf("%c", e + LOWER);
                }
                else
                {
                    // encrypt plaintext uppercase character by key value
                    int e = (s[i] - UPPER + k) % 26;
                    printf("%c", e + UPPER);
                }
            }
            else
            {
                // print nonalphabetical characters unchanged
                printf("%c", s[i]);
            }
        }
        printf("\n");
        return 0;
    }
    // display error message if no key, or more than 1 key, is provided
    else
    {
        printf("Please enter one key.\n");
        return 1;
    }
}
