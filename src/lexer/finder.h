#ifndef FINDER_H
#define FINDER_H

/***
 * finder: gets character by character until the word is recognizable, or t here
 * @get_backend function: to look at the next character without popping it
 * @pop_backend function: pops the character in backend so the next one is
 * available
 * @return the string to be used for the token
 */
char *finder(void);

#endif /* FINDER_H */
