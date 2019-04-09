## Process this file with automake to produce Makefile.in
## $Id: Makefile.am,v 1.4 2002/01/08 10:38:22 flux Exp $

INCLUDES = -I$(top_srcdir)

bin_PROGRAMS = clminfo clmdist clmmeet clmconf
clminfo_SOURCES = clminfo.c
clmdist_SOURCES = clmdist.c
clmmeet_SOURCES = clmmeet.c
clmconf_SOURCES = clmconf.c

LDADD = ../intalg/la.o ../intalg/libintalg.a ../mcl/libmcl.a ../nonema/libnonema.a ../util/libutil.a

