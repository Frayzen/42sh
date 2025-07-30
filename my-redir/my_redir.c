#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#define ERROR(Err) fprintf(stderr, Err)

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        ERROR("TOO Few Args");
        return 2;
    }
    int fd = open(argv[1], 0644);
    pid_t pid = fork();
    if (pid == 0)
    {
        execvp();
    }

    int status;
    waitpid(pid, &status, 0);

    return 0;
}
