#
# Copyright (c) 2015 Mariusz Zaborski <oshogbo@FreeBSD.org>
#

PROGS=	packunpack

.PHONY: ${PROGS}

all: ${PROGS}

${PROGS}:
	$(MAKE) -f make/Makefile.$@


