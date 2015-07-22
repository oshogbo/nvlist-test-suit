# Test suite for nvlist library.

This repository contains random based stress test for nvlist library in FreeBSD.

## Compilation

$ gmake [LIBNV=/path/to/the/libnv] [LIBNVCONTRIB=/path/to/the/contib/libnv]
	[INC=/path/to/includes/] [CC=/path/to/the/afl-clang]

## Run alf

$ ./alf-fuzz -i testdata -o aflresult bin/unpack @@
