#
# Copyright (c) 2015 Mariusz Zaborski <oshogbo@FreeBSD.org>
#

include make/pre_common.mk

PROGS=	libnv
PROGS+=	packunpack
PROGS+=	genafldata
PROGS+=	unpack

.PHONY: ${PROGS} clean

all: ${PROGS}

${PROGS}:
	$(MAKE) -f make/Makefile.$@ LIBNV=${LIBNV} INCLIBNV=${INCLIBNV}

clean:
	rm -f $(addprefix ${BINDIR}/, $(PROGS))
	rm -f ${OBJDIR}/*.o
	rm -f testdata/nvlist.*
