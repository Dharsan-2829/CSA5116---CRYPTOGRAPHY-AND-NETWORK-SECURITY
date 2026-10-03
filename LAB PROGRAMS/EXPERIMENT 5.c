#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* Affine Caesar cipher: C = (a*p + b) mod 26 */
int gcd(int a, int b) { return b ? gcd(b, a % b) : a; }

int modinv(int a)
{
    for (int x = 1; x < 26; x++)
        if ((a * x) % 26 == 1) return x;
    return -1;
}

int main()
{
    int a, b, i;
    char text[500];

    printf("Values of a that are NOT allowed (gcd(a,26) != 1): ");
    for (a = 0; a < 26; a++)
        if (gcd(a, 26) != 1) printf("%d ", a);
    printf("\nValues of a that are allowed (gcd(a,26) = 1)     : ");
    for (a = 0; a < 26; a++)
        if (gcd(a, 26) == 1) printf("%d ", a);

    printf("\n\na) Limitations on b: none. b can be any value 0..25 because adding b only shifts the\n");
    printf("   alphabet and never destroys the one-to-one property (b = 0 is just a multiplicative cipher).\n");
    printf("b) a must be coprime with 26, so a cannot be even or 13 (0,2,4,...,24 and 13).\n");
    printf("   Example a=2, b=3: E(0) = %d and E(13) = %d  -> not one-to-one.\n\n", (2 * 0 + 3) % 26, (2 * 13 + 3) % 26);

    printf("Enter a and b: ");
    scanf("%d %d", &a, &b);
    if (gcd(a, 26) != 1)
    {
        printf("a = %d is not allowed (not coprime with 26).\n", a);
        return 0;
    }
    getchar();
    printf("Enter plaintext: ");
    fgets(text, sizeof text, stdin);
    text[strcspn(text, "\n")] = 0;

    int ai = modinv(a);
    printf("Ciphertext : ");
    char enc[500];
    for (i = 0; text[i]; i++)
    {
        if (isalpha((unsigned char)text[i]))
        {
            int p = toupper((unsigned char)text[i]) - 'A';
            enc[i] = 'A' + (a * p + b) % 26;
        }
        else enc[i] = text[i];
    }
    enc[i] = 0;
    printf("%s\n", enc);

    printf("Decrypted  : ");
    for (i = 0; enc[i]; i++)
    {
        if (isalpha((unsigned char)enc[i]))
        {
            int c = enc[i] - 'A';
            putchar('A' + (ai * (c - b + 26)) % 26);
        }
        else putchar(enc[i]);
    }
    printf("\n");
    return 0;
}
