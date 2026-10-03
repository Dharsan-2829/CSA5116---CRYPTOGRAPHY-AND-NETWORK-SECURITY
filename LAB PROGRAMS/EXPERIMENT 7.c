#include <stdio.h>
#include <string.h>

/* Decrypting the simple-substitution ciphertext (the "Gold-Bug" cipher).
   Symbols that are not on a normal keyboard are written with ASCII stand-ins:
        double dagger -> #     dagger -> +     pilcrow -> @     em dash -> -
   Step 1: count how often each symbol occurs.
   Step 2: most frequent symbol (8) = e ; the trigram ;48 appears very often = "the"  => ;=t 4=h 8=e
   Step 3: deduce more words (e.g. "that", "and", "north", "degrees") to get the rest.        */
int main()
{
    const char *ct =
        "53##+305))6*;4826)4#.)4#);806*;48+8@60))85;;]8*;:#*8+83"
        "(88)5*+;46(;88*96*?;8)*#(;485);5*+2:*#(;4956*2(5*-4)8@8*"
        ";4069285);)6+8)4##;1(#9;48081;8:8#1;48+85;4)485+528806*81"
        "(#9;48;(88;4(#?34;48)4#;161;:188;#?;";

    int count[256] = {0}, i, j;
    for (i = 0; ct[i]; i++) count[(unsigned char)ct[i]]++;

    printf("Symbol frequencies (most frequent first):\n");
    int done[256] = {0};
    for (j = 0; j < 8; j++)
    {
        int best = -1;
        for (i = 0; i < 256; i++)
            if (count[i] && !done[i] && (best < 0 || count[i] > count[best])) best = i;
        done[best] = 1;
        printf("  %c : %d\n", best, count[best]);
    }

    /* substitution key obtained after the analysis */
    char map[256] = {0};
    map['5'] = 'a'; map['2'] = 'b'; map['-'] = 'c'; map['+'] = 'd'; map['8'] = 'e';
    map['1'] = 'f'; map['3'] = 'g'; map['4'] = 'h'; map['6'] = 'i'; map['0'] = 'l';
    map['9'] = 'm'; map['*'] = 'n'; map['#'] = 'o'; map['.'] = 'p'; map['('] = 'r';
    map[')'] = 's'; map[';'] = 't'; map['?'] = 'u'; map['@'] = 'v'; map[']'] = 'w';
    map[':'] = 'y';

    printf("\nCiphertext:\n%s\n\nPlaintext:\n", ct);
    for (i = 0; ct[i]; i++) putchar(map[(unsigned char)ct[i]] ? map[(unsigned char)ct[i]] : '?');

    printf("\n\nReadable form: A good glass in the bishop's hostel in the devil's seat twenty-one degrees\n");
    printf("and thirteen minutes northeast and by north main branch seventh limb east side shoot\n");
    printf("from the left eye of the death's-head a bee line from the tree through the shot fifty feet out.\n");
    return 0;
}
