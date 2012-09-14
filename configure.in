AC_INIT([tunneler], [1.1.1], [tvkalvas@jyu.fi])
AM_CONFIG_HEADER(src/config.h)
AM_INIT_AUTOMAKE(tunneler,1.1.1)

dnl Check for various tools
AC_PROG_CC
AC_PROG_INSTALL

dnl Check for SDL 1.2.0+
SDL_VERSION=1.2.0
AM_PATH_SDL($SDL_VERSION,:,AC_MSG_ERROR([*** SDL >= $SDL_VERSION not found!]))

dnl Get compile and linking flags
CFLAGS="$CFLAGS $SDL_CFLAGS"
LIBS="$LIBS $SDL_LIBS"

dnl Generate Makefile
AC_OUTPUT(Makefile	\
	  src/Makefile	\
	 )
