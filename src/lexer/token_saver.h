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
 * Retrieve the second token from the buffer, do not change the buffer
 * @return The second token of the buffer
 * @see tok_pop() to access the next token
 */
struct token *tok_peek2(void);

/***
 * Jump to the next token of the buffer and destroy the token (INCLUDING the
 * string)
 * @return true if the jump happened, false if the buffer is empty
 * @see tok_peek() to get the current token value
 */
void tok_pop_clean(void);

/***
 * Jump to the next token of the buffer and free the token (but NOT the string)
 * @return true if the jump happened, false if the buffer is empty
 * @see tok_peek() to get the current token value
 */
void tok_pop(void);

/***
 * Swap the saved token
 * @param new the new saved token
 * @return the old token
 */
struct token *swap_next_token(struct token *new);

#endif /* !TOKEN_SAVER_H */
