# define the sub directories

bin_PROGRAMS = unit

LIBS = $(addprefix $(top_builddir)/src/, @LIB_LIST@)

unit_SOURCES =\
			  exec/unit_exec.c\
			  exec/unit_redirs.c\
			  io_backend/unit_streamers.c \
			  token/unit_token.c \
			  parser/grules/unit_easy.c \
			  lexer/unit_finder.c\
			  lexer/unit_tokenizer.c\
			  lexer/unit_token_saver.c\
			  io_backend/unit_backend_saver.c\
			  fd_manager/unit_dict.c


unit_CPPFLAGS = -I$(top_srcdir)/src -I.

unit_CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic -Wno-type-limits -fsanitize=address

unit_LDADD =  \
			  $(LIBS) \
			  -lcriterion
