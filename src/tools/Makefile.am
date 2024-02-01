lib_LIBRARIES = libtools.a

libtools_a_SOURCES = \
	definitions.h\
	redirection/redirection.h \
	redirection/redirection.c \
	str/string.c str/string.h\
	token/token.c token/token.h\
	ast/ast.c ast/ast.h\
	definitions.h\
	str/string.h str/string.c\
	ast/ast_utils.c ast/ast_utils.h\
	assignment/assignment.c assignment/assignment.h\
	pretty_print/pretty_print.h pretty_print/pretty_print.c\
	fd_manager/fd_dictionary.c fd_manager/fd_dictionnary.h\
	funct_dict/funct_dict.c funct_dict/funct_dict.h
libtools_a_CPPFLAGS = -I$(top_srcdir)/src
libtools_a_CFLAGS = $(CFLAGS_42SH)
