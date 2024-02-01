#ifndef PATH_UTILS_H
#    include <stdbool.h>

/***
 * Looks through all the possible path to see if the file exists
 * @param filename the file to look for
 * @param env_var the env_var containing the path
 * @param file_type_mask the argumen to give to access(2)
 * @return the valid path, NULL if doesn't exist
 */
char *look_file_path(char *filename, char *env_var, int file_type_mask);

#endif // !PATH_UTILS_H
