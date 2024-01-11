# define the sub directories

bin_PROGRAMS = unit

LIBS = $(addprefix $(top_builddir)/src/, @LIB_LIST@)

unit_SOURCES =\
			  io_backend/backend_saver.c \
			  tools/pretty_print.c

unit_CPPFLAGS = -I$(top_srcdir)/src -I.

unit_CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic -g

unit_LDADD =  \
		$(LIBS) \
		-lcriterion
