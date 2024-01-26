lib_LIBRARIES = libparser.a

libparser_a_SOURCES = \
	command/expansion.c \
	command/expansion.h \
	command/expander.c \
	command/expander.h \
	grammar/gr_element.c \
	grammar/gr_entry.c \
	grammar/gr_simple_command.c \
	grammar/gr_command.c \
	grammar/gr_pipeline.c \
	grammar/gr_and_or.c \
	grammar/gr_list.c \
	grammar/gr_if.c \
	grammar/gr_shell_cmd.c \
	grammar/gr_compound_list.c \
	grammar/gr_else.c \
	grammar/gr_redir.c \
	grammar/gr_prefix.c \
	grammar/gr_while.c \
	grammar/gr_until.c \
	grammar/gr_for.c \
	tools/gr_utils.c \
	tools/gr_utils.h \
	tools/gr_tools.c \
	tools/gr_tools.h \
	grammar/rules.h
libparser_a_CPPFLAGS = -I$(top_srcdir)/src
libparser_a_CFLAGS = $(CFLAGS_42SH)
