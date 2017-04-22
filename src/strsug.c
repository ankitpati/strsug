/* strsug.c */
/* Command line utility to print auto-complete suggestions
 *
 * Copyright 2014 Ankit Pati <ankitpati@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation;  either version 3 of the License, or (at your option)  any  later
 * version.
 *
 * This  program is distributed in the hope that it will be useful, but  WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR  A  PARTICULAR  PURPOSE.  See the GNU General  Public  License  for  more
 * details.
 *
 * You should have received a copy of the GNU General Public License along  with
 * this program; if not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>

#define BUFFER 4096

#define ERR_USAGE 1
#define ERR_OPEN  2
#define ERR_ALLOC 3
#define ERR_READ  4

void error(int code)
{
    switch (code) {
    case ERR_USAGE:
        fprintf(stderr,
                "Usage: strsug [input pathname] [output pathname] <needle>\n");
        break;

    case ERR_OPEN:
        fprintf(stderr, "File Open Error!\n");
        break;

    case ERR_ALLOC:
        fprintf(stderr, "Insufficient Memory!\n");
        break;

    case ERR_READ:
        fprintf(stderr, "File Read Error!\n");
        break;

    default:
        fprintf(stderr, "Unknown Error!\n");
        break;
    }

    exit(code);
}

char *strinbuf(char *hay, char *ned)
{
    size_t pos, nlen, htest, i;

    for (nlen = 0; ned[nlen]; ++nlen);
    for (htest = 0; hay[htest] != '\n' && htest < nlen; ++htest);

    if (htest != nlen)
        return NULL;

    for (pos = 0; hay[pos + nlen - 1] != '\n'; ++pos) {
        for (i = 0; i < nlen && (hay + pos)[i] == ned[i]; ++i);
        if (i == nlen)
            return hay + pos;
    }

    return NULL;
}

unsigned prmatch(FILE * fout, char *key, char *buf, size_t siz)
{
    size_t i, mlen;
    unsigned count;

    for (i = count = 0; i < siz; ++i) {
        if (buf[i] == '\n')
            continue;

        if (strinbuf(buf + i, key)) {
            for (mlen = 0; (buf + i)[mlen] != '\n'; ++mlen);
            fwrite(buf + i, sizeof(char), mlen, fout);
            fputc('\n', fout);
            ++count;
        }

        while (buf[i] != '\n')
            ++i;
    }
    return count;
}

size_t trimend(char *buf, size_t siz)
{
    for (--siz; buf[siz] != '\n' && siz + 1; --siz);
    return siz;
}

size_t readfile(FILE * fin, char *buf, size_t siz, size_t preserve)
{
    size_t i, j;

    if (preserve == siz)
        preserve = 0;       /* drop excessively long phrases */

    for (i = 0, j = siz - preserve; j < siz; ++i, ++j)
        buf[i] = buf[j];

    return fread(buf + i, sizeof(char), siz - preserve, fin);
}

int main(int argc, char **argv)
{
    size_t red, preserve, trim;
    char *buf, *ned;
    FILE *fin, *fout;

    switch (argc) {
    case 2:
        fin  = stdin;
        fout = stdout;
        ned  = argv[1];
        break;

    case 3:
        fin  = fopen(argv[1], "r");
        fout = stdout;
        if (!fin)
            error(ERR_OPEN);
        ned = argv[2];
        break;

    case 4:
        fin = fopen(argv[1], "r");
        if (!fin)
            error(ERR_OPEN);
        fout = fopen(argv[2], "w");
        if (!fout)
            error(ERR_OPEN);
        ned = argv[3];
        break;

    default:
        error(ERR_USAGE);
        break;
    }

    buf = malloc(BUFFER);

    if (!buf)
        error(ERR_ALLOC);

    preserve = 0;

    while ((red = readfile(fin, buf, BUFFER, preserve))) {
        if (ferror(fin))
            error(ERR_READ);

        trim = trimend(buf, red + preserve);
        preserve = red + preserve - trim;
        prmatch(fout, ned, buf, trim);
    }

    if (fin != stdin)
        fclose(fin);

    if (fout != stdout)
        fclose(fout);

    free(buf);
    return 0;
}
/* end of strsug.cpp */
