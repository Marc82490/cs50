#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#define LOWER 97
#define UPPER 65

int main(int argc, string argv[])
{
    // encrypt message only if a valid key is provided
    if (argc == 2)
    {
        int m = strlen(argv[1]);
        int key[m];
        // iterate over the characters in the key
        for (int i = 0; i < m; i++)
        {
            // display error message if an invalid key is provided
            if (!isalpha(argv[1][i]))
            {
                printf("Please enter one alphabetic key.\n");
                return 1;
            }
            // store key values as 0-indexed integers if key is valid
            else
            {
                key[i] = toupper(argv[1][i]) - UPPER;
            }
        }
        printf("plaintext: ");
        string s = get_string();
        printf("ciphertext: ");
        // start counting position in key
        int counter = 0;
        // iterate over characters in plaintext message
        for (int i = 0, n = strlen(s); i < n; i++)
        {
            if (isalpha(s[i]))
            {
                // get current key value from array
                int k = key[counter % m];
                // apply current key value to current plaintext letter
                if (islower(s[i]))
                {
                    int e = (s[i] - LOWER + k) % 26;
                    printf("%c", e + LOWER);
                }
                else
                {
                    int e = (s[i] - UPPER + k) % 26;
                    printf("%c", e + UPPER);
                }
                // increment key counter if current plaintext was alphabetic
                counter++;
            }
            // print non-alphabetic characters unchanged
            else
            {
                printf("%c", s[i]);
            }
        }
        printf("\n");
        return 0;
    }
    // display error message if no key or more than 1 key is provided
    else
    {
        printf("Please enter one alphabetic key.\n");
        return 1;
    }
}
