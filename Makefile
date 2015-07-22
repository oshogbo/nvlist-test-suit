#
# Copyright (c) 2015 Mariusz Zaborski <oshogbo@FreeBSD.org>
#

PROGS=	packunpack
PROGS+=	genafldata
PROGS+=	unpack

.PHONY: ${PROGS}

all: ${PROGS}

${PROGS}:
	$(MAKE) -f make/Makefile.$@


