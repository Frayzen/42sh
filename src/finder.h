#ifndef FINDER_H
#define FINDER_H

char *finder(char *string, int offset);

enum type
{
      IF,
      THEN,
      ELIF,
      ELSE,
      FI,
      SEMI_COLON,
      BACK_N,
      SINGLE_QUOTE,
      WORD,
      MAX_TYPE,
};

static const char *const type_names[] =
{
      [IF] = "if",
      [THEN] = "then",
      [ELIF] = "elif",
      [ELSE] = "else",
      [FI] = "fi",
      [SEMI_COLON] = ";",
      [BACK_N] = "\n",
      [SINGLE_QUOTE] = "'",
};

#endif /* FINDER_H */
