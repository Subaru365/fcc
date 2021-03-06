#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Token value
enum
{
    TK_NUM = 256,
    TK_EOF,
};

// Token type
typedef struct
{
    int ty;
    int val;
    char *input;
} Token;

Token tokens[100];

void tokenize(char *p)
{
    int i = 0;
    while (*p)
    {
        // Skip spaces
        if (isspace(*p))
        {
            p++;
            continue;
        }

        if (*p == '+' || *p == '-')
        {
            tokens[i].ty = *p;
            tokens[i].input = p;
            i++;
            p++;
            continue;
        }

        if (isdigit(*p))
        {
            tokens[i].ty = TK_NUM;
            tokens[i].input = p;
            tokens[i].val = strtol(p, &p, 10);
            i++;
            continue;
        }

        fprintf(stderr, "Can't tokenize: %s\n", p);
        exit(1);
    }

    tokens[i].ty = TK_EOF;
    tokens[i].input = p;
}

// Error handler
void error(int i)
{
    fprintf(stderr, "Unexpected Token: %s\n", tokens[i].input);
    exit(1);
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Invalid args count\n");
        return 1;
    }

    tokenize(argv[1]);

    printf(".intel_syntax noprefix\n");
    printf(".global _main\n");
    printf("_main:\n");

    // Ensure that initial value is number
    if (tokens[0].ty != TK_NUM)
        error(0);
    printf("    mov rax, %d\n", tokens[0].val);

    // Print assembly
    int i = 1;
    while (tokens[i].ty != TK_EOF)
    {
        if (tokens[i].ty == '+')
        {
            i++;
            if (tokens[i].ty != TK_NUM)
                error(i);
            printf("    add rax, %d\n", tokens[i].val);
            i++;
            continue;
        }

        if (tokens[i].ty == '-')
        {
            i++;
            if (tokens[i].ty != TK_NUM)
                error(i);
            printf("    sub rax, %d\n", tokens[i].val);
            i++;
            continue;
        }

        error(i);
    }

    printf("    ret\n");
    return 0;
}
