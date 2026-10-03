#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* Keyword (monoalphabetic) cipher: keyword followed by the unused letters in normal order.
   Example: keyword CIPHER -> C I P H E R A B D F G J K L M N O Q S T U V W X Y Z           */
int main()
{
    char key[100], text[500], cipher[27], inv[26];
    int used[26] = {0}, n = 0, i;

    printf("Enter keyword: ");
    scanf("%s", key);
    for (i = 0; key[i]; i++)
    {
        char c = toupper((unsigned char)key[i]);
        if (isalpha((unsigned char)c) && !used[c - 'A']) { used[c - 'A'] = 1; cipher[n++] = c; }
    }
    for (i = 0; i < 26; i++)
        if (!used[i]) cipher[n++] = 'A' + i;
    cipher[26] = 0;

    printf("plain : a b c d e f g h i j k l m n o p q r s t u v w x y z\ncipher: ");
    for (i = 0; i < 26; i++) { printf("%c ", cipher[i]); inv[cipher[i] - 'A'] = 'a' + i; }

    getchar();
    printf("\nEnter plaintext: ");
    fgets(text, sizeof text, stdin);
    text[strcspn(text, "\n")] = 0;

    printf("Ciphertext : ");
    char enc[500];
    for (i = 0; text[i]; i++)
        enc[i] = isalpha((unsigned char)text[i]) ? cipher[toupper((unsigned char)text[i]) - 'A'] : text[i];
    enc[i] = 0;
    printf("%s\nDecrypted  : ", enc);
    for (i = 0; enc[i]; i++)
        putchar(isalpha((unsigned char)enc[i]) ? inv[enc[i] - 'A'] : enc[i]);
    printf("\n");
    return 0;
}
