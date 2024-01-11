#ifndef IO_STREAMERS
#define IO_STREAMERS

/***
 * Retrieve the content of a file in a string
 * @param path_to_file path to the file
 * @return string conataining the content of the file
 */
char *io_streamer_file(char *path_to_file);

/***
 * Retrieve the content of stdin in a string
 * @return the string entered on stdin
 */
char *io_streamer_stdin(void);

#endif /* !IO_STREAMERS */
