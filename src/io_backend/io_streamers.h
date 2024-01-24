#ifndef IO_STREAMERS_H
#define IO_STREAMERS_H

#include <stddef.h>
#include <stdio.h>

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
 * If no string can be read, a \0 is pushed to the io buffer
 * /!\ Do not forget to call main_to_stream before
 * @param size the maximum number of char to read
 */
void stream_input(size_t size);

/***
 * Open and read the file given with the path
 * @param path_to_file the path to the file to read
 */
void io_streamer_file(char *path_to_file);

/***
 * Swap old and new fd
 * @param new_file the fd to swap
 * @return the fd swapped
 */
FILE *swap_fd(FILE *new_file);

/***
 * Load file given with path (need to be valid)
 * @param path_to_file the file to load
 * @return the fd of the old file
 */
FILE *load_file(char *path_to_file);
#endif /* !IO_STREAMERS_H */
