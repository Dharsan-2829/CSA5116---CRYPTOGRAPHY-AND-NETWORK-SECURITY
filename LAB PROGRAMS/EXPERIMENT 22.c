
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* ---- Simplified DES (S-DES): 8-bit block, 10-bit key ---- */
static const int P10[10] = {3,5,2,7,4,10,1,9,8,6};
static const int P8[8]   = {6,3,7,4,8,5,10,9};
static const int IPt[8]  = {2,6,3,1,4,8,5,7};
static const int IPi[8]  = {4,1,3,5,7,2,8,6};
static const int EP[8]   = {4,1,2,3,2,3,4,1};
static const int P4[4]   = {2,4,3,1};
static const int S0[4][4] = {{1,0,3,2},{3,2,1,0},{0,2,1,3},{3,1,3,2}};
static const int S1[4][4] = {{0,1,2,3},{2,0,1,3},{3,0,1,0},{2,1,0,3}};

static int perm(int in, int inbits, const int *t, int n)
{
    int out = 0;
    for (int i = 0; i < n; i++) out = (out << 1) | ((in >> (inbits - t[i])) & 1);
    return out;
}
static int rotl5(int x, int n) { return ((x << n) | (x >> (5 - n))) & 31; }

static void keygen(int key, int *k1, int *k2)
{
    int p = perm(key, 10, P10, 10), l = p >> 5, r = p & 31;
    l = rotl5(l, 1); r = rotl5(r, 1);
    *k1 = perm((l << 5) | r, 10, P8, 8);
    l = rotl5(l, 2); r = rotl5(r, 2);
    *k2 = perm((l << 5) | r, 10, P8, 8);
}
static int sbox(int x, const int S[4][4])
{
    int row = (((x >> 3) & 1) << 1) | (x & 1), col = (((x >> 2) & 1) << 1) | ((x >> 1) & 1);
    return S[row][col];
}
static int fk(int x, int k)
{
    int l = x >> 4, r = x & 15;
    int e = perm(r, 4, EP, 8) ^ k;
    int p = perm((sbox(e >> 4, S0) << 2) | sbox(e & 15, S1), 4, P4, 4);
    return ((l ^ p) << 4) | r;
}
static int swp(int x) { return ((x & 15) << 4) | (x >> 4); }

static int sdes_enc(int p, int key)
{
    int k1, k2;
    keygen(key, &k1, &k2);
    int x = perm(p, 8, IPt, 8);
    x = fk(x, k1); x = swp(x); x = fk(x, k2);
    return perm(x, 8, IPi, 8);
}
static int sdes_dec(int c, int key)
{
    int k1, k2;
    keygen(key, &k1, &k2);
    int x = perm(c, 8, IPt, 8);
    x = fk(x, k2); x = swp(x); x = fk(x, k1);
    return perm(x, 8, IPi, 8);
}

static int readbits()
{
    char s[100];
    scanf("%99s", s);
    int v = 0;
    for (int i = 0; s[i]; i++) v = (v << 1) | (s[i] == 49);
    return v;
}
static void printbits(int v, int n) { for (int i = n - 1; i >= 0; i--) putchar(((v >> i) & 1) + 48); putchar(32); }


/* Cipher Block Chaining (CBC) mode with S-DES:  C_i = E(K, P_i XOR C_(i-1)),  C_0 = IV
   Test: IV = 10101010, key = 0111111101, plaintext = 00000001 00100011  ->  ciphertext 11110100 00001011 */
int main()
{
    int key, iv, n, i, p[50], c[50], prev;

    printf("Enter 10-bit key (e.g. 0111111101): ");
    key = readbits();
    printf("Enter 8-bit IV (e.g. 10101010): ");
    iv = readbits();
    printf("Enter number of 8-bit plaintext blocks: ");
    scanf("%d", &n);
    for (i = 0; i < n; i++)
    {
        printf("Enter plaintext block %d (8 bits): ", i + 1);
        p[i] = readbits();
    }

    prev = iv;
    printf("\nCiphertext: ");
    for (i = 0; i < n; i++)
    {
        c[i] = sdes_enc(p[i] ^ prev, key);
        prev = c[i];
        printbits(c[i], 8);
    }

    prev = iv;
    printf("\nDecrypted : ");
    for (i = 0; i < n; i++)
    {
        printbits(sdes_dec(c[i], key) ^ prev, 8);
        prev = c[i];
    }
    printf("\n");
    return 0;
}
