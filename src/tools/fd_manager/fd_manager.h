#ifndef FD_MANAGER_H
#define FD_MANAGER_H

#define DICT_SIZE 1024
#define DICT (get_dict(NULL))

struct fd_pair
{
    int old_fd;
    int new_fd;
};

struct fd_dictionary
{
    struct fd_pair entries[DICT_SIZE];
    int nb_entries;
};

/***
 * Initialize the file descriptor dictionary
 * @return the new file descriptor dictionary
 */
struct fd_dictionary *dict_init(void);

/***
 * Gets the file descriptor dictonary
 * @param dico file descripor dictonary to set or NULL
 * @return the file descriptor dictonary
 */
struct fd_dictionary *get_dict(struct fd_dictionary *dico);

/***
 * Adds an entry to the file descriptor dictionary
 * @param old_fd original file descriptor
 * @param new_fd new redirected file descriptor
 */
void dict_push(int old_fd, int new_fd);

/***
 * Gives the new file descriptor associated with the file descriptor given
 * @param old_fd original file descriptor
 * @return the new redirected file descriptor
 */
int dict_peek_new(int old_fd);

/***
 * Gives the old file descriptor associated with the file descriptor given
 * @param new_fd new redirected file descriptor
 * @return the original file descriptor
 */
int dict_peek_old(int new_fd);

/***
 * Removes the entry associated with a file descriptor from the file descriptor
 * dictionary
 * @param old_fd original file descriptor to remove from the entries
 */
void dict_pop(int old_fd);

/***
 * Frees the file descriptor dictionary
 */
void dict_free(void);

#endif /* !FD_MANAGER_H */
