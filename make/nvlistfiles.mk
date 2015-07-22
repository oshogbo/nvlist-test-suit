NVLISTSRC=	dnvlist.c
NVLISTSRC+=	msgio.c
NVLISTSRC+=	nvlist.c
NVLISTSRC+=	nvpair.c

NVLISTOBJ=	$(addprefix ${OBJDIR}/, $(NVLISTSRC:.c=.o))

