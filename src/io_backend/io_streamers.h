#ifndef IO_STREAMERS_H
#define IO_STREAMERS_H

/***
 * According to the arguments given to main, calls the right
    streamer function
 * @param argc number of arguments given to the function 42sh
 * @param argc list of the arguments given to the function 42sh
 * @return nothing
 */

#include <stdio.h>
void main_to_stream(int argc, char **argv);

/***
 * Return the following of the current stream
 * @return the next character
 */
char stream_next(void);

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

void io_streamer_stdin(void);

void io_streamer_string(int argc, char **argv);
#endif /* !IO_STREAMERS_H */
