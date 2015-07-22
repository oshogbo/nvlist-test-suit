#!/bin/sh
#
# Copyright (c) 2015 Mariusz Zaborski <oshogbo@FreeBSD.org>
#

PROG=bin/packunpack

if [ \! -x "${PROG}" ]; then
	echo "First use gmake to compile tools." >&2
	exit 1
fi

if [ $# -ne 3 ]; then
	echo "Usage: $0 [min-nvlist-size] [max-nvlist-size] [count]" >&2
	exit 1
fi

i=0
while [ ${i} -ne ${3} ]; do
	nr=$(jot -r 1 "${1}" "${2}")
	echo "${i} of ${3}, number of elements ${nr}..."

	./${PROG} ${nr}
	if [ $? -ne 0 ]; then
		echo "Error detected."
		exit 1
	fi

	i=$((i+1))
done

