#ifndef FD_DICT_H
#define FD_DICT_H

#define FUNCT_DICT_SIZE 1024
#define FUNCT_DICT (get_funct_dict())

struct funct_pair
{
    char *name;
    struct ast *body;
};

struct funct_dictionary
{
    struct funct_pair entries[FUNCT_DICT_SIZE];
    int nb_entries;
};

/***
 * Initialize or gets the file descriptor dictonary
 * @return the file descriptor dictonary
 */
struct funct_dictionary *get_funct_dict(void);

/***
 * Adds an entry to the file descriptor dictionary
 * @param old_fd original file descriptor
 * @param new_fd new redirected file descriptor
 */
void funct_dict_push(char *name, struct ast *body);

/***
 * Gives the new file descriptor associated with the file descriptor given
 * @param old_fd original file descriptor
 * @return the new redirected file descriptor
 */
struct ast *funct_dict_peek_value(char *name);

/***
 * Gives the old file descriptor associated with the file descriptor given
 * @param new_fd new redirected file descriptor
 * @return the original file descriptor
 */
char *funct_dict_peek_key(struct ast *body);

/***
 * Removes the entry associated with a file descriptor from the file descriptor
 * dictionary
 * @param old_fd original file descriptor to remove from the entries
 */
void funct_dict_pop(char *name);

/***
 * Frees the file descriptor dictionary
 */
void funct_dict_free(void);

#endif /* !FD_DICT_H */
