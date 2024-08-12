SUBDIRS = src \
		  tests

export LIB_LIST=exec/libexec.a \
		 parser/libparser.a\
		 lexer/liblexer.a \
		 io_backend/libiobackend.a \
		 exit/libexit.a \
		 env/libenv.a \
		 tools/libtools.a \
		 arg_parser/libargparser.a\
		 arg_saver/libargsaver.a

export CFLAGS_42SH=-std=c99 -Werror -Wall -Wextra -Wvla -pedantic -O2 -fsanitize=address

setup: clean
	bear -- make -j8

funct_errors: all
	chmod +x ./tests/funct/*.sh
	./tests/funct/launch.sh errors

funct: all 
	chmod +x ./tests/funct/*.sh
	./tests/funct/launch.sh all

# check-local: funct

.phony: funct funct_errors check
