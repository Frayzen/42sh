lib_LIBRARIES = libtools.a

libtools_a_SOURCES = \
	ring_buffer/ring_buffer.c ring_buffer/ring_buffer.h\
	token/token.c token/token.h\
	ast/ast_utils.c ast/ast_utils.h\
	ast/ast.c ast/ast.h
libtools_a_CPPFLAGS = -I$(top_srcdir)/src
libtools_a_CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic
