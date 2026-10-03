#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>

/* Letter-frequency attack on any monoalphabetic substitution cipher (no human help).
   1. Initial key: ciphertext letters ranked by frequency are matched to English letters ranked by frequency.
   2. The key is improved by hill climbing (swapping two letter assignments) using a score made of
      English letter frequencies + common bigrams + common trigrams.
   3. Several random restarts give several candidates; they are printed in order of likelihood.
   Longer ciphertexts (200+ letters) give much better results.   Compile with:  gcc file.c -lm        */
#define MAXN 2000
#define RESTARTS 40
const char *ORDER = "ETAOINSHRDLCUMWFGYPBVKJXQZ";
double eng[26] = {8.167, 1.492, 2.782, 4.253, 12.702, 2.228, 2.015, 6.094, 6.966, 0.153, 0.772, 4.025, 2.406,
                  6.749, 7.507, 1.929, 0.095, 5.987, 6.327, 9.056, 2.758, 0.978, 2.360, 0.150, 1.974, 0.074};
const char *BG[] = {"TH","HE","IN","ER","AN","RE","ON","AT","EN","ND","TI","ES","OR","TE","OF","ED","IS","IT",
                    "AL","AR","ST","TO","NT","NG","SE","HA","AS","OU","IO","LE","VE","CO","ME","DE","HI","RI",
                    "RO","IC","NE","EA","RA","CE"};
const char *TG[] = {"THE", "AND", "ING", "ION", "ENT", "FOR", "THA"};

char ct[MAXN];      /* letters only, upper case, as indexes later */
int n;
double lf[26];

typedef struct { char key[27]; double score; } Cand;
Cand cand[RESTARTS];
int nc = 0;

double score_key(const char *key)
{
    char pt[MAXN + 1];
    double s = 0;
    int i, j;
    for (i = 0; i < n; i++) pt[i] = key[ct[i] - 'A'];
    for (i = 0; i < n; i++)
    {
        s += lf[pt[i] - 'A'];
        if (i + 1 < n)
            for (j = 0; j < 42; j++)
                if (pt[i] == BG[j][0] && pt[i + 1] == BG[j][1]) { s += 1.5; break; }
        if (i + 2 < n)
            for (j = 0; j < 7; j++)
                if (pt[i] == TG[j][0] && pt[i + 1] == TG[j][1] && pt[i + 2] == TG[j][2]) { s += 3.0; break; }
    }
    return s;
}

double climb(char *key)
{
    double best = score_key(key);
    int improved = 1, a, b;
    while (improved)
    {
        improved = 0;
        for (a = 0; a < 26; a++)
            for (b = a + 1; b < 26; b++)
            {
                char t = key[a]; key[a] = key[b]; key[b] = t;
                double s = score_key(key);
                if (s > best + 1e-9) { best = s; improved = 1; }
                else { t = key[a]; key[a] = key[b]; key[b] = t; }
            }
    }
    return best;
}

int cmp(const void *a, const void *b)
{
    double d = ((Cand *)b)->score - ((Cand *)a)->score;
    return (d > 0) - (d < 0);
}

int main()
{
    char text[MAXN];
    int i, r, top, cnt[26] = {0};

    for (i = 0; i < 26; i++) lf[i] = log(eng[i] / 100.0);

    printf("Enter ciphertext: ");
    fgets(text, sizeof text, stdin);
    text[strcspn(text, "\n")] = 0;
    for (i = 0; text[i]; i++)
        if (isalpha((unsigned char)text[i])) { ct[n++] = toupper((unsigned char)text[i]); cnt[ct[n - 1] - 'A']++; }
    printf("How many top plaintexts do you want? ");
    scanf("%d", &top);

    /* frequency-rank initial key */
    int rank[26], used[26] = {0};
    for (i = 0; i < 26; i++)
    {
        int best = -1;
        for (int j = 0; j < 26; j++)
            if (!used[j] && (best < 0 || cnt[j] > cnt[best])) best = j;
        used[best] = 1;
        rank[i] = best;
    }
    char base[27];
    for (i = 0; i < 26; i++) base[rank[i]] = ORDER[i];
    base[26] = 0;

    srand(1);
    for (r = 0; r < RESTARTS; r++)
    {
        char key[27];
        strcpy(key, base);
        if (r > 0)
            for (i = 0; i < 4 + r / 5; i++)
            {
                int a = rand() % 26, b = rand() % 26;
                char t = key[a]; key[a] = key[b]; key[b] = t;
            }
        double s = climb(key);
        int dup = 0;
        for (i = 0; i < nc; i++) if (!strcmp(cand[i].key, key)) dup = 1;
        if (!dup) { strcpy(cand[nc].key, key); cand[nc].score = s; nc++; }
    }
    qsort(cand, nc, sizeof(Cand), cmp);

    if (top > nc) top = nc;
    printf("\nTop %d possible plaintexts:\n", top);
    for (r = 0; r < top; r++)
    {
        printf("%2d. (score %.1f) ", r + 1, cand[r].score);
        for (i = 0; text[i]; i++)
            putchar(isalpha((unsigned char)text[i]) ? cand[r].key[toupper((unsigned char)text[i]) - 'A'] : text[i]);
        printf("\n");
    }
    return 0;
}
