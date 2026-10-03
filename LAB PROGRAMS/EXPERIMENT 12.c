#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* Hill cipher (2x2) with key K = | 9 4 |
                                  | 5 7 |      Cipher pair = (p1 p2) * K mod 26 */
int K[2][2] = {{9, 4}, {5, 7}}, Ki[2][2];

int md(int x) { x %= 26; return x < 0 ? x + 26 : x; }

int inv(int a)
{
    a = md(a);
    for (int x = 1; x < 26; x++)
        if (a * x % 26 == 1) return x;
    return -1;
}

int main()
{
    const char *msg = "meet me at the usual place at ten rather than eight oclock";
    char p[200], c[200], d[200];
    int n = 0, i;

    for (i = 0; msg[i]; i++)
        if (isalpha((unsigned char)msg[i])) p[n++] = tolower((unsigned char)msg[i]);
    if (n % 2) p[n++] = 'x';
    p[n] = 0;

    printf("Plaintext (cleaned): %s\n\nEncryption:\n", p);
    for (i = 0; i < n; i += 2)
    {
        int a = p[i] - 'a', b = p[i + 1] - 'a';
        int c1 = md(a * K[0][0] + b * K[1][0]), c2 = md(a * K[0][1] + b * K[1][1]);
        printf("(%c%c) = (%2d %2d) -> (%2d*9+%2d*5, %2d*4+%2d*7) = (%3d, %3d) mod 26 = (%2d, %2d) = %c%c\n",
               p[i], p[i + 1], a, b, a, b, a, b, a * 9 + b * 5, a * 4 + b * 7, c1, c2, 'a' + c1, 'a' + c2);
        c[i] = 'a' + c1;
        c[i + 1] = 'a' + c2;
    }
    c[n] = 0;
    printf("\nCiphertext: %s\n", c);

    int det = md(K[0][0] * K[1][1] - K[0][1] * K[1][0]);
    int di = inv(det);
    printf("\nDecryption:\ndet(K) = %d mod 26, det^-1 = %d\n", det, di);
    Ki[0][0] = md(di * K[1][1]);  Ki[0][1] = md(-di * K[0][1]);
    Ki[1][0] = md(-di * K[1][0]); Ki[1][1] = md(di * K[0][0]);
    printf("K^-1 = | %2d %2d |\n       | %2d %2d |\n", Ki[0][0], Ki[0][1], Ki[1][0], Ki[1][1]);

    for (i = 0; i < n; i += 2)
    {
        int a = c[i] - 'a', b = c[i + 1] - 'a';
        int p1 = md(a * Ki[0][0] + b * Ki[1][0]), p2 = md(a * Ki[0][1] + b * Ki[1][1]);
        printf("(%c%c) = (%2d %2d) -> (%2d*%d+%2d*%d, %2d*%d+%2d*%d) mod 26 = (%2d, %2d) = %c%c\n",
               c[i], c[i + 1], a, b, a, Ki[0][0], b, Ki[1][0], a, Ki[0][1], b, Ki[1][1], p1, p2, 'a' + p1, 'a' + p2);
        d[i] = 'a' + p1;
        d[i + 1] = 'a' + p2;
    }
    d[n] = 0;
    printf("\nRecovered plaintext: %s\n", d);
    return 0;
}
