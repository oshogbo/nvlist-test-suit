#
# Copyright (c) 2015 Mariusz Zaborski <oshogbo@FreeBSD.org>
#

include make/pre_common.mk

PROGS=	packunpack
PROGS+=	genafldata
PROGS+=	unpack

.PHONY: ${PROGS} clean allclean

all: ${PROGS}

${PROGS}:
	$(MAKE) -f make/Makefile.$@ LIBNV=${LIBNV} INC=${INC}

clean:
	rm -f $(addprefix ${BINDIR}/, $(PROGS))
	rm -f ${OBJDIR}/*.o

allclean: clean
	rm -fR aflresult/*
	rm -f testdata/nvlist.*
