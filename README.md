# minishell

## Build requirements
- GNU readline headers and library
- termcap/termcap-compatible (ncurses) library
- POSIX environment with `make` and `cc`

## Building with system packages
When `libreadline-dev` (or the equivalent for your distribution) is available,
just run:

```
make
```

The `Makefile` always enables the real readline backend, so the build fails
early if the library is missing.

## Building without sudo
If system-wide installation is not possible, bundle readline locally:

1. Download the official tarball (`readline-X.Y.tar.gz`) and extract it under
   `vendor/` (e.g. `vendor/readline-src`).
2. Configure and install into a writable prefix:
   ```
   ./configure --prefix=/workspace/vendor/readline --disable-shared
   make
   make install
   ```
3. Build minishell with that prefix:
   ```
   make READLINE_PREFIX=/workspace/vendor/readline
   ```

You can further customize include/library flags if needed:

```
make READLINE_INC_FLAGS="-I/path/include" \
     READLINE_LIB_FLAGS="-L/path/lib" \
     READLINE_LIBS="-lreadline -lncurses"
```