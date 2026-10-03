#include <stdio.h>
#include <string.h>
#include <ctype.h>

char M[5][5];

/* build 5x5 matrix from keyword (I and J share a cell) */
void build(const char *key)
{
    int used[26] = {0}, n = 0;
    char all[200];
    snprintf(all, sizeof all, "%sABCDEFGHIJKLMNOPQRSTUVWXYZ", key);
    for (int i = 0; all[i]; i++)
    {
        char c = toupper((unsigned char)all[i]);
        if (!isalpha((unsigned char)c)) continue;
        if (c == 'J') c = 'I';
        if (!used[c - 'A'])
        {
            used[c - 'A'] = 1;
            M[n / 5][n % 5] = c;
            n++;
        }
    }
}

void show()
{
    printf("Playfair matrix:\n");
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++) printf("%c ", M[i][j]);
        printf("\n");
    }
}

void find(char c, int *r, int *col)
{
    if (c == 'J') c = 'I';
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
            if (M[i][j] == c) { *r = i; *col = j; return; }
}

/* dir = +1 encrypt, -1 decrypt */
void pair(char a, char b, int dir, char *oa, char *ob)
{
    int r1, c1, r2, c2;
    find(a, &r1, &c1);
    find(b, &r2, &c2);
    if (r1 == r2)      { *oa = M[r1][(c1 + dir + 5) % 5]; *ob = M[r2][(c2 + dir + 5) % 5]; }
    else if (c1 == c2) { *oa = M[(r1 + dir + 5) % 5][c1]; *ob = M[(r2 + dir + 5) % 5][c2]; }
    else               { *oa = M[r1][c2]; *ob = M[r2][c1]; }
}

/* clean plaintext, split into digraphs, put X between double letters / at the end */
int prepare(const char *in, char *out)
{
    char t[600];
    int n = 0, len = 0;
    for (int i = 0; in[i]; i++)
        if (isalpha((unsigned char)in[i]))
        {
            char c = toupper((unsigned char)in[i]);
            t[n++] = (c == 'J') ? 'I' : c;
        }
    for (int i = 0; i < n;)
    {
        out[len++] = t[i];
        if (i + 1 < n && t[i + 1] != t[i]) { out[len++] = t[i + 1]; i += 2; }
        else { out[len++] = 'X'; i++; }
    }
    out[len] = 0;
    return len;
}

void run(const char *txt, int dir, char *out)
{
    int n = strlen(txt);
    for (int i = 0; i + 1 < n; i += 2) pair(txt[i], txt[i + 1], dir, &out[i], &out[i + 1]);
    out[n] = 0;
}

/* Playfair cipher using a keyword */
int main()
{
    char key[100], text[500], prep[700], enc[700], dec[700];

    printf("Enter keyword: ");
    scanf("%s", key);
    getchar();
    build(key);
    show();

    printf("Enter plaintext: ");
    fgets(text, sizeof text, stdin);

    prepare(text, prep);
    printf("Prepared digraphs: ");
    for (int i = 0; prep[i]; i += 2) printf("%c%c ", prep[i], prep[i + 1]);

    run(prep, +1, enc);
    printf("\nCiphertext : ");
    for (int i = 0; enc[i]; i += 2) printf("%c%c ", enc[i], enc[i + 1]);

    run(enc, -1, dec);
    printf("\nDecrypted  : %s\n", dec);
    return 0;
}
