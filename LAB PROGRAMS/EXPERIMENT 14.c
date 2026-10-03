#include <stdio.h>
#include <string.h>

/* One-time pad version of the Vigenere cipher: key stream of numbers (shifts) */
int main()
{
    const char *pt = "sendmoremoney";
    int key[] = {9, 0, 1, 7, 23, 15, 21, 14, 11, 11, 2, 8, 9};
    int n = strlen(pt), i;
    char ct[50];

    /* a) encryption */
    printf("a) Plaintext : %s\n   Key stream: ", pt);
    for (i = 0; i < n; i++) printf("%d ", key[i]);
    for (i = 0; i < n; i++) ct[i] = 'a' + (pt[i] - 'a' + key[i]) % 26;
    ct[n] = 0;
    printf("\n   Ciphertext: %s\n", ct);

    /* b) find key so that the ciphertext decrypts to another plaintext: k = (C - P) mod 26 */
    const char *want = "cashnotneeded";
    printf("\nb) Wanted plaintext: %s\n   Key stream needed: ", want);
    int k2[50];
    for (i = 0; i < n; i++)
    {
        k2[i] = ((ct[i] - 'a') - (want[i] - 'a') + 26) % 26;
        printf("%d ", k2[i]);
    }
    printf("\n   Check (decrypt with this key): ");
    for (i = 0; i < n; i++) putchar('a' + (ct[i] - 'a' - k2[i] + 26) % 26);
    printf("\n");
    return 0;
}
