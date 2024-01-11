# define the sub directories

bin_PROGRAMS = unit

LIBS = $(addprefix $(top_builddir)/src/, @LIB_LIST@)

unit_SOURCES =\
			  io_backend/unit_backend_saver.c\
			  lexer/unit_token_saver.c\
			  lexer/unit_finder.c


unit_CPPFLAGS = -I$(top_srcdir)/src -I.

unit_CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic

unit_LDADD =  \
		$(LIBS) \
		-lcriterion
