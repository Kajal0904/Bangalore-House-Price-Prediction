#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX_LEN 31

char keywords[][10] = {
    "int", "float", "char", "if", "else", "while", "return"
};

int isKeyword(char *word) {
    int i;
    for(i = 0; i < 7; i++) {
        if(strcmp(word, keywords[i]) == 0)
            return 1;
    }
    return 0;
}

int isOperator(char ch) {
    return (ch == '+' || ch == '-' || ch == '*' || ch == '/' ||
            ch == '=' || ch == '!' || ch == '<' || ch == '>');
}

int isSpecialSymbol(char ch) {
    return (ch == ';' || ch == ',' || ch == '(' ||
            ch == ')' || ch == '{' || ch == '}');
}

int main() {
    FILE *fp;
    char ch, buffer[MAX_LEN];
    int i = 0;

    fp = fopen("input.txt", "r");

    if(fp == NULL) {
        printf("File not found\n");
        return 0;
    }

    while((ch = fgetc(fp)) != EOF) {

        // Ignore spaces, tabs, newline
        if(isspace(ch))
            continue;

        // Ignore single line comments
        if(ch == '/') {
            char next = fgetc(fp);

            if(next == '/') {
                while((ch = fgetc(fp)) != '\n');
            }

            // Ignore multi-line comments
            else if(next == '*') {
                char prev;
                while((ch = fgetc(fp)) != EOF) {
                    if(prev == '*' && ch == '/')
                        break;
                    prev = ch;
                }
            }

            else {
                printf("OPERATOR: /\n");
                ungetc(next, fp);
            }
        }

        // Identifier or Keyword
        else if(isalpha(ch)) {
            i = 0;
            buffer[i++] = ch;

            while(isalnum(ch = fgetc(fp)) && i < MAX_LEN) {
                buffer[i++] = ch;
            }

            buffer[i] = '\0';
            ungetc(ch, fp);

            if(isKeyword(buffer))
                printf("KEYWORD: %s\n", buffer);
            else
                printf("IDENTIFIER: %s\n", buffer);
        }

        // Integer constant
        else if(isdigit(ch)) {
            i = 0;
            buffer[i++] = ch;

            while(isdigit(ch = fgetc(fp))) {
                buffer[i++] = ch;
            }

            buffer[i] = '\0';
            ungetc(ch, fp);

            printf("INTEGER CONSTANT: %s\n", buffer);
        }

        // Operators
        else if(isOperator(ch)) {
            char next = fgetc(fp);

            if((ch == '=' && next == '=') ||
               (ch == '!' && next == '=') ||
               (ch == '<' && next == '=') ||
               (ch == '>' && next == '=')) {

                printf("OPERATOR: %c%c\n", ch, next);
            }

            else {
                printf("OPERATOR: %c\n", ch);
                ungetc(next, fp);
            }
        }

        // Special Symbols
        else if(isSpecialSymbol(ch)) {
            printf("SPECIAL SYMBOL: %c\n", ch);
        }
    }

    fclose(fp);
    return 0;
}