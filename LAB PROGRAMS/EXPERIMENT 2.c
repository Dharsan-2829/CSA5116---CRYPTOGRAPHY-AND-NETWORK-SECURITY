#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* Monoalphabetic substitution cipher: plaintext alphabet -> permuted cipher alphabet */
int main()
{
    char key[100], text[500], inv[26], used[26] = {0};
    int i;

    printf("Enter cipher alphabet (26 unique letters, e.g. QWERTYUIOPASDFGHJKLZXCVBNM): ");
    scanf("%s", key);
    if (strlen(key) != 26)
    {
        printf("Key must have exactly 26 letters.\n");
        return 0;
    }
    for (i = 0; i < 26; i++)
    {
        key[i] = toupper((unsigned char)key[i]);
        if (!isalpha((unsigned char)key[i]) || used[key[i] - 'A'])
        {
            printf("Key must be a permutation of the 26 letters.\n");
            return 0;
        }
        used[key[i] - 'A'] = 1;
        inv[key[i] - 'A'] = 'A' + i;
    }

    getchar();
    printf("Enter plaintext: ");
    fgets(text, sizeof text, stdin);
    text[strcspn(text, "\n")] = 0;

    printf("Ciphertext : ");
    for (i = 0; text[i]; i++)
    {
        if (isalpha((unsigned char)text[i]))
            putchar(key[toupper((unsigned char)text[i]) - 'A']);
        else
            putchar(text[i]);
    }

    printf("\nDecrypted  : ");
    for (i = 0; text[i]; i++)
    {
        if (isalpha((unsigned char)text[i]))
        {
            char c = key[toupper((unsigned char)text[i]) - 'A'];
            putchar(inv[c - 'A']);
        }
        else
            putchar(text[i]);
    }
    printf("\n");
    return 0;
}
