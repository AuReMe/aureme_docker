## Process this file with automake to produce Makefile.in
## $Id: Makefile.am,v 1.14 2002/02/04 12:24:17 joostvb Exp $

## stuff in shtest should get distributed but not installed
SUBDIRS = intalg mcl nonema util shcl shmcl shmx shmcx shtest zoem doc graphs

VERSION: NEWS
	date +%y-%j > VERSION

VERSION = @VERSION@

EXTRA_DIST = \
   LICENSE \
   VERSION \
   bootstrap
