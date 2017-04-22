# String Suggest
## Command line utility to print auto-complete suggestions

This small and fast C code reads a list of possible newline separated phrases
from a file, and prints the ones containing a given substring. Useful for the
core of an autocomplete feature.

There are two versions of the code that have identical interfaces, but have
different internals.

### `strsug.c`
The portable variant that uses the C standard library to read and write.

### `sysstrsug.c`
The Unix-specific variant that uses system calls to read and write.

### Kickstart
```
git clone https://github.com/ankitpati/strsug.git
cd strsug
make

src/strsug    test/strlst.txt tech
src/sysstrsug test/strlst.txt tech
```
