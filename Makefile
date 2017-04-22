# Makefile
# Makefile for Command line utility to print auto-complete suggestions
#
# Copyright 2014 Ankit Pati <ankitpati@gmail.com>
#
# This library is free software;  you can redistribute it and/or modify it under
# the  terms of the GNU Lesser General Public License  as published by the  Free
# Software Foundation;  either version 3 of the License, or (at your option) any
# later version.
#
# This  library is distributed in the hope that it will be useful,  but  WITHOUT
# ANY WARRANTY;  without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License  for more
# details.
#
# You  should  have received a copy of the GNU  Lesser  General  Public  License
# along with this program; if not, see <http://www.gnu.org/licenses/>.

CC=gcc
CFLAGS=-Wall -Wextra

SOURCES=src/strsug.c \
		src/sysstrsug.c
EXECUTABLES=$(SOURCES:.c=)

all: $(SOURCES) $(EXECUTABLES) done

%: %.c
	$(CC) $^ $(CFLAGS) -o $@

clean:
	@echo Cleaning...
	find . -type f -name "*.o"   -delete
	find . -type f -name "*.gch" -delete
	strip $(EXECUTABLES)

done:
	@echo Done!

# end of Makefile
