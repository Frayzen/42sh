lib_LIBRARIES = libexec.a

libexec_a_SOURCES = \
	execs.h\
	exec_commands.c\
	exec_entry.c\
	exec_list.c \
	exec_condition.c
libexec_a_CPPFLAGS = -I$(top_srcdir)/src
libexec_a_CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic
