#ifndef VAR_DICT_H
#define VAR_DICT_H

#define VAR_DICT_SIZE 1024
#define VAR_DICT (get_dict())
#include "tools/definitions.h"
struct var_pair
{
    char * key;
    char * value;
    BOOL env_var;
};

struct var_dict
{
    struct var_pair entries[VAR_DICT_SIZE];
    int nb_entries;
};

/***
 * Initialize or gets the file descriptor dictonary
 * @return the file descriptor dictonary
 */
struct var_dict *get_dict(void);

/***
 * Adds an entry to the file descriptor dict
 * @param key original file descriptor
 * @param value new redirected file descriptor
 */
void dict_push(char * key, char * value);

/***
 * Gives the new file descriptor associated with the file descriptor given
 * @param key original file descriptor
 * @return the new redirected file descriptor
 */
char * dict_peek_new(char * key);

/***
 * Gives the old file descriptor associated with the file descriptor given
 * @param value new redirected file descriptor
 * @return the original file descriptor
 */
char * dict_peek_old(char * value);

/***
 * Removes the entry associated with a file descriptor from the file descriptor
 * dict
 * @param key original file descriptor to remove from the entries
 */
void dict_pop(char * key);

/***
 * Frees the file descriptor dict
 */
void dict_free(void);

#endif /* !var_DICTIONNARY_H */
