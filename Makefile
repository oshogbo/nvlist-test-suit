#
# Copyright (c) 2015 Mariusz Zaborski <oshogbo@FreeBSD.org>
#

include make/pre_common.mk
include make/nvlistfiles.mk

PROGS=	packunpack
PROGS+=	genafldata
PROGS+=	unpack

.PHONY: ${PROGS} clean allclean

all: ${PROGS}

${PROGS}: ${NVLISTOBJ}
	$(MAKE) -f make/Makefile.$@ LIBNV=${LIBNV} INC=${INC}

clean:
	rm -f $(addprefix ${BINDIR}/, $(PROGS))
	rm -f ${OBJDIR}/*.o

allclean: clean
	rm -fR aflresult/*
	rm -f testdata/nvlist.*

${OBJDIR}/%.o: ${LIBNV}/%.c
	$(CC) -c $(CFLAGS) -o $@ $< -I${LIBNV} -I${LIBNVCONTRIB}

${OBJDIR}/%.o: ${LIBNVCONTRIB}/%.c
	$(CC) -c $(CFLAGS) -o $@ $< -I${LIBNV} -I${LIBNVCONTRIB}
