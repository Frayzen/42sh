check_PROGRAMS = io_backend_test

io_backend_test_SOURCES = \
					 io_backend/backend_saver.c
io_backend_test_CPPFLAGS = -I$(top_srcdir)/src
io_backend_test_LDADD = \
				   -lcriterion \
				   $(top_builddir)/src/io_backend/libiobackend.a

check-local:
	./io_backend_test
	
