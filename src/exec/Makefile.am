lib_LIBRARIES = libexec.a

libexec_a_SOURCES = \
	builtins/builtins.h\
	builtins/unset.c\
	builtins/echo.c\
	builtins/dot.c\
	builtins/exit.c\
	builtins/continue_break.c\
	execs.h\
	exec_and_or.c\
	commands/exec_commands.c\
	commands/exec_pipe.c\
	commands/exec_cmd.h\
	exec_entry.c\
	exec_list.c \
	exec_condition.c\
	exec_loop.c\
	exec_for.c
libexec_a_CPPFLAGS = -I$(top_srcdir)/src
libexec_a_CFLAGS = $(CFLAGS_42SH)
