bin_PROGRAMS = 42sh

42sh_CFLAGS = -std=c99 -pedantic -Werror -Wall -Wextra -Wvla
42sh_CPPFLAGS = -Isrc
42sh_SOURCES = \
	src/io_backend/backend_saver.c \
	src/main.c
42sh_LDADD = $(top_builddir)/src/io_backend/libiobackend.a

SUBDIRS = \
		  src/io_backend
