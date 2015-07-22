#
# Copyright (c) 2015 Mariusz Zaborski <oshogbo@FreeBSD.org>
#

LIBNV?=-lnv
INCLIBNV?=/usr/local/include/sys

CC=	atf-gcc
LIBS=	${LIBNV}
BINDIR=	bin
SRCDIR=	src
OBJDIR=	obj
CFLAGS=	-Wall -Wextra -I${INCLIBNV} -ggdb
