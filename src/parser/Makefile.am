lib_LIBRARIES = libparser.a

libparser_a_SOURCES = \
	grammar/gr_element.c \
	grammar/gr_entry.c \
	grammar/gr_simple_command.c \
	grammar/gr_command.c \
	grammar/gr_pipeline.c \
	grammar/gr_and_or.c \
	grammar/gr_list.c \
	grammar/rules.h
libparser_a_CPPFLAGS = -I$(top_srcdir)/src
libparser_a_CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic
