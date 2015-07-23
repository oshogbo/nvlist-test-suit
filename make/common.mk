#
# Copyright (c) 2015 Mariusz Zaborski <oshogbo@FreeBSD.org>
#

include make/nvlistfiles.mk

OBJ=$(addprefix ${OBJDIR}/, $(SRC:.c=.o))

all: $(NVLISTOBJ) $(TARGET)

${TARGET}: ${BINDIR}/${TARGET}

${BINDIR}/$(TARGET): $(OBJ)
	$(CC) -o $@ $(OBJ) $(NVLISTOBJ) $(LIBS) -ggdb

${OBJDIR}/%.o: ${SRCDIR}/%.c
	$(CC) -c $(CFLAGS) -o $@ $<

${OBJDIR}/%.o: ${LIBNV}/%.c
	$(CC) -c $(CFLAGS) -o $@ $< -I${LIBNV} -I${LIBNVCONTRIB}

${OBJDIR}/%.o: ${LIBNVCONTRIB}/%.c
	$(CC) -c $(CFLAGS) -o $@ $< -I${LIBNV} -I${LIBNVCONTRIB}
