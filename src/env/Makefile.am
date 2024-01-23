lib_LIBRARIES = libenv.a

libenv_a_SOURCES = \
	env.c env.h \
	vars/vars.c vars/vars.h
libenv_a_CPPFLAGS = -I$(top_srcdir)/src
libenv_a_CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic
