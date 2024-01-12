#ifndef IO_STREAMERS
#define IO_STREAMERS

/***
 * Retrieve the content of a file in a string
 * @param path_to_file path to the file
 * @return string conataining the content of the file
 */
void io_streamer_file(char *path_to_file);

/***
 * Retrieve the content of the string after the -c option
 * @param argc number of arguments given to the function 42sh
 * @param argc list of the arguments given to the function 42sh
 * @return the string following "-c"
 */
void io_streamer_string(int argc, char **argv);

/***
 * Retrieve the content of stdin in a string
 * @return the string entered on stdin
 */
void io_streamer_stdin(void);

#endif /* !IO_STREAMERS */
