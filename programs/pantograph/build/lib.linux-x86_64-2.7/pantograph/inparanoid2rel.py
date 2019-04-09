#!/usr/bin/env python
# encoding: utf-8
"""
inparanoid2rel.py

Parses inparanoid output and translate it to a simple .rel file

Created by Nicolas Loira on 2010-06-15.
Copyright (c) 2010 LaBRI, Univ.Bordeaux I, France. All rights reserved.
"""

import sys
import os
import getopt

from xml.etree.ElementTree import *
from collections import defaultdict

import re

VERSION="0.1.2"


def main():
	output = None

	# parse parameters
	corrFile=None
	opts, args = getopt.getopt(sys.argv[1:], "ho:vs:t:", ["help", "output=", "corr="])
	for o, v in opts:
		if o == "--corr": corrFile=v
		if o == "-v":
			print VERSION
			return 0
		if o in ("-o","--output"):
			output = v

	
	# parse correlation file, if it exists
	rename=dict()
	if corrFile:
		for line in open(corrFile):
			elems=line[:-1].split()
			rename[elems[0]]=elems[1]
	
	# output to file?
	if output is not None:
		outfd = open(output,"w")
	else:
		outfd = sys.stdout
	
	# infile is first non-named parameter
	
	infile = args[0]

	species = None

	# try xml parser first
	if infile.lower().find("xml")>-1:
	    species, groups = parseInparanoidXML(infile)
	else:
		groups=parseInparanoidTable(infile)

	order=None
	iFirst=None
	iSecond=None
	
	saceLocus=re.compile("Y[A-Z][RL]\d{3}[WC]")
	
	for g in groups:
		if order is None:
			# which one is sace?
			if saceLocus.match(g[0][0]):
				iFirst, iSecond= 1,0
			else:
				iFirst, iSecond= 0,1
			
		# first target, then sace
		# ugly, I know
		gFirst=[rename.get(x,x) for x in g[iFirst]]
		gSecond=[rename.get(x,x) for x in g[iSecond]]		
		line = ",".join(gFirst)+"\t"+",".join(gSecond)+"\n"
		outfd.write(line)
		
		
def parseInparanoidTable(tableFile):
	
	geneMap=[]
	
	for line in open(tableFile):
		if line.startswith("OrtoID"): continue
		
		elems=line[:-1].split("\t")
		
		# elems[2]:source elems[3]:target
		
		#sourceOrg=elems[2][0:3]
		#targetOrg=elems[3][0:3]

		# take gene names from both lists
		# skip confidence values between genes ([::2]), drop prefix ([4:])
		#sourceGenes= [ x[4:] for x in elems[2].split()[::2] ]
		#targetGenes= [ x[4:] for x in elems[3].split()[::2] ]
		sourceGenes= [ x for x in elems[2].split()[::2] ]
		targetGenes= [ x for x in elems[3].split()[::2] ]

		geneMap.append( (sourceGenes, targetGenes) )

	return geneMap



def  parseInparanoidXML(xmlfile):
	"""parse an xml file from Inparanoid website. Returns the mapping between two organism"""
	
	ixml = parse(open(xmlfile))
	
	root = ixml.getroot()
	
	# remember all genes
	species=root.findall("species")
	
	geneLocus=dict()
	genes = defaultdict(list)
	gene2specie=dict()
	
	
	for s in species:
		specieid=s.attrib["id"]
		# specieid=s.attrib["NCBITaxId"]
		
		for gene in s.findall("database/genes/gene"):
			gid, protId = gene.attrib["id"], gene.attrib["protId"]
			genes[specieid].append(gid)
			geneLocus[gid]=protId
			gene2specie[gid]=specieid

	# remember ortholog groups
	species = genes.keys()
	clusters = root.findall("clusters/cluster")
	geneMap=[]
	
	
	for c in clusters:
		inCluster = defaultdict(list)
		for g in c.findall("geneRef"):
			gid = g.attrib["id"]
			org=gene2specie[gid]
			gName = geneLocus[gid]
			inCluster[org].append(gName)
			
		geneMap.append( [ inCluster[org] for org in species] )
		
	return species, geneMap

if __name__ == '__main__':
    main()

