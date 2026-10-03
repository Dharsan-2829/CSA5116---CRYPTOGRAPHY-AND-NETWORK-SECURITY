#include <stdio.h>
#include <ctype.h>

/* Known-plaintext attack on a 2x2 Hill cipher.
   Two plaintext digraphs P (2x2) and their ciphertext digraphs C (2x2) satisfy  C = P * K (mod 26)
   so the key is  K = P^-1 * C (mod 26).   Test: plaintext "abbb", ciphertext "fhol" -> K = | 9 4 | 5 7 |   */
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
    char p[10], c[10];
    int P[2][2], C[2][2], K[2][2], i;

    printf("Enter 4 plaintext letters  (two digraphs, e.g. abbb): ");
    scanf("%4s", p);
    printf("Enter 4 ciphertext letters (e.g. fhol): ");
    scanf("%4s", c);
    for (i = 0; i < 4; i++)
    {
        P[i / 2][i % 2] = tolower((unsigned char)p[i]) - 'a';
        C[i / 2][i % 2] = tolower((unsigned char)c[i]) - 'a';
    }

    int det = md(P[0][0] * P[1][1] - P[0][1] * P[1][0]);
    int di = inv(det);
    if (di < 0)
    {
        printf("Plaintext matrix is not invertible mod 26 (det = %d). Choose different digraphs.\n", det);
        return 0;
    }
    int Pi[2][2] = {{md(di * P[1][1]), md(-di * P[0][1])}, {md(-di * P[1][0]), md(di * P[0][0])}};

    for (i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            K[i][j] = md(Pi[i][0] * C[0][j] + Pi[i][1] * C[1][j]);

    printf("Recovered key matrix K:\n| %2d %2d |\n| %2d %2d |\n", K[0][0], K[0][1], K[1][0], K[1][1]);

    /* verify */
    int ok = 1;
    for (i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            if (md(P[i][0] * K[0][j] + P[i][1] * K[1][j]) != C[i][j]) ok = 0;
    printf("Verification: %s\n", ok ? "P*K = C  (key is correct)" : "failed");
    return 0;
}
