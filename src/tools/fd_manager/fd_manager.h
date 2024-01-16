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
 * Initialisize or get the file descriptor dictonary
 * @param dico file descripor dictonary to set or NULL
 * @return the file descriptor dictonary
 */
struct fd_dictionary *get_fd_dictionary(struct fd_dictionary *dico);

#endif /* !FD_MANAGER_H */
