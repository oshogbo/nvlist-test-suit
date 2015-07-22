#
# Copyright (c) 2015 Mariusz Zaborski <oshogbo@FreeBSD.org>
#

OBJ=$(addprefix ${OBJDIR}/, $(SRC:.c=.o))

all: $(TARGET)

${TARGET}: ${BINDIR}/${TARGET}

${BINDIR}/$(TARGET): $(OBJ)
	$(CC) -o $@ $(OBJ) $(LIBS)

${OBJDIR}/%.o: ${SRCDIR}/%.c
	$(CC) -c $(CPPFLAGS) -o $@ $<
