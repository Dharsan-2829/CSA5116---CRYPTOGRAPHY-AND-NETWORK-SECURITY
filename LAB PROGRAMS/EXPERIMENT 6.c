#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* Break an affine cipher using the two most frequent ciphertext letters.
   Assume they correspond to plaintext E (4) and T (19):
       a*4 + b = c1 (mod 26)
       a*19 + b = c2 (mod 26)
   For the problem: c1 = B (1), c2 = U (20)  ->  a = 3, b = 15                              */
int gcd(int a, int b) { return b ? gcd(b, a % b) : a; }

int modinv(int a)
{
    for (int x = 1; x < 26; x++)
        if ((a * x) % 26 == 1) return x;
    return -1;
}

int main()
{
    char c1, c2, text[500];
    int a, b, found = 0, A = 0, B = 0;

    printf("Enter most frequent ciphertext letter  : ");
    scanf(" %c", &c1);
    printf("Enter second most frequent letter      : ");
    scanf(" %c", &c2);
    int x1 = toupper((unsigned char)c1) - 'A', x2 = toupper((unsigned char)c2) - 'A';

    for (a = 1; a < 26; a++)
    {
        if (gcd(a, 26) != 1) continue;
        for (b = 0; b < 26; b++)
            if ((a * 4 + b) % 26 == x1 && (a * 19 + b) % 26 == x2)
            {
                printf("Key found: a = %d, b = %d  (E(4)=%c, E(19)=%c)\n", a, b, c1, c2);
                found = 1; A = a; B = b;
            }
    }
    if (!found)
    {
        printf("No valid key for the assumption E->%c, T->%c\n", c1, c2);
        return 0;
    }

    getchar();
    printf("Enter ciphertext to decrypt (optional, press Enter to skip): ");
    fgets(text, sizeof text, stdin);
    text[strcspn(text, "\n")] = 0;
    int ai = modinv(A);
    printf("Plaintext: ");
    for (int i = 0; text[i]; i++)
    {
        if (isalpha((unsigned char)text[i]))
        {
            int c = toupper((unsigned char)text[i]) - 'A';
            putchar('a' + (ai * (c - B + 26)) % 26);
        }
        else putchar(text[i]);
    }
    printf("\n");
    return 0;
}
