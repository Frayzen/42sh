SUBDIRS = src \
		  tests/unit

42sh: src/42sh
	mv src/42sh .

src/42sh: .FORCE
	make -C src

unit: src/42sh .FORCE
	make -C tests/unit
	mv tests/unit/unit .

funct: 42sh .FORCE
	chmod +x ./tests/funct/launch.sh
	./tests/funct/launch.sh

clean-local:
	$(RM) 42sh unit funct
.FORCE:

.phony: unit funct 42sh .FORCE
