SUBDIRS = src \
		  tests

export LIB_LIST=exec/libexec.a \
		 parser/libparser.a\
		 lexer/liblexer.a \
		 io_backend/libiobackend.a \
		 exit/libexit.a \
		 env/libenv.a \
		 tools/libtools.a

# all:42sh

# 42sh: src/42sh

# src/42sh: .FORCE
# 	make -C src

# unit: src/42sh .FORCE
# 	make -C tests/unit
# 	mv tests/unit/unit .
# 	./unit --verbose

# funct_errors: 42sh .FORCE
# 	chmod +x ./tests/funct/*.sh
# 	./tests/funct/launch.sh errors

# funct: 42sh .FORCE
# 	chmod +x ./tests/funct/*.sh
# 	./tests/funct/launch.sh all


# clean-local:
# 	$(RM) src/42sh unit
# .FORCE:

# .phony: unit funct 42sh .FORCE check
