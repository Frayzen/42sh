
SUBDIRS = src \
		  tests/unit

42sh:
	make -C src
	mv src/42sh .

unit: 42sh
	make -C tests/unit
	mv tests/unit/unit .

funct:
	make -C tests/funct
	mv tests/funct/funct .

clean-local:
	$(RM) 42sh unit funct
