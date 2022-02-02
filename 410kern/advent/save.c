// HEAVILY edited for 15-410 by Zachary J. Snow (zsnow)
// removed all "encryption"; save to "files" in heap rather than on disk

/*	@(#)save.c	8.1 (Berkeley) 5/31/93				*/
/*	$NetBSD: save.c,v 1.14 2014/03/22 22:04:40 dholland Exp $	*/

/*-
 * Copyright (c) 1991, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * The game adventure was originally written in Fortran by Will Crowther
 * and Don Woods.  It was later translated to C and enhanced by Jim
 * Gillogly.  This code is derived from software contributed to Berkeley
 * by Jim Gillogly at The Rand Corporation.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

// #include <sys/types.h>
// #include <sys/time.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
// #include <err.h>
#include <assert.h>

#include "hdr.h"
#include "extern.h"

struct savefile {
	char *name;
	unsigned char *buf;
	size_t size;
	size_t pos;
	struct savefile *next;
};

static struct savefile *head = NULL;

////////////////////////////////////////////////////////////
// savefile

/*
 * Open a savefile.
 */
static struct savefile *
savefile_open(const char *name, bool forwrite)
{
	struct savefile *sf;
	for (sf = head; sf != NULL; sf = sf->next) {
		if (strcmp(name, sf->name) == 0)
			break;
	}

	if (sf == NULL && forwrite) {
		sf = malloc(sizeof(struct savefile));
		if (!sf)
			return NULL;
		sf->next = head;
		head = sf;
		sf->name = strdup(name);
		sf->buf = NULL;
		sf->size = 0;
		if (!sf->name) {
			free(sf);
			return NULL;
		}
	}

	if (sf != NULL)
		sf->pos = 0;

	return sf;
}

/*
 * Raw read.
 */
static int
savefile_rawread(struct savefile *sf, void *data, size_t len)
{
	assert(sf->pos + len <= sf->size);
	memcpy(data, sf->buf + sf->pos, len);
	sf->pos += len;
	return 0;
}

/*
 * Raw write.
 */
static int
savefile_rawwrite(struct savefile *sf, const void *data, size_t len)
{
	while (sf->pos + len > sf->size) {
		unsigned char *buf = realloc(sf->buf, (sf->size + 1) * 2);
		if (!buf)
			return 1;
		sf->buf = buf;
		sf->size += 1;
		sf->size *= 2;
	}
	memcpy(sf->buf + sf->pos, data, len);
	sf->pos += len;
	return 0;
}

/*
 * Close a savefile.
 */
static int
savefile_close(struct savefile *sf)
{
	int ret;
	ret = 0;
	return ret;
}

////////////////////////////////////////////////////////////
// save + restore

static int *const save_ints[] = {
	&abbnum,
	&attack,
	&blklin,
	&bonus,
	&chloc,
	&chloc2,
	&clock1,
	&clock2,
	&closed,
	&isclosing,
	&daltloc,
	&demo,
	&detail,
	&dflag,
	&dkill,
	&dtotal,
	&foobar,
	&gaveup,
	&holding,
	&iwest,
	&k,
	&k2,
	&knfloc,
	&kq,
	&latency,
	&limit,
	&lmwarn,
	&loc,
	&maxdie,
	&maxscore,
	&newloc,
	&numdie,
	&obj,
	&oldloc2,
	&oldloc,
	&game_panic,
	&saveday,
	&savet,
	&scoring,
	&spk,
	&stick,
	&tally,
	&tally2,
	&turns,
	&verb,
	&wasdark,
	&yea,
};
static const unsigned num_save_ints = __arraycount(save_ints);

#define INTARRAY(sym) { sym, __arraycount(sym) }

static const struct {
	int *ptr;
	unsigned num;
} save_intarrays[] = {
	INTARRAY(atloc),
	INTARRAY(dseen),
	INTARRAY(dloc),
	INTARRAY(odloc),
	INTARRAY(fixed),
	INTARRAY(hinted),
	INTARRAY(links),
	INTARRAY(place),
	INTARRAY(prop),
	INTARRAY(tk),
};
static const unsigned num_save_intarrays = __arraycount(save_intarrays);

#undef INTARRAY

#if 0
static const struct {
	void *ptr;
	size_t len;
} save_blobs[] = {
	{ &wd1, sizeof(wd1) },
	{ &wd2, sizeof(wd2) },
	{ &tkk, sizeof(tkk) },
};
static const unsigned num_save_blobs = __arraycount(save_blobs);
#endif

/*
 * Write out a save file. Returns nonzero on error.
 */
int
save(const char *outfile)
{
	struct savefile *sf;
	unsigned i, j, n;
	uint32_t val;

	sf = savefile_open(outfile, true);
	if (sf == NULL) {
		printf("Failed opening savefile!\n");
		return 1;
	}

	/*
	 * Integers
	 */
	for (i=0; i<num_save_ints; i++) {
		val = *(save_ints[i]);
		if (savefile_rawwrite(sf, &val, sizeof(val))) {
			savefile_close(sf);
			printf("Failed writing savefile!\n");
			return 1;
		}
	}

	/*
	 * Arrays of integers
	 */
	for (i=0; i<num_save_intarrays; i++) {
		n = save_intarrays[i].num;
		for (j=0; j<n; j++) {
			val = save_intarrays[i].ptr[j];
			if (savefile_rawwrite(sf, &val, sizeof(val))) {
				savefile_close(sf);
				printf("Failed writing savefile!\n");
				return 1;
			}
		}
	}

#if 0
	/*
	 * Blobs
	 */
	for (i=0; i<num_save_blobs; i++) {
		if (savefile_rawwrite(sf, save_blobs[i].ptr, save_blobs[i].len)) {
			savefile_close(sf);
			return 1;
		}
	}
#endif

	savefile_close(sf);
	return 0;
}

/*
 * Read in a save file. Returns nonzero on error.
 */
int
restore(const char *infile)
{
	struct savefile *sf;
	unsigned i, j, n;
	uint32_t val;

	sf = savefile_open(infile, false);
	if (sf == NULL) {
		printf("Failed opening savefile!\n");
		return 1;
	}

	/*
	 * Integers
	 */
	for (i=0; i<num_save_ints; i++) {
		if (savefile_rawread(sf, &val, sizeof(val))) {
			savefile_close(sf);
			printf("Failed reading savefile!\n");
			return 1;
		}
		*(save_ints[i]) = val;
	}

	/*
	 * Arrays of integers
	 */
	for (i=0; i<num_save_intarrays; i++) {
		n = save_intarrays[i].num;
		for (j=0; j<n; j++) {
			if (savefile_rawread(sf, &val, sizeof(val))) {
				savefile_close(sf);
				printf("Failed reading savefile!\n");
				return 1;
			}
			save_intarrays[i].ptr[j] = val;
		}
	}

#if 0
	/*
	 * Blobs
	 */
	for (i=0; i<num_save_blobs; i++) {
		if (savefile_rawread(sf, save_blobs[i].ptr, save_blobs[i].len)) {
			savefile_close(sf);
			return 1;
		}
	}
#endif

	savefile_close(sf);

	/* Load theoretically invalidates these */
	tkk = NULL;
	wd1 = NULL;
	wd2 = NULL;

	return 0;
}
