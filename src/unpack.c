/*-
 * Copyright (c) 2015 Mariusz Zaborski <oshogbo@FreeBSD.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/nv.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int
main(int argc, char **argv)
{
	nvlist_t *nvl;
	char *buf;
	int ret;
	size_t size;
	FILE *pfile;
	bool print;

	ret = 1;
	nvl = NULL;
	buf = NULL;
	pfile = NULL;
	print = false;
	if (argc != 2 && argc != 3) {
		fprintf(stderr, "Usage: %s [filename] [print]\n", argv[0]);
		return (1);
	}
	if (argc == 3 && strcmp(argv[2], "print"))
		print = true;

	pfile = fopen(argv[1], "r");
	if (pfile == NULL) {
		fprintf(stderr, "Unable to open file %s.\n", argv[1]);
		return (1);
	}

	fseek(pfile, 0, SEEK_END);
	size = ftell(pfile);
	fseek(pfile, 0, SEEK_SET);

	buf = malloc(size);
	if (buf == NULL) {
		fprintf(stderr, "Unable to read file %s.\n", argv[1]);
		goto out;
	}

	if (fread(buf, 1, size, pfile) != size) {
		fprintf(stderr, "Unable to read full file %s.\n", argv[1]);
		goto out;
	}

	nvl = nvlist_unpack(buf, size, 0);
	if (nvl == NULL || nvlist_error(nvl) != 0)
		printf("Failed to unpack.\n");
	if (print)
		nvlist_fdump(nvl, stdout);

	ret = 0;
out:
	if (pfile != NULL)
		fclose(pfile);
	if (nvl != NULL)
		nvlist_destroy(nvl);
	free(buf);

	return (ret);
}

