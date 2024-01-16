#ifndef FD_MANAGER_H
#define FD_MANAGER_H

#define DICTIONARY_SIZE 1024
#define FD_DICTIONARY (get_fd_dictionary(NULL))

struct fd_pair
{
    int old_fd;
    int new_fd;
};

struct fd_dictionary
{
    struct fd_pair entries[DICTIONARY_SIZE];
    int nb_entries;
};

/***
 * Gets the file descriptor dictonary
 * @param dico file descripor dictonary to set or NULL
 * @return the file descriptor dictonary
 */
struct fd_dictionary *get_fd_dictionary(struct fd_dictionary *dico);

/***
 * Adds an entry to the file descriptor dictionary
 * @param old_fd original file descriptor
 * @param new_fd new redirected file descriptor
 */
void add_to_fd_dico(int old_fd, int new_fd);

/***
 * Gives the new file descriptor associated with the file descriptor given
 * @param old_fd original file descriptor
 * @return the new redirected file descriptor
 */
int get_new_fd(int old_fd);

/***
 * Gives the old file descriptor associated with the file descriptor given
 * @param new_fd new redirected file descriptor
 * @return the original file descriptor
 */
int get_old_fd(int new_fd);

/***
 * Removes the entry associated with a file descriptor from the file descriptor
 * dictionary
 * @param old_fd original file descriptor to remove from the entries
 */
void remove_form_fd_dictionary(int old_fd);

/***
 * Frees the file descriptor dictionary
 */
void free_fd_dictionary(void);

#endif /* !FD_MANAGER_H */
