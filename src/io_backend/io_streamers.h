#ifndef IO_STREAMERS_H
#define IO_STREAMERS_H

/***
 * According to the arguments given to main, calls the right
    streamer function
 * @param argc number of arguments given to the function 42sh
 * @param argc list of the arguments given to the function 42sh
 * @return nothing
 */

void main_to_stream(int argc, char **argv);

/***
 * Return the following of the current stream
 * @return the next character
 */
char stream_next(void);

#endif /* !IO_STREAMERS_H */
