#!/bin/sh
#
# Copyright (c) 2015 Mariusz Zaborski <oshogbo@FreeBSD.org>
#

PROG=bin/genafldata
DESTDIR=testdata

if [ \! -x "${PROG}" ]; then
	echo "First use gmake to compile tools." >&2
	exit 1
fi

if [ $# -eq 1 -a "${1}" = "std" ]; then
	echo "Generating file with 0 elements..."
	./${PROG} ${DESTDIR} 0 || exit 1
	echo "Generating file with 1 elements..."
	./${PROG} ${DESTDIR} 1 || exit 1

	s=1
	while [ ${s} -lt 10000 ]; do
		e=$((s*10))
		echo "Generating 10 files with [${s},${e}] elements..."
		$0 ${s} ${e} 10 || exit 1
		s=${e}
	done

	exit 0
elif [ $# -ne 3 ]; then
	echo "Usage:" >&2
	echo "       $0 [std]" >&2
	echo "       $0 [min-nvlist-size] [max-nvlist-size] [count]" >&2
	exit 1
fi

i=0
while [ ${i} -ne ${3} ]; do
	nr=$(jot -r 1 "${1}" "${2}")
	echo "Generating file ${i} of ${3}, number of elements ${nr}..."

	./${PROG} ${DESTDIR} ${nr}
	if [ $? -ne 0 ]; then
		echo "Unable to generate file."
		exit 1
	fi

	i=$((i+1))
done

