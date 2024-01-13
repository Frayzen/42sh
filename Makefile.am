SUBDIRS = src \
		  tests/unit

all:42sh

42sh: src/42sh

src/42sh: .FORCE
	make -C src

unit: src/42sh .FORCE
	make -C tests/unit
	mv tests/unit/unit .
	./unit --verbose

funct: 42sh .FORCE
	chmod +x ./tests/funct/launch.sh
	./tests/funct/launch.sh

check: unit funct

clean-local:
	$(RM) src/42sh unit
.FORCE:

.phony: unit funct 42sh .FORCE check
