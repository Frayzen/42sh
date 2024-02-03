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
		  arg_saver\
		  .

TOP_BD=$(top_builddir)
LIBS=$(addprefix $(TOP_BD)/src/, $(LIB_LIST))

bin_PROGRAMS = 42sh

42sh_SOURCES = main.c
42sh_CPPFLAGS = -I$(top_srcdir)/src
42sh_CFLAGS = $(CFLAGS_42SH)
42sh_LDADD = $(LIBS)
