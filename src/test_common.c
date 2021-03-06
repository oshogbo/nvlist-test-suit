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

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <uuid.h>
#include <math.h>

#define MAX_ELEMENTS_ARRAY	16

enum nvtype {
	NVLIST_BOOL,
	NVLIST_NUMBER,
	NVLIST_STRING,
	NVLIST_NVLIST,
	NVLIST_UP,
	NVLIST_NULL,
	NVLIST_BINARY,
#ifdef ARRAY_SUPPORT
	NVLIST_BOOL_ARRAY,
	NVLIST_NUMBER_ARRAY,
	NVLIST_STRING_ARRAY,
	NVLIST_NVLIST_ARRAY,
#endif	/* ARRAY_SUPPORT */
	NVLIST_LAST
};

#ifdef ARRAY_SUPPORT
static size_t
array_elements_count(int maxcount)
{
	int r;

	if (maxcount < 0) {
		abort();
	}

	r = rand() % (MAX_ELEMENTS_ARRAY - 1) + 1;

	if (r > maxcount)
		return (maxcount);
	return (r);
}
#endif	/* ARRAY_SUPPORT */

static bool
generate_string(char **buf)
{
	uuid_t store;
	uint32_t status;

	if (uuidgen(&store, 1) != 0)
		return (false);
	uuid_to_string(&store, buf, &status);
	if (status != uuid_s_ok)
		return (false);

	return (true);
}

nvlist_t *
generate_nvlist(unsigned int count)
{
	nvlist_t *nvl, *pvl;
	unsigned int i;
#ifdef ARRAY_SUPPORT
	unsigned int j;
#endif	/* ARRAY_SUPPORT */
	enum nvtype t;
	char *name;

	srand(time(NULL));

	pvl = nvl = nvlist_create(0);
	for (i = 0; i < count; i++) {
		if (!generate_string(&name))
			goto err;

		t = rand() % NVLIST_LAST;
		switch (t) {
		case NVLIST_BOOL:
			nvlist_add_bool(pvl, name, rand() % 2);
			break;
		case NVLIST_NUMBER:
			nvlist_add_bool(pvl, name, rand());
			break;
		case NVLIST_STRING:
		    {
			char *buf;

			if (!generate_string(&buf))
				goto err;
			buf[rand() % (strlen(buf) + 1)] = 0x00;

			nvlist_add_string(pvl, name, buf);
			free(buf);
			break;
		    }
		case NVLIST_NVLIST:
		    {
			nvlist_t *new;

			new = nvlist_create(0);
			if (new == NULL)
				goto err;
			nvlist_move_nvlist(pvl, name, new);
			pvl = new;

			break;
		    }
		case NVLIST_UP:
		    {
			nvlist_t *old;

			i--;
			/* This also is bad Mkay? */
			old = __DECONST(nvlist_t *,
			    nvlist_get_parent(pvl, NULL));
			if (old != NULL)
				pvl = old;
			break;
		    }
		case NVLIST_NULL:
			nvlist_add_null(pvl, name);
			break;
		case NVLIST_BINARY:
		    {
			char *buf;

			if (!generate_string(&buf))
				goto err;
			nvlist_add_binary(pvl, name, buf,
			    (rand() % strlen(buf)) + 1);
			free(buf);
			break;
		    }
#ifdef ARRAY_SUPPORT
		case NVLIST_BOOL_ARRAY:
		    {
			bool *buf;
			size_t size;

			if (i == count - 1) {
				i--;
				break;
			}

			size = array_elements_count(count - i);
			buf = malloc(size);
			if (buf == NULL)
				goto err;
			for (j = 0; j < size; j++)
				buf[j] = rand() % 2;
			nvlist_move_bool_array(pvl, name, buf, size);
			i += size - 1;
			break;
		    }
		case NVLIST_NUMBER_ARRAY:
		    {
			uint64_t *buf;
			size_t size;

			if (i == count - 1) {
				i--;
				break;
			}

			size = array_elements_count(count - i);
			buf = malloc(size * sizeof(*buf));
			if (buf == NULL)
				goto err;
			for (j = 0; j < size; j++)
				buf[j] = rand();
			nvlist_move_number_array(pvl, name, buf, size);
			i += size - 1;
			break;
		    }
		case NVLIST_STRING_ARRAY:
		    {
			char **buf;
			size_t size;

			if (i == count - 1) {
				i--;
				break;
			}

			size = array_elements_count(count - i);
			buf = malloc(size * sizeof(*buf));
			if (buf == NULL)
				goto err;
			for (j = 0; j < size; j++) {
				if (!generate_string(&buf[j])) {
					size_t k;

					for (k = 0; k < j; k++)
						free(buf[k]);
					free(buf);
					goto err;
				}
			}
			nvlist_move_string_array(pvl, name, buf, size);
			i += size - 1;
			break;
		    }
		case NVLIST_NVLIST_ARRAY:
		    {
			nvlist_t **buf;
			size_t size, childsize, done;

			if (i == count - 1) {
				i--;
				break;
			}

			done = childsize = 0;
			size = array_elements_count(count - i);
			if (size == 0)
				abort();
			buf = malloc(size * sizeof(*buf));
			if (buf == NULL)
				goto err;
			for (j = 0; j < size; j++) {
				childsize = array_elements_count(count - i -
				    done - size);
				done += childsize;
				buf[j] = generate_nvlist(childsize);
				if (nvlist_error(buf[j]) != 0)
					abort();
				if (buf[j] == NULL) {
					size_t k;

					for (k = 0; k < j; k++)
						nvlist_destroy(buf[k]);
					free(buf);
					goto err;
				}
			}
			nvlist_move_nvlist_array(pvl, name, buf, size);
			i += size - 1 + done;
			break;
		    }
#endif	/* ARRAY_SUPPORT */
		default:
			goto err;
		}
		free(name);
		name = NULL;
	}
	if (i != count)
		abort();

	if (nvlist_error(nvl) != 0)
		goto err;

	return (nvl);

err:
	free(name);
	nvlist_destroy(nvl);
	return (NULL);
}

