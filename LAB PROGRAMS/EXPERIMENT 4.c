#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* Polyalphabetic substitution cipher (Vigenere): a different Caesar shift for each plaintext letter, taken from the key */
void vigenere(char *s, const char *key, int dir)
{
    int j = 0, kl = strlen(key);
    for (int i = 0; s[i]; i++)
    {
        if (isalpha((unsigned char)s[i]))
        {
            char base = isupper((unsigned char)s[i]) ? 'A' : 'a';
            int k = toupper((unsigned char)key[j % kl]) - 'A';
            s[i] = (s[i] - base + dir * k + 26) % 26 + base;
            j++;
        }
    }
}

int main()
{
    char text[500], key[100];

    printf("Enter plaintext: ");
    fgets(text, sizeof text, stdin);
    text[strcspn(text, "\n")] = 0;
    printf("Enter key (letters only): ");
    scanf("%s", key);

    vigenere(text, key, +1);
    printf("Ciphertext : %s\n", text);
    vigenere(text, key, -1);
    printf("Decrypted  : %s\n", text);
    return 0;
}
