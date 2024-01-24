#ifndef TOKEN_SAVER_H
#define TOKEN_SAVER_H

#include "tools/token/token.h"

/***
 * Retrieve the first token from the buffer, do not change the buffer
 * @return The first token of the buffer
 * @see tok_pop() to access the next token
 */
struct token *tok_peek(void);

/***
 * Jump to the next token of the buffer and destroy the token (INCLUDING the
 * string)
 * @return true if the jump happened, false if the buffer is empty
 * @see tok_peek() to get the current token value
 */
bool tok_pop_clean(void);

/***
 * Jump to the next token of the buffer and free the token (but NOT the string)
 * @return true if the jump happened, false if the buffer is empty
 * @see tok_peek() to get the current token value
 */
bool tok_pop(void);

/***
 * Clean the memory saved by the buffer, the buffer is not usable after this
 */
void clean_token_saver(void);

/***
 * Consume all the tokens to ensure a proper exit (grammar error)
 */
void comsume_all(void);

/***
 * Swap token buffers
 * @param new, the buffer to load, NULL only retrieve the current one
 * @return the current loaded buffer
 */
struct ringbuffer *swap_token_buffer(struct ringbuffer *new)

#endif /* !TOKEN_SAVER_H */
