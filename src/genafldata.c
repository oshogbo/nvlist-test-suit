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

#include <sys/types.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <nv.h>

#include "test_common.h"

int
main(int argc, char **argv)
{
	nvlist_t *nvl;
	char *filename, *buf;
	int len, fd, ret, count;
	size_t size;

	ret = 1;
	nvl = NULL;
	buf = NULL;
	if (argc != 3) {
		fprintf(stderr, "Usage: %s [destination_dir] [elements_count]\n", argv[0]);
		return (1);
	}

	len = asprintf(&filename, "%s/nvlist.XXXXXXXX", argv[1]);
	if (len <= 0 || filename == NULL) {
		fprintf(stderr, "Unable to generate filename.\n");
		return (1);
	}
	fd = mkstemp(filename);
	free(filename);
	if (fd < 0) {
		fprintf(stderr, "Unable to create file.\n");
		goto out;
	}

	/* Don't do this at home. */
	count = atoi(argv[2]);
	nvl = generate_nvlist(count);
	if (nvl == NULL || nvlist_error(nvl) != 0) {
		fprintf(stderr, "Unable to generate test data.\n");
		goto out;
	}

	buf = nvlist_pack(nvl, &size);
	if (buf == NULL) {
		fprintf(stderr, "Unable to pack nvlist.\n");
		goto out;
	}
	if (write(fd, buf, size) != (ssize_t)size) {
		fprintf(stderr, "Unable to write data.\n");
		goto out;
	}

	ret = 0;
out:
	if (fd >= 0)
		close(fd);
	if (nvl != NULL)
		nvlist_destroy(nvl);
	free(buf);

	return (ret);
}

