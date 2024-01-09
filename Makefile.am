bin_PROGRAMS = 42sh

42sh_CFLAGS = -std=c99 -pedantic -Werror -Wall -Wextra -Wvla
42sh_CPPFLAGS = -I$(top_srcdir)/src
42sh_SOURCES = \
	src/io_backend/backend_saver.c \
	src/main.c

42sh_LDADD = src/io_backend/libiobackend.a

SUBDIRS = src/io_backend
