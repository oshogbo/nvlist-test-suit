#
# Copyright (c) 2015 Mariusz Zaborski <oshogbo@FreeBSD.org>
#

LIBNV?=/usr/src/lib/libnv
LIBNVCONTRIB?=/usr/src/sys/contrib/libnv
INC?=/usr/include

CC=	atf-gcc
LIBS=
BINDIR=	bin
SRCDIR=	src
OBJDIR=	obj
CFLAGS=	-Wall -Wextra -I${INC}
