#ifndef BACKEND_SAVER_H
#define BACKEND_SAVER_H

#include <stdbool.h>
#include <string.h>

#define BACKEND_BUFFER_SIZE 1024

/***
 * Add string to the io backend buffer
 * @param str the string that should be null terminated and do not contain null
 * char
 * @see io_push_chars() for non null terminated strings
 */
void io_push(char *str);

/***
 * Add char array to the io backend buffer
 * @param str the char array pointer
 * @param len the number of char to add
 * @see io_push() for null terminated string
 */
void io_push_chars(char *str, size_t len);

/***
 * Retrieve the first character from the buffer, do not change the buffer
 * @return The first character of the buffer
 * @see io_pop() to access the next char
 */
char io_peek(void);

/***
 * Jump to the next char of the buffer
 * @return true if the jump happened, false if the buffer is empty
 * @see io_get_char() to get the current char value
 */
bool io_pop(void);

/***
 * Clean the memory saved by the buffer, the buffer is not usable after this
 */
void clean_backend_saver(void);

/***
 * Swap backend buffers, to use in dot builtin
 * @param new, the buffer to load, NULL only retrieve the current one
 * @return the current loaded buffer
 */
struct ringbuffer *swap_backend_buffer(struct ringbuffer *new);
#endif /* !BACKEND_SAVER_H */
