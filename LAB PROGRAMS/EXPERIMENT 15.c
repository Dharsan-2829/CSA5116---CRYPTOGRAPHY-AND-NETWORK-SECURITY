#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>

/* Letter-frequency attack on an additive (shift) cipher.
   All 26 shifts are tried; each candidate is scored with the English letter frequencies
   (log-likelihood) and the candidates are printed in order of likelihood.                   */
double eng[26] = {8.167, 1.492, 2.782, 4.253, 12.702, 2.228, 2.015, 6.094, 6.966, 0.153, 0.772, 4.025, 2.406,
                  6.749, 7.507, 1.929, 0.095, 5.987, 6.327, 9.056, 2.758, 0.978, 2.360, 0.150, 1.974, 0.074};

typedef struct { int shift; double score; char text[1000]; } Cand;

int cmp(const void *a, const void *b)
{
    double d = ((Cand *)b)->score - ((Cand *)a)->score;
    return (d > 0) - (d < 0);
}

int main()
{
    char ct[1000];
    Cand c[26];
    int top, s, i;

    printf("Enter ciphertext: ");
    fgets(ct, sizeof ct, stdin);
    ct[strcspn(ct, "\n")] = 0;
    printf("How many top plaintexts do you want (1-26)? ");
    scanf("%d", &top);
    if (top < 1) top = 1;
    if (top > 26) top = 26;

    for (s = 0; s < 26; s++)
    {
        c[s].shift = s;
        c[s].score = 0;
        for (i = 0; ct[i]; i++)
        {
            if (isalpha((unsigned char)ct[i]))
            {
                int p = (toupper((unsigned char)ct[i]) - 'A' - s + 26) % 26;
                c[s].text[i] = 'a' + p;
                c[s].score += log(eng[p] / 100.0);
            }
            else c[s].text[i] = ct[i];
        }
        c[s].text[i] = 0;
    }
    qsort(c, 26, sizeof(Cand), cmp);

    printf("\nTop %d possible plaintexts:\n", top);
    for (i = 0; i < top; i++)
        printf("%2d. key=%2d  score=%8.2f  %s\n", i + 1, c[i].shift, c[i].score, c[i].text);
    return 0;
}
