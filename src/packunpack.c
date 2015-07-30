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

	buf = nvlist_pack(nvl, &size);
	if (buf == NULL) {
		fprintf(stderr, "Unable to pack nvlist.\n");
		abort();
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
				abort();
			}

			if (type != newtype) {
				fprintf(stderr, "Types are diffrent.\n");
				abort();
			}

			switch (type) {
			case NV_TYPE_BOOL:
				if (nvlist_get_bool(pvl, name) !=
				    nvlist_get_bool(newpvl, newname)) {
					fprintf(stderr, "Bool check failed.\n");
					abort();
				}
				break;
			case NV_TYPE_STRING:
				if (strcmp(nvlist_get_string(pvl, name),
				    nvlist_get_string(newpvl, newname)) != 0) {
					fprintf(stderr, "String check failed.\n");
					abort();
				}
				break;
			case NV_TYPE_NUMBER:
				if (nvlist_get_number(pvl, name) !=
				    nvlist_get_number(newpvl, newname)) {
					fprintf(stderr, "Number check failed.\n");
					abort();
				}
				break;
			case NV_TYPE_NVLIST:
				pvl = nvlist_get_nvlist(pvl, name);
				newpvl = nvlist_get_nvlist(newpvl, newname);
				if (pvl == NULL || newpvl == NULL) {
					fprintf(stderr, "Nvlist check failed.\n");
					abort();
				}
				cookie = NULL;
				newcookie = NULL;
				break;
			case NV_TYPE_NULL:
				if (!nvlist_exists_null(pvl, name) ||
				    !nvlist_exists_null(newpvl, newname)) {
					fprintf(stderr, "NULL check failed.\n");
					abort();
				}
				break;
			case NV_TYPE_BINARY:
			    {
				size_t spvl, snewpvl;
				const void *buf, *newbuf;

				buf = nvlist_get_binary(pvl, name, &spvl);
				newbuf = nvlist_get_binary(newpvl, newname,
				    &snewpvl);
				if (spvl != snewpvl ||
				    buf == NULL || newbuf == NULL ||
				    memcmp(buf, newbuf, spvl) != 0) {
					fprintf(stderr, "Binary check failed.\n");
					abort();
				}
				break;
			    }
#ifdef ARRAY_SUPPORT
			case NV_TYPE_BOOL_ARRAY:
			    {
				size_t size, newsize, i;
				const bool *val, *newval;

				val = nvlist_get_bool_array(pvl, name, &size);
				newval = nvlist_get_bool_array(newpvl, newname,
				    &newsize);
				if (size != newsize) {
					fprintf(stderr, "Bool array check failed.");
					abort();
				}
				for (i = 0; i < size; i++) {
					if (val[i] != newval[i]) {
						fprintf(stderr, "Bool array check failed.");
						abort();
					}
				}
				break;
			    }
			case NV_TYPE_NUMBER_ARRAY:
			    {
				size_t size, newsize, i;
				const uint64_t *val, *newval;

				val = nvlist_get_number_array(pvl, name, &size);
				newval = nvlist_get_number_array(newpvl,
				    newname, &newsize);
				if (size != newsize) {
					fprintf(stderr, "Number array check failed.");
					abort();
				}
				for (i = 0; i < size; i++) {
					if (val[i] != newval[i]) {
						fprintf(stderr, "Number array check failed.");
						abort();
					}
				}
				break;
			    }
			case NV_TYPE_STRING_ARRAY:
			    {
				size_t size, newsize, i;
				const char * const *val;
				const char * const *newval;

				val = nvlist_get_string_array(pvl, name, &size);
				newval = nvlist_get_string_array(newpvl,
				    newname, &newsize);
				if (size != newsize) {
					fprintf(stderr, "String array check failed.");
					abort();
				}
				for (i = 0; i < size; i++) {
					if (strcmp(val[i], newval[i]) != 0) {
						fprintf(stderr, "String number check failed.");
						abort();
					}
				}
				break;
			    }
			case NV_TYPE_NVLIST_ARRAY:
			    {
				size_t size, newsize;
				const nvlist_t * const *val;
				const nvlist_t * const *newval;

				val = nvlist_get_nvlist_array(pvl, name, &size);
				newval = nvlist_get_nvlist_array(newpvl,
				    newname, &newsize);
				if (size != newsize) {
					fprintf(stderr, "Nvlist array check failed.");
					abort();
				}
				if (pvl == NULL || newpvl == NULL) {
					fprintf(stderr, "Nvlist array check failed.\n");
					abort();
				}
				pvl = val[0];
				newpvl = newval[0];
				cookie = NULL;
				newcookie = NULL;
				break;
			    }
#endif	/* ARRAY_SUPPORT */
			default:
				fprintf(stderr, "Unknown type.\n");
			}
		}
		if (name != NULL && newname != NULL) {
			fprintf(stderr, "Unapacked nvlist is diffrent then orginal.\n");
			abort();
		}
#ifdef ARRAY_SUPPORT
	} while ((pvl = nvlist_get_pararr(pvl, &cookie)) != NULL &&
		(newpvl = nvlist_get_pararr(newpvl, &newcookie)) != NULL);
#else
	} while ((pvl = nvlist_get_parent(pvl, &cookie)) != NULL &&
		(newpvl = nvlist_get_parent(newpvl, &newcookie)) != NULL);
#endif	/* ARRAY_SUPPORT */

	if (pvl != NULL && newpvl != NULL) {
		fprintf(stderr, "Unapacked nvlist is diffrent then orginal.\n");
		abort();
	}

	newbuf = nvlist_pack(newnvl, &newsize);
	if (newsize != size) {
		fprintf(stderr, "Size is not deterministic.\n");
		abort();
	}

	if (memcmp(newbuf, buf, size) != 0) {
		fprintf(stderr, "Pack is not deterministic.\n");
		abort();
	}

	nvlist_destroy(nvl);
	free(buf);

	return (0);
}

