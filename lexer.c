#include "lexer.h"
#include "token.h"
#include "buffer.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

static FILE* file;
static char_t look;
static uint32_t row;
static uint32_t col;

typedef struct
{
    const char_t* name;
    const token_type_t token_type;
} keyword_t;

static const keyword_t KEYWORDS[] = {
    {"var", TK_VAR},
    {"let", TK_LET},
    {"func", TK_FUNC},
    {"true", TK_TRUE},
    {"false", TK_FALSE},
    {"if", TK_IF},
    {"else", TK_ELSE},
    {"loop", TK_LOOP},
    {"while", TK_WHILE},
    {"for", TK_FOR},
    {"in", TK_IN},
    {"break", TK_BREAK},
    {"continue", TK_CONTINUE},
    {"ret", TK_RETURN},
    {"print", TK_PRINT},
    {"read", TK_READ},
    {"quit", TK_QUIT},
    {"and", TK_AND},
    {"or", TK_OR},
    {"not", TK_NOT},
    {"int", TK_INT},
    {"str", TK_STR},
    {"real", TK_REAL},
    {"bool", TK_BOOL},
    {"void", TK_VOID},
    {"extern", TK_EXTERN},
    {"__line__", TK_LINE},
};

void lexer_init(const char_t* filename)
{
    row = 0;
    col = 0;
    look = 0;
    file = fopen(filename, "r");
}

void lexer_free()
{
    fclose(file);
}

uint32_t lexer_row()
{
    return row;
}

uint32_t lexer_col()
{
    return col;
}

void lexer_skip_white()
{
    while ((look = fgetc(file)) != EOF && isspace(look))
    {
        col++;

        if (look == '\n')
        {
            row++;
            col = 0;
        }
    }
}

void lexer_skip_line_comment()
{
    while (look == '#')
    {
        while ((look = fgetc(file)) != EOF && look != '\n')
        {
        }

        row++;
        col = 0;

        lexer_skip_white();
    }
}

int is_ident_char(int c)
{
    return isalnum(c) || c == '_';
}

int fpeek(FILE *stream)
{
    int c = fgetc(stream);
    ungetc(c, stream);
    return c;
}

token_type_t find_keyword(const char_t* name)
{
    for (int i = 0; i < sizeof (KEYWORDS) / sizeof (KEYWORDS[0]); i++)
    {
        if (strcmp(KEYWORDS[i].name, name) == 0)
            return KEYWORDS[i].token_type;
    }
    return TK_BAD;
}

token_t lexer_next()
{
    lexer_skip_white();
    lexer_skip_line_comment();

    token_t token;
    token.type = TK_BAD;
    token.row = row;
    token.col = col;
    token.value.as_long = 0;

    col++;

    if (feof(file))
    {
        token.type = TK_FIN;
    }
    else if (look == '{')
    {
        token.type = TK_L_BRACE;
    }
    else if (look == '}')
    {
        token.type = TK_R_BRACE;
    }
    else if (look == '(')
    {
        token.type = TK_L_PAREN;
    }
    else if (look == ')')
    {
        token.type = TK_R_PAREN;
    }
    else if (look == '[')
    {
        token.type = TK_L_BRACKET;
    }
    else if (look == ']')
    {
        token.type = TK_R_BRACKET;
    }
    else if (look == ',')
    {
        token.type = TK_COMMA;
    }
    else if (look == ':')
    {
        token.type = TK_COLON;
    }
    else if (look == '.' && fpeek(file) == '.')
    {
        token.type = TK_DOTDOT;
        look = fgetc(file);
    }
    else if (look == '.')
    {
        token.type = TK_PERIOD;
    }
    else if (look == ';')
    {
        token.type = TK_SEMICOLON;
    }
    else if (look == '+')
    {
        token.type = TK_PLUS;
    }
    else if (look == '-')
    {
        token.type = TK_MINUS;
    }
    else if (look == '*')
    {
        token.type = TK_MUL;
    }
    else if (look == '/')
    {
        token.type = TK_DIV;
    }
    else if (look == '%')
    {
        token.type = TK_MOD;
    }
    else if (look == '\\')
    {
        token.type = TK_BACKSLASH;
    }
    else if (look == '?')
    {
        token.type = TK_QUESTION;
    }
    else if (look == '+' && fpeek(file) == '+')
    {
        token.type = TK_INC;
        look = fgetc(file);
    }
    else if (look == '-' && fpeek(file) == '-')
    {
        token.type = TK_DEC;
        look = fgetc(file);
    }
    else if (look == '=' && fpeek(file) == '=')
    {
        token.type = TK_EQ;
        look = fgetc(file);
    }
    else if (look == '=')
    {
        token.type = TK_ASSIGN;
    }
    else if (look == '>' && fpeek(file) == '=')
    {
        token.type = TK_GTE;
        look = fgetc(file);
    }
    else if (look == '>')
    {
        token.type = TK_GT;
    }
    else if (look == '<' && fpeek(file) == '=')
    {
        token.type = TK_LTE;
        look = fgetc(file);
    }
    else if (look == '<')
    {
        token.type = TK_LT;
    }
    else if (look == '!' && fpeek(file) == '=')
    {
        token.type = TK_NE;
        look = fgetc(file);
    }
    else if (isalpha(look) || look == '_')
    {
        buffer_t ident;
        buffer_init(&ident, 32);
        buffer_add(&ident, look);

        while (is_ident_char(fpeek(file)))
        {
            look = fgetc(file);
            col++;
            buffer_add(&ident, look);
        }

        buffer_add(&ident, '\0');
        buffer_shrink(&ident);

        token_type_t t = find_keyword((char_t*)ident.data);
        token.type = t == TK_BAD ? TK_IDENT : t;
        token.value.as_str = (char_t*) ident.data;
    }
    else if (isdigit(look))
    {
        bool_t has_dot = false;

        buffer_t number;
        buffer_init(&number, 32);
        buffer_add(&number, look);

        while (true)
        {
            char_t peek = fpeek(file);

            if (isdigit(peek) || peek == '.')
            {
                if (peek == '.')
                    has_dot = true;

                if ((look = fgetc(file)) != EOF)
                {
                    col++;
                    buffer_add(&number, look);
                    continue;
                }
            }
            break;
        }

        buffer_add(&number, '\0');
        buffer_shrink(&number);

        // long vs byte vs short vs int32 ?!
        // how about real, hex, octal, binary, bigint ?!

        if (has_dot)
        {
            byte_t* end = number.data + number.used - 1;
            token.type = TK_REAL;
            token.value.as_real = strtod((char_t*) number.data, (char_t**) &end);
        }
        else
        {
            byte_t* end = number.data + number.used - 1;
            token.type = TK_INT;
            token.value.as_int = strtol((char_t*) number.data, (char_t**) &end, 10);
        }
    }
    else if (look == '"')
    {
        buffer_t str;
        buffer_init(&str, 64);

        while (fpeek(file) != '"' && (look = fgetc(file)) != EOF)
        {
            if (look == '\\')
            {
                char_t escaped = 0;
                switch (fpeek(file))
                {
                case 'n':
                    escaped = '\n';
                    break;
                case 't':
                    escaped = '\t';
                    break;
                case 'a':
                    escaped = '\a';
                    break;
                case 'f':
                    escaped = '\f';
                    break;
                case 'r':
                    escaped = '\r';
                    break;
                case 'v':
                    escaped = '\v';
                    break;
                case '\\':
                    escaped = '\\';
                    break;
                case '"':
                    escaped = '"';
                    break;
                }

                if (escaped != 0)
                {
                    look = escaped;
                    fgetc(file);
                    col++;
                }
            }

            col++;
            buffer_add(&str, look);
        }

        look = fgetc(file);

        if (look != '"')
        {
            token.type = TK_BAD;
        }
        else
        {
            buffer_shrink(&str);

            token.type = TK_STR;
            token.value.as_str = (char_t*) str.data;
        }
    }

    return token;
}
