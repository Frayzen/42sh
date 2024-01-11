#ifndef IO_STREAMERS
#define IO_STREAMERS

/***
 * Retrieve the content of the string after the -c option
 * @param argc number of arguments given to the function 42sh
 * @param argc list of the arguments given to the function 42sh
 * @return the string following "-c"
 */
char *io_streamer_string(int argc, char **argv);

/***
 * Retrieve the content of stdin in a string
 * @return the string entered on stdin
 */
char *io_streamer_stdin(void);

#endif /* !IO_STREAMERS */
