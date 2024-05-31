#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "parser.h"
#include "front.h"
#include <stdlib.h>

/* Global Variables */
int nextToken;
int problemtoken = 0;

/* Local Variables */
static int charClass;
static char lexeme[100];
static char nextChar;
static int lexLen;
static FILE *in_fp;

/* Local Function declarations */
static void addChar();
static void getChar();
static void getNonBlank();
void TokenOutput();/* Function to print token code as a string */
static void error(); 
void expr();
void term();
void factor();
static void error();
void operation();
void S();
void U();
void C();
void Ss();



/* main driver */
int main(int argc, char *argv[]) {
    char *file = argv[1];
    in_fp = fopen(file,"r");
    printf("DCooke Parser :: R11787304\n");

    if(argc != 2){
        // If statement to handle requirement 4
        printf("Error: No input file provided");
        exit(2);

    }

    if(in_fp == NULL){
        // handles requirement 5
        printf("Error: The file %s does not exist\n",file);
        exit(3);
    }
    else{

        getChar();
        lex();
        Ss();
        if(nextToken == EOF && problemtoken == 0){
            // Handles requirement 2
            printf("Syntax Validated\n");
            exit(0);
        }
        else{
            error();
            
        }
    }

    

    
    return 0;
}

/* lookup - a function to lookup operators and parentheses and return the token */
static int lookup(char ch) {
    switch (ch) {
        case '(':
            addChar();
            return LEFT_PAREN;
        case ')':
            addChar();
            return RIGHT_PAREN;
        case '+':
            addChar();
            getChar();
            if (nextChar == '+') {
                addChar();
                return INC_OP;
            } else if (nextChar == '-') {
                addChar();
                return UNKNOWN;
            }
            fseek(in_fp, -1, SEEK_CUR);
            return ADD_OP;
        case '-':
            addChar();
            getChar();
            if (nextChar == '-') {
                addChar();
                return DEC_OP;
            }
            fseek(in_fp, -1, SEEK_CUR);
            return SUB_OP;
        case '*':
            addChar();
            return MULT_OP;
        case '/':
            addChar();
            return DIV_OP;
        case '\0':
            addChar();
            return EOF;
        case '<':
            addChar();
            getChar();
            if (nextChar == '>') {
                addChar();
                return NEQUAL_OP;
            } else if (nextChar == '=') {
                addChar();
                return LEQUAL_OP;
            }
            fseek(in_fp, -1, SEEK_CUR);
            return LESSER_OP;
        case '>':
            addChar();
            getChar();
            if (nextChar == '=') {
                addChar();
                return GEQUAL_OP;
            }
            fseek(in_fp, -1, SEEK_CUR);
            return GREATER_OP;
        case '%':
            addChar();
            return MOD_OP;
        case ';':
            addChar();
            return SEMICOLON;
        case ':':
            addChar();
            getChar();
            if (nextChar == '=') {
                addChar();
                return ASSIGN_OP;
            } else {
                fseek(in_fp, -1, SEEK_CUR);
                return COLON;
            }
        case '=':
            addChar();
            return EQUAL_OP;
        default:
            addChar();
            return UNKNOWN;
    }
}

/* addChar - a function to add nextChar to lexeme */
static void addChar() {
    if (lexLen <= 98) {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = '\0';
    } else {
        printf("Error - lexeme is too long \n");
    }
}

/* getChar - a function to get the next character of input and determine its character class */
static void getChar() {
    if ((nextChar = getc(in_fp)) != EOF) {
        if (isalpha(nextChar))
            charClass = LETTER;
        else if (isdigit(nextChar))
            charClass = DIGIT;
        else
            charClass = UNKNOWN;
    } else {
        charClass = EOF;
    }
}

/* getNonBlank - a function to call getChar until it returns a non-whitespace character */
static void getNonBlank() {
    while (isspace(nextChar))
        getChar();
}

/* lex - a simple lexical analyzer for arithmetic expressions */
int lex() {
    lexLen = 0;
    getNonBlank();

    switch (charClass) {
        case LETTER:
            addChar();
            getChar();
            while (charClass == LETTER) {
                addChar();
                getChar();
            }
            if (strcmp(lexeme, "read") == 0) {
                nextToken = KEY_READ;
            } else if (strcmp(lexeme, "if") == 0) {
                nextToken = KEY_IF;
            } else if (strcmp(lexeme, "fi") == 0) {
                nextToken = KEY_FI;
            } else if (strcmp(lexeme, "else") == 0) {
                nextToken = KEY_ELSE;
            } else if (strcmp(lexeme, "then") == 0) {
                nextToken = KEY_THEN;
            } else if (strcmp(lexeme, "write") == 0) {
                nextToken = KEY_WRITE;
            } else {
                nextToken = IDENT;
            }
            break;
        case DIGIT:
            addChar();
            getChar();
            while (charClass == DIGIT) {
                addChar();
                getChar();
            }
            nextToken = INT_LIT;
            break;
        case UNKNOWN:
            nextToken = lookup(nextChar);
            getChar();
            break;
        case EOF:
            nextToken = EOF;
            return EOF;
            break;
    }

    /*printf("%s ", lexeme);
    printf("\t\t");
    TokenOutput(nextToken);
    printf("\n");*/

    return nextToken;
}

void TokenOutput(int nextToken) {
// Function to output token code as 

    switch (nextToken) {
        case INT_LIT:
            printf( "INT_LIT");
            break;
        case IDENT:
            printf("IDENT");
            break;
        case ASSIGN_OP:
            printf("ASSIGN_OP");
            break;
        case ADD_OP:
            printf("ADD_OP");
            break;
        case SUB_OP:
            printf("SUB_OP");
            break;
        case MULT_OP:
            printf("MULT_OP");
            break;
        case DIV_OP:
            printf("DIV_OP");
            break;
        case LEFT_PAREN:
            printf("LEFT_PAREN");
            break;
        case RIGHT_PAREN:
            printf("RIGHT_PAREN");
            break;
        case EOF:
            break;
        case KEY_READ:
            printf("KEY_READ");
            break;
        case LESSER_OP:
            printf("LESSER_OP");
            break;
        case GREATER_OP:
            printf("GREATER_OP");
            break;
        case EQUAL_OP:
            printf("EQUAL_OP");
            break;
        case KEY_WRITE:
            printf("KEY_WRITE");
            break;
        case COLON:
            printf("COLON");
            break;
        case SEMICOLON:
            printf("SEMICOLON");
            break;
        case MOD_OP:
            printf("MOD_OP");
            break;
        case KEY_ELSE:
            printf("KEY_ELSE");
            break;
        case KEY_THEN:
            printf("KEY_THEN");
            break;
        case KEY_IF:
            printf("KEY_IF");
            break;
        case KEY_FI:
            printf("KEY_FI");
            break;
        case DEC_OP:
            printf("DEC_OP");
            break;
        case INC_OP:
            printf("INC_OP");
            break;
        case NEQUAL_OP:
            printf("NEQUAL_OP");
            break;
        case LEQUAL_OP:
            printf("LEQUAL_OP");
            break;
        case GEQUAL_OP:
            printf("GEQUAL_OP");
            break;
        default:
            printf("UNKNOWN");
            break;
    }
}
void expr() {
//E ::= T | E + T | E - T
    term();
    while (nextToken == ADD_OP || nextToken == SUB_OP) {
        lex();
        term();
    }
}

void term() {
//T ::= F | T * F | T / F | T % F
    factor();
    while (nextToken == MULT_OP || nextToken == DIV_OP || nextToken == MOD_OP) {
        lex();
        factor();
    }
}

void operation() {
//O ::= ++V | --V
    if (nextToken == INC_OP || nextToken == DEC_OP) {
        lex();
        if (nextToken == IDENT) {
            lex();
        } else {
            error();
        }
    } else {
        error();
    }
}

void factor() {
//F ::= (E) | O | N | V
    if (nextToken == IDENT || nextToken == INT_LIT) {
        lex();
    } else if (nextToken == LEFT_PAREN) {
        lex();
        expr();
        if (nextToken == RIGHT_PAREN) {
            lex();
        }
        else{
            error();
        }
    } else if (nextToken == INC_OP || nextToken == DEC_OP) {
        operation();
    } else {
        error();
    }
}

static void error() {
    problemtoken++;
    printf("Error encountered: the next lexeme was %s and the next token was ", lexeme);
    TokenOutput(nextToken);
    exit(1);
    

    
}
//S;S
void Ss()
{
    if(nextToken != EOF){
        S();

        if(nextToken == SEMICOLON){
            lex();
            Ss();
        } 
    }
}
//S ::= V:=E | read(V) | write(E) | if C: then S U fi
void S() {

//S ::= V :=E
    switch (nextToken) {
        case IDENT:
            lex();
            if (nextToken == ASSIGN_OP) {
                lex();
                expr();
            } else {
                error();
            }
            break;
        case KEY_READ:
//read(V)
            lex();
            if (nextToken == LEFT_PAREN) {
                lex();
                if (nextToken == IDENT) {
                    lex();
                }

                if (nextToken == RIGHT_PAREN) {
                    lex();
                }
                else{
                    error();
                }
            }
            else{
                error();
            }

            break;
        case KEY_WRITE:
//Write(E)
            lex();
            if (nextToken == LEFT_PAREN) {
                lex();
                expr();
                if (nextToken == RIGHT_PAREN) {
                    lex();
                }
                else{
                    error();
                }
            }
            else{
                error();
            }
            break;
        case KEY_IF:
    //if C: then S U fi
            lex();
            C();
            if(nextToken == COLON){
                lex();
                if(nextToken == KEY_THEN){
                    lex();
                    Ss();
                    U();
                    if(nextToken == KEY_FI){
                        lex();
                    } else{ error();}
                } else{error();}
            }
            else{error();}

            break;


        default:
            error();
            break;
    }
}

void U() {
    //U ::= else S | ε
    if (nextToken == KEY_ELSE) {
        lex();
        Ss();
    }
}

void C() {
//C ::= E < E | E > E | E = E | E <> E | E <= E | E >= E | ( C )
if(nextToken == LEFT_PAREN){
    lex();
    C();
    if (nextToken == RIGHT_PAREN)
    {
        lex();
    }
}
else{
    expr();
    switch(nextToken){
        case LESSER_OP:
        lex();
        expr();
        break;
        case GREATER_OP:
        lex();
        expr();
        break;
        case EQUAL_OP:
        lex();
        expr();
        break;
        case NEQUAL_OP:
        lex();
        expr();
        break;
        case LEQUAL_OP:
        lex();
        expr();
        break;
        case GEQUAL_OP:
        lex();
        expr();
        break;
        default:
        error();
        break;
    }
    }
    

}