# Test suite for nvlist library.

This repository contains random based stress test for nvlist library in FreeBSD.

## Compilation

$ gmake [LIBNV=/path/to/the/libnv.a] [INCLIBNV=/path/to/includes/] [CC=/path/to/the/afl-gcc]

## Nvlist compilation

libnv.a should be compiled using atf-gcc and -ggdb flag.
