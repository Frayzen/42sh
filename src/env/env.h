#ifndef ENV_H
#define ENV_H

// need to add th defualt value in the get_env_flag function for each new flag
struct env_flag
{
    int print;
    int verbose;
};

/***
 * get the environment flag structure
 * @return a pointer to the environment flag structure
 */
struct env_flag *get_env_flag();

#endif /* ENV_H */
