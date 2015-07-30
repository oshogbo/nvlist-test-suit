#
# Copyright (c) 2015 Mariusz Zaborski <oshogbo@FreeBSD.org>
#

include make/nvlistfiles.mk

OBJ=$(addprefix ${OBJDIR}/, $(SRC:.c=.o))

all: $(TARGET)

${TARGET}: ${BINDIR}/${TARGET}

${BINDIR}/$(TARGET): $(NVLISTOBJ) $(OBJ)
	$(CC) -o $@ $(OBJ) $(NVLISTOBJ) $(LIBS) -ggdb

${OBJDIR}/%.o: ${SRCDIR}/%.c
	$(CC) -c $(CFLAGS) -o $@ $<
