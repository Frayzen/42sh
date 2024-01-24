#include "builtins.h"
#include <unistd.h>
#include <linux/limits.h>

static char *create_path_from_filename(char *filename, char *path)
{
  char new_path[PATH_MAX];
  srtcpy(new_path, path);
  new_path = strcat(new_path, filename);
  return new_path;
}

static char *look_file_path(char *filename)
{
  char *dup = strdup(getenv("PATH"));
  char *start = dup;
  char *end = NULL;
  do {
    end = strchr(start, ':');
    if (end)
    {
      end[0] = 0;
    }
    char *path_file = create_path_from_filename(filename, start);
    if (access(path_file, F_OK))
    {
      return path_file;
      free(dup);
    }
    start = p + 1;
  } while (end);
  free(dup);
  return NULL
}

int builtin_dot(char **argv)
{
  char *path = argv[1];
  if (!strchr(path, '/'))
  {
    path = look_file_path(path);
  }
  else if (!access(path, F_OK))
    path = NULL;
  if (!path)
  {
    dprintf(STDERR, "error path file\n");
    return 1;
  }
  //TODO exec file
  return 0;
  
}
