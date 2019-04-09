## Process this file with automake to produce Makefile.in
## $Id: Makefile.am,v 1.6 2002/02/13 16:14:31 flux Exp $

INCLUDES = -I$(top_srcdir)

noinst_PROGRAMS = zoem
zoem_SOURCES = bind.c constant.c counter.c curly.c digest.c entry.c env.c\
   file.c filter.c key.c ops.c parse.c read.c ref.c segment.c util.c zoem.c 

LDADD = ../util/libutil.a

EXTRA_DIST = bind.h constant.h counter.h curly.h digest.h entry.h env.h\
   file.h filter.h iface.h key.h ops.h parse.h read.h ref.h segment.h util.h 
