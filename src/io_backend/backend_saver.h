#ifndef BACKEND_SAVER_H
#define BACKEND_SAVER_H

#define RINGBUFSIZE 1024

struct ringbuffer{
    char value[RINGBUFSIZE];
    char *begin;
    char *end;
};

void io_put(char *str);
char io_get_char(void);
char io_pop(void);

#endif /* !BACKEND_SAVER_H */
