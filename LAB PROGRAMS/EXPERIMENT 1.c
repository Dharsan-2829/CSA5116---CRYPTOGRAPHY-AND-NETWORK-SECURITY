#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* Caesar cipher: each letter is replaced by the letter k places further down the alphabet (k = 1..25) */
void caesar(char *s, int k)
{
    for (int i = 0; s[i]; i++)
    {
        if (isalpha((unsigned char)s[i]))
        {
            char base = isupper((unsigned char)s[i]) ? 'A' : 'a';
            s[i] = (s[i] - base + k + 26) % 26 + base;
        }
    }
}

int main()
{
    char text[500];
    int k;

    printf("Enter plaintext: ");
    fgets(text, sizeof text, stdin);
    text[strcspn(text, "\n")] = 0;

    printf("Enter key k (1-25): ");
    scanf("%d", &k);
    if (k < 1 || k > 25)
    {
        printf("Key must be in the range 1 to 25.\n");
        return 0;
    }

    caesar(text, k);
    printf("Ciphertext : %s\n", text);

    caesar(text, -k);
    printf("Decrypted  : %s\n", text);
    return 0;
}
