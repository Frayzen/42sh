#ifndef BACKEND_SAVER_H
#define BACKEND_SAVER_H

#include <stdbool.h>
#include <string.h>
#define RINGBUFSIZE 1024

struct ringbuffer{
    char value[RINGBUFSIZE];
    char *begin;
    char *end;
};

/***
 * Add string to the io backend buffer
 * @param str the string that should be null terminated and do not contain null char
 * @see io_put_chars() for non null terminated strings
 */
void io_put(char *str);

/***
 * Add char array to the io backend buffer
 * @param str the char array pointer
 * @param len the number of char to add
 * @see io_put() for null terminated string
 */
void io_put_chars(char *str, size_t len);

/***
 * Retrieve the first character from the buffer, do not change the buffer
 * @return The first character of the buffer
 * @see io_pop() to access the next char
 */
char io_get_char(void);

/***
 * Jump to the next char of the buffer
 * @return true if the jump happened, false if the buffer is empty
 * @see io_get_char() to get the current char value
 */
bool io_pop(void);

#endif /* !BACKEND_SAVER_H */
