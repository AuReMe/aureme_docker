## Process this file with automake to produce Makefile.in
## $Id: Makefile.am,v 1.4 2001/10/14 20:06:57 joostvb Exp $

## bin_PROGRAMS = mcl
## mcl_SOURCES = mcl.c mcl.h
## mcl_LDADD = @LIBOBJS@

INCLUDES = -I$(top_srcdir)

noinst_LIBRARIES = libmcl.a

LDADD = libmcl.a ../intalg/libintalg.a ../nonema/libnonema.a ../util/libutil.a

libmcl_a_SOURCES = clm.c compose.c dpsd.c interpret.c mcl.c params.c

EXTRA_DIST = clm.h compose.h dpsd.h interpret.h mcl.h params.h

