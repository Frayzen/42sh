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


#endif /* FINDER_H */
