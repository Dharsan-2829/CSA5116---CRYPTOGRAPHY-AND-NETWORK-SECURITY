#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef unsigned char u8;

static const u8 IP[64] = {58,50,42,34,26,18,10,2,60,52,44,36,28,20,12,4,62,54,46,38,30,22,14,6,64,56,48,40,32,24,16,8,
                          57,49,41,33,25,17,9,1,59,51,43,35,27,19,11,3,61,53,45,37,29,21,13,5,63,55,47,39,31,23,15,7};
static const u8 FP[64] = {40,8,48,16,56,24,64,32,39,7,47,15,55,23,63,31,38,6,46,14,54,22,62,30,37,5,45,13,53,21,61,29,
                          36,4,44,12,52,20,60,28,35,3,43,11,51,19,59,27,34,2,42,10,50,18,58,26,33,1,41,9,49,17,57,25};
static const u8 EXP[48] = {32,1,2,3,4,5,4,5,6,7,8,9,8,9,10,11,12,13,12,13,14,15,16,17,
                           16,17,18,19,20,21,20,21,22,23,24,25,24,25,26,27,28,29,28,29,30,31,32,1};
static const u8 PBOX[32] = {16,7,20,21,29,12,28,17,1,15,23,26,5,18,31,10,2,8,24,14,32,27,3,9,19,13,30,6,22,11,4,25};
static const u8 PC1[56] = {57,49,41,33,25,17,9,1,58,50,42,34,26,18,10,2,59,51,43,35,27,19,11,3,60,52,44,36,
                           63,55,47,39,31,23,15,7,62,54,46,38,30,22,14,6,61,53,45,37,29,21,13,5,28,20,12,4};
static const u8 PC2[48] = {14,17,11,24,1,5,3,28,15,6,21,10,23,19,12,4,26,8,16,7,27,20,13,2,
                           41,52,31,37,47,55,30,40,51,45,33,48,44,49,39,56,34,53,46,42,50,36,29,32};
static const int SHIFTS[16] = {1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};
static const u8 SB[8][64] = {
{14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7, 0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8, 4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0, 15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13},
{15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10, 3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5, 0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15, 13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9},
{10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8, 13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1, 13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7, 1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12},
{7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15, 13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9, 10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4, 3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14},
{2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9, 14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6, 4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14, 11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3},
{12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11, 10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8, 9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6, 4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13},
{4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1, 13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6, 1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2, 6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12},
{13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7, 1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2, 7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8, 2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}};

/* ---- helper functions ---- */
static void permute(const u8 *t, int n, const u8 *in, u8 *out)
{
    u8 tmp[64];
    for (int i = 0; i < n; i++) tmp[i] = in[t[i] - 1];
    memcpy(out, tmp, n);
}

static void bytes2bits(const u8 *b, int n, u8 *bits)
{
    for (int i = 0; i < n; i++)
        for (int j = 0; j < 8; j++) bits[i * 8 + j] = (b[i] >> (7 - j)) & 1;
}

static void bits2bytes(const u8 *bits, int n, u8 *b)
{
    for (int i = 0; i < n; i++)
    {
        b[i] = 0;
        for (int j = 0; j < 8; j++) b[i] = (b[i] << 1) | bits[i * 8 + j];
    }
}

static void hex2bytes(const char *h, u8 *b, int n)
{
    for (int i = 0; i < n; i++)
    {
        unsigned v = 0;
        sscanf(h + 2 * i, "%2x", &v);
        b[i] = (u8)v;
    }
}

static void printhex(const u8 *b, int n)
{
    for (int i = 0; i < n; i++) printf("%02X", b[i]);
}

static void rotl28(u8 *h, int n)
{
    u8 t[28];
    for (int i = 0; i < 28; i++) t[i] = h[(i + n) % 28];
    memcpy(h, t, 28);
}

/* ---- key schedule: 16 subkeys of 48 bits (as bit arrays) ---- */
static void des_setkey(const u8 key[8], u8 sk[16][48])
{
    u8 kb[64], cd[56];
    bytes2bits(key, 8, kb);
    permute(PC1, 56, kb, cd);
    for (int r = 0; r < 16; r++)
    {
        rotl28(cd, SHIFTS[r]);
        rotl28(cd + 28, SHIFTS[r]);
        permute(PC2, 48, cd, sk[r]);
    }
}

/* ---- round function f(R, K) ---- */
static void feistel(const u8 *R, const u8 *k, u8 *out)
{
    u8 e[48], s[32];
    permute(EXP, 48, R, e);
    for (int i = 0; i < 48; i++) e[i] ^= k[i];
    for (int b = 0; b < 8; b++)
    {
        int row = (e[b * 6] << 1) | e[b * 6 + 5];
        int col = (e[b * 6 + 1] << 3) | (e[b * 6 + 2] << 2) | (e[b * 6 + 3] << 1) | e[b * 6 + 4];
        int v = SB[b][row * 16 + col];
        for (int j = 0; j < 4; j++) s[b * 4 + j] = (v >> (3 - j)) & 1;
    }
    permute(PBOX, 32, s, out);
}

/* ---- one 64-bit block; dec=1 uses the subkeys in reverse order ---- */
static void des_crypt(const u8 in[8], u8 out[8], u8 sk[16][48], int dec)
{
    u8 b[64], L[32], R[32], f[32], t[32];
    bytes2bits(in, 8, b);
    permute(IP, 64, b, b);
    memcpy(L, b, 32);
    memcpy(R, b + 32, 32);
    for (int r = 0; r < 16; r++)
    {
        feistel(R, sk[dec ? 15 - r : r], f);
        for (int i = 0; i < 32; i++) t[i] = L[i] ^ f[i];
        memcpy(L, R, 32);
        memcpy(R, t, 32);
    }
    memcpy(b, R, 32);
    memcpy(b + 32, L, 32);
    permute(FP, 64, b, b);
    bits2bytes(b, 8, out);
}

/* Error propagation: ECB versus CBC (using DES).
   a) CBC decryption: P_i = D(C_i) XOR C_(i-1). A damaged C1 corrupts P1 (completely) and P2 (only the same bit).
      P3 = D(C3) XOR C2 uses only undamaged blocks -> NO block beyond P2 is affected.
   b) A bit error in the source P1 changes C1, and because C1 is chained into C2, C2 into C3 ...
      the error propagates through ALL following ciphertext blocks. The receiver, however, decrypts correctly:
      P1 comes out with the same single-bit error and every later block P2..Pn is recovered correctly. */
#define NB 4
int main()
{
    u8 key[8], iv[8], sk[16][48];
    u8 p[NB * 8], c[NB * 8], d[NB * 8], e[NB * 8], q[NB * 8], c2[NB * 8], tmp[8], prev[8];
    char kh[20] = "133457799BBCDFF1", ivh[20] = "0011223344556677";
    const char *msg = "BLOCK-01BLOCK-02BLOCK-03BLOCK-04";
    int i, j;

    hex2bytes(kh, key, 8);
    hex2bytes(ivh, iv, 8);
    des_setkey(key, sk);
    memcpy(p, msg, NB * 8);

    /* ---- CBC encrypt ---- */
    memcpy(prev, iv, 8);
    for (i = 0; i < NB; i++)
    {
        for (j = 0; j < 8; j++) tmp[j] = p[i * 8 + j] ^ prev[j];
        des_crypt(tmp, c + i * 8, sk, 0);
        memcpy(prev, c + i * 8, 8);
    }
    /* ---- ECB encrypt ---- */
    for (i = 0; i < NB; i++) des_crypt(p + i * 8, e + i * 8, sk, 0);

    printf("PART (a): one bit error in transmitted ciphertext block C1\n");
    c[0] ^= 0x01;
    e[0] ^= 0x01;

    /* CBC decrypt with the error */
    memcpy(prev, iv, 8);
    for (i = 0; i < NB; i++)
    {
        des_crypt(c + i * 8, tmp, sk, 1);
        for (j = 0; j < 8; j++) d[i * 8 + j] = tmp[j] ^ prev[j];
        memcpy(prev, c + i * 8, 8);
    }
    /* ECB decrypt with the error */
    for (i = 0; i < NB; i++) des_crypt(e + i * 8, q + i * 8, sk, 1);

    for (i = 0; i < NB; i++)
    {
        printf("  Block P%d   original: %.8s   ECB: %s   CBC: %s\n", i + 1, msg + i * 8,
               memcmp(q + i * 8, p + i * 8, 8) ? "CORRUPTED" : "correct  ",
               memcmp(d + i * 8, p + i * 8, 8) ? "CORRUPTED" : "correct");
    }
    printf("  -> ECB: only P1 affected.  CBC: P1 garbled and P2 has a 1-bit error (see below), P3 and P4 are fine.\n");
    printf("  P2 differs from original in bits: ");
    for (j = 0; j < 8; j++) printf("%02X ", d[8 + j] ^ p[8 + j]);

    printf("\n\nPART (b): one bit error in the SOURCE plaintext block P1\n");
    p[0] ^= 0x01;
    memcpy(prev, iv, 8);
    for (i = 0; i < NB; i++)
    {
        for (j = 0; j < 8; j++) tmp[j] = p[i * 8 + j] ^ prev[j];
        des_crypt(tmp, c2 + i * 8, sk, 0);
        memcpy(prev, c2 + i * 8, 8);
    }
    /* compare with original CBC ciphertext (undo the earlier damage of C1) */
    c[0] ^= 0x01;
    for (i = 0; i < NB; i++)
        printf("  Ciphertext block C%d %s\n", i + 1, memcmp(c + i * 8, c2 + i * 8, 8) ? "is DIFFERENT (error propagated)" : "unchanged");
    memcpy(prev, iv, 8);
    for (i = 0; i < NB; i++)
    {
        des_crypt(c2 + i * 8, tmp, sk, 1);
        for (j = 0; j < 8; j++) d[i * 8 + j] = tmp[j] ^ prev[j];
        memcpy(prev, c2 + i * 8, 8);
    }
    printf("  Receiver output: P1 %s the (erroneous) source P1, P2..P4 %s\n",
           memcmp(d, p, 8) == 0 ? "equals" : "differs from",
           memcmp(d + 8, p + 8, 24) == 0 ? "are recovered correctly" : "are wrong");
    return 0;
}
