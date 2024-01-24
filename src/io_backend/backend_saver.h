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

#endif /* !BACKEND_SAVER_H */
