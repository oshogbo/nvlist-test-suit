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

#include <stdlib.h>
#include <string.h>

#include "test_common.h"

int
main(int argc, char **argv)
{
	nvlist_t *nvl, *newnvl;
	const nvlist_t *pvl, *newpvl;
	unsigned int count;
	char *buf, *newbuf;
	size_t size, newsize;
	void *cookie, *newcookie;
	const char *name, *newname;
	int type, newtype;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s [elements_count]\n", argv[0]);
		return (1);
	}

	/* Don't do this at home. */
	count = atoi(argv[1]);
	nvl = generate_nvlist(count);
	if (nvl == NULL || nvlist_error(nvl) != 0) {
		fprintf(stderr, "Unable to generate test data.\n");
		return (1);
	}

#if 0
	nvlist_fdump(nvl, stdout);
#endif

	buf = nvlist_pack(nvl, &size);
	if (buf == NULL) {
		fprintf(stderr, "Unable to pack nvlist.\n");
		return (1);
	}
	newnvl = nvlist_unpack(buf, size, 0);

	pvl = nvl;
	newpvl = newnvl;
	cookie = newcookie = NULL;
	do {
		while ((name = nvlist_next(pvl, &type, &cookie)) != NULL &&
		    (newname = nvlist_next(newpvl, &newtype,
		    &newcookie)) != NULL) {
			if (strcmp(name, newname) != 0) {
				fprintf(stderr, "Names are diffrent.\n");
				return (1);
			}

			if (type != newtype) {
				fprintf(stderr, "Types are diffrent.\n");
				return (1);
			}

			switch (type) {
			case NV_TYPE_BOOL:
				if (nvlist_get_bool(pvl, name) !=
				    nvlist_get_bool(newpvl, newname)) {
					fprintf(stderr, "Bool check failed.\n");
					return (1);
				}
				break;
			case NV_TYPE_STRING:
				if (strcmp(nvlist_get_string(pvl, name),
				    nvlist_get_string(newpvl, newname)) != 0) {
					fprintf(stderr, "String check failed.\n");
					return (1);
				}
				break;
			case NV_TYPE_NUMBER:
				if (nvlist_get_number(pvl, name) !=
				    nvlist_get_number(newpvl, newname)) {
					fprintf(stderr, "Number check failed.\n");
					return (1);
				}
				break;
			case NV_TYPE_NVLIST:
				pvl = nvlist_get_nvlist(pvl, name);
				newpvl = nvlist_get_nvlist(newpvl, newname);
				if (pvl == NULL || newpvl == NULL) {
					fprintf(stderr, "Nvlist check failed.\n");
					return (1);
				}
				cookie = NULL;
				newcookie = NULL;
				break;
			default:
				fprintf(stderr, "Unknown type.\n");
			}
		}
		if (name != NULL && newname != NULL) {
			fprintf(stderr, "Unapacked nvlist is diffrent then orginal.\n");
			return (1);
		}
	} while ((pvl = nvlist_get_parent(pvl, &cookie)) != NULL &&
		(newpvl = nvlist_get_parent(newpvl, &newcookie)) != NULL);

	if (pvl != NULL && newpvl != NULL) {
		fprintf(stderr, "Unapacked nvlist is diffrent then orginal.\n");
		return (1);
	}

	newbuf = nvlist_pack(newnvl, &newsize);
	if (newsize != size) {
		fprintf(stderr, "Size is not deterministic.\n");
		return (1);
	}

	if (memcmp(newbuf, buf, size) != 0) {
		fprintf(stderr, "Pack is not deterministic.\n");
		return (1);
	}

	nvlist_destroy(nvl);
	free(buf);

	return (0);
}

