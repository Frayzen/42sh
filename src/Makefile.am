# define the sub directories
SUBDIRS = \
		  io_backend\
		  lexer\
		  parser\
		  tools\
		  exec\
		  exit\
		  env\
		  arg_parser\
		  .

TOP_BD=$(top_builddir)
LIBS=$(addprefix $(TOP_BD)/src/, $(LIB_LIST))

bin_PROGRAMS = 42sh

42sh_SOURCES = main.c
42sh_CPPFLAGS = -I$(top_srcdir)/src
42sh_CFLAGS = $(CFLAGS_42SH)
42sh_LDADD = $(LIBS)

# debug_CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic -O2 -g
# debug_CPPFLAGS = -I%D%
# debug_SOURCES = debug.c
# debug_LDADD = $(LIBS)
