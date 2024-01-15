#ifndef IO_STREAMERS
#define IO_STREAMERS

#include <stddef.h>

#define _POSIX_C_SOURCE 200809L

/***
 * According to the arguments given to main, calls the right
    streamer function
 * @param argc number of arguments given to the function 42sh
 * @param argc list of the arguments given to the function 42sh
 * @return nothing
 */

void main_to_stream(int argc, char **argv);

/***
 * Append using io_push the next string of max length size
 * /!\ Do not forget to call main_to_stream before
 * @param size the maximum number of char to read
 * @return the number of character read
 */
size_t stream_input(size_t size);

#endif /* !IO_STREAMERS */
