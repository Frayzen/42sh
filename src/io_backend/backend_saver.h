#ifndef BACKEND_SAVER_H
#define BACKEND_SAVER_H

/***
 * Retrieve the next char of the current io input
 * @return the character
 */
char io_peek(void);

/***
 * Jump to the next char of the buffer and discard the current
 */
void io_pop(void);

/***
 * Swap the saved char
 * @param new the new saved char
 * @return the old char
 */
char swap_next_char(char new);

#endif /* !BACKEND_SAVER_H */
