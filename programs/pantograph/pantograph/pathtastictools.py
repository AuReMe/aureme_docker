#!/usr/bin/env python2.7
# encoding: utf-8
"""
untitled.py

Created by Nicolas Loira on 2010-12-18.
Copyright (c) 2010 LaBRI, Univ.Bordeaux I, France. All rights reserved.
"""

import sys
import os
# from xml.etree.ElementTree import *
import xml.etree.ElementTree as ET
from collections import defaultdict
# import re
import zipfile
import logic

PTlogFD = sys.stderr

def log(m):
	PTlogFD.write(m)
	PTlogFD.write('\n')

def logRewrite(status, ga, newFormula=''):
	log("\t".join((status, ga.formula, newFormula, ga.rid, ga.getReactionName())))
# 	log( "\t".join((status, ga.formula)))


class SBMLmodel(object):
	def __init__(self, filename=None, logFD=None):

		global PTlogFD

		if logFD is not None:
			PTlogFD = logFD

		# Setup ET to understand that SBML is the default namespace
		# ET.register_namespace('', "http://www.sbml.org/sbml/level2/version4")
		# ET.register_namespace('', "http://www.sbml.org/sbml/level2/version1")


		if filename:
			self.parseXML(filename)

		self.toRemove = None
		self.lockedReactions = []



	def parseXML(self, modelFile):
		assert modelFile

		if modelFile == "-":
			modelfd = sys.stdin
		else:
			modelfd = self.openModel(modelFile)

		# parse file
		result = ET.parse(modelfd)
		root = result.getroot()
		self.xmlTree = result
		self.root = root
		self.URI, self.tag = root.tag[1:].split("}", 1)
		assert self.URI, "URI was not set correctly."

		# shortcut for list of reaction nodes
		self.reacNodes = root.findall("*/{%s}listOfReactions/{%s}reaction" % (self.URI, self.URI))

		# map of all parents (useful for removing nodes)
		self.parentMap = dict((c, p) for p in root.getiterator() for c in p)

	def openModel(self, modelFile):
		assert modelFile is not None
		modelfd = None

		if zipfile.is_zipfile(modelFile):
			zp = zipfile.ZipFile(modelFile, 'r')
			firstsbml = next((name for name in zp.namelist() if name.endswith("xml") or name.endswith("sbml")), None)
			if firstsbml == None:
				log("No .xml/.sbml file found in "+modelFile)
				sys.exit(2)
			modelfd = zp.open(firstsbml, 'r')
		else:
			modelfd = open(modelFile)

		return modelfd


	def write(self, outFD):
		#self.xmlTree.write(outFD)

		# COBRA doesn't like the extra namespaces added by ElementTree,
		# so we'll parse the output
		assert self.root is not None
		assert outFD is not None

		xmlstr = ET.tostring(self.root)
		cleanxml = xmlstr
		#cleanxml=re.sub("<ns\d+:", "<", xmlstr)
		# cleanxml=re.sub("</ns\d+:", "</", cleanxml)

		outFD.write('<?xml version="1.0" encoding="UTF-8"?>\n')
		outFD.write(cleanxml)
		outFD.write('\n')


	def deleteReactionsOutside(self, compartmentName):

		cID = None

		# find compartment id
		root = self.root
		compNodes = root.findall("*/{%s}listOfCompartments/{%s}compartment" % (self.URI, self.URI))

		for comp in compNodes:
			if comp.get('name') == compartmentName:
				cID = comp.get('id')

		# get set of metabolites in peroxisome
		speciesNodes = root.findall("*/{%s}listOfSpecies/{%s}species" % (self.URI, self.URI))

		inCompartment = set()
		for s in speciesNodes:
			if s.get('compartment') == cID:
				inCompartment.add(s.get('id'))

		# mark reactions without a foot in compartment
		if not self.toRemove:
			self.toRemove = []

		for r in self.reacNodes:
			speciesInReaction = set()
			speciesInReaction.update([sr.get('species') for sr in r.findall("{%s}listOfReactants/{%s}speciesReference" % (self.URI, self.URI))])
			speciesInReaction.update([sr.get('species') for sr in r.findall("{%s}listOfProducts/{%s}speciesReference" % (self.URI, self.URI))])
			speciesInReaction.update([sr.get('species') for sr in r.findall("{%s}listOfModifiers/{%s}modifierSpeciesReference" % (self.URI, self.URI))])

			inReactionAndCompartment = inCompartment.intersection(speciesInReaction)
			if len(inReactionAndCompartment) == 0:
				self.toRemove.append(r)

		# do the cleaning
		for reaction in self.toRemove:
			parent = self.parentMap[reaction]
			parent.remove(reaction)
			self.reacNodes.remove(reaction)

	def deleteUnusedSpecies(self):

		# remember the useful ones
		speciesInReactions = set()

		for r in self.reacNodes:
			speciesInReactions.update([sr.get('species') for sr in r.findall("{%s}listOfReactants/{%s}speciesReference" % (self.URI, self.URI))])
			speciesInReactions.update([sr.get('species') for sr in r.findall("{%s}listOfProducts/{%s}speciesReference" % (self.URI, self.URI))])
			speciesInReactions.update([sr.get('species') for sr in r.findall("{%s}listOfModifiers/{%s}modifierSpeciesReference" % (self.URI, self.URI))])

		log("species in reactions:")
		log(len(speciesInReactions))
		# remember which ones we need to delete
		toDelete = []

		speciesNodes = self.root.findall("*/{%s}listOfSpecies/{%s}species" % (self.URI, self.URI))
		log('species nodes:')
		log(len(speciesNodes))
		toDelete = [s for s in speciesNodes if s.get('id') not in speciesInReactions]

		# delete them!
		for s in toDelete:
			parent = self.parentMap[s]
			parent.remove(s)

		# done!

	def getGeneAssociations(self, reset=False):

		if not reset and hasattr(self, 'r2formula'):
			return self.r2loci, self.r2formula

		assert self.root is not None and self.URI is not None

		r2loci = dict()
		r2formula = dict()
		r2formulaNode = dict()
		rid2node = dict()

		# look for reactions

		for r in self.reacNodes:
			reacId = r.get('id', 'INVALID')
			rid2node[reacId] = r
			notes = r.find("{%s}notes" % (self.URI))
			if notes is None:
				continue
			geneFormula = None

			body = notes.find("{http://www.w3.org/1999/xhtml}body")
			if body is not None:
				notes = body


			for line in notes:
				text = line.text
				if text is None: continue
				if text.startswith("GENE ASSOCIATION:") or text.startswith("GENE_ASSOCIATION:"):
					geneFormula = text[17:].strip()
					geneFormula = geneFormula.replace(" or ()","") # PATCH!
					geneFormula = geneFormula.replace("() or ","") # PATCH!
					geneFormula = geneFormula.strip()
					lineWithGA = line


			# skip reactions without gene association
			if not geneFormula:
				continue
			r2formula[reacId] = geneFormula
			r2formulaNode[reacId] = lineWithGA

			loci = frozenset(geneFormula.replace("(", "").replace(")", "").replace("and", "").replace("or", "").split())
			# $loci is now a set of locus for this reactions
			r2loci[reacId] = loci

		self.r2loci = r2loci
		self.r2formula = r2formula
		self.r2formulaNode = r2formulaNode
		self.rid2node = rid2node

		return r2loci, r2formula

	def getGAGroup(self, excludeLocked=False):
		"""generate a GAGroup object with the existing GeneAssociations"""
		r2loci, r2formula = self.getGeneAssociations()

		ridWithFormula = r2formula.keys()
		reactionsWithFormula = [r for r in self.reacNodes if r.get('id') in ridWithFormula]

		if excludeLocked:
			reactionsWithFormula = [r for r in reactionsWithFormula if r not in self.lockedReactions]

		GAs = GAGroup()
		for reac in reactionsWithFormula:
			ga = GeneAssociation(reac, self)
			GAs.add(ga)

		return GAs

	def DEPRECATED_rewriteGeneAssociations(self, rel):
		pass


	def rewriteGeneAssociations2(self, rel):
		GAs = self.getGAGroup(excludeLocked=True)
		allMapped = frozenset(rel.map1to1.keys()+rel.map1toN.keys())

		if not self.toRemove:
			self.toRemove = []

		for ga in GAs:
			newFormula = None
			if ga.loci and ga.loci <= allMapped:
				# we can translate all genes in ga.loci
				newFormula = ga.formula
				for l in ga.loci:
					lm1 = rel.map1to1.get(l, None)
					lm2 = rel.map1toN.get(l, None)
					translation = lm1 if lm1 else " or ".join(lm2)
					newFormula = newFormula.replace(l, translation)
				# log("HT Rewrite (%s) to (%s)" % (ga.formula,newFormula))
				logRewrite("HT", ga, newFormula)
			elif len(ga.loci) == 1:
				# there's no good match for this one, so take the best one we can find
				l = list(ga.loci)[0]
				bestTargets = rel.getTargetTally(frozenset(ga.loci))

				if not bestTargets:
					# try with any source group that includes l
					bestTargets = rel.getTargetTallyForLoci(l)

				if not bestTargets:
					# we really tried!
					# log("L1 no bestTargets for SG(1):(%s)" % (ga.formula))
					logRewrite("L1", ga)
					self.toRemove.append(ga)
					continue


				# choose the smallest of the bests non-empty targets
				# bestT: bestTarget

				highestTally = bestTargets[0][0]
				allCandidateTargets = [t for t in bestTargets if t[0] == highestTally and len(t[1]) > 0]
				allCandidateTargets.sort(reverse=True, key=lambda t: len(t[1]))
				bestT = allCandidateTargets[0]

				if not bestT:
					# log("F1 not even with BestQuality we could find SG(1):(%s)" % (ga.formula))
					logRewrite("F1", ga)
					continue
				else:
					# this will work fine even if there is only one elemen in bestT[1]
					translation = " or ".join(bestT[1])

					newFormula = ga.formula
					newFormula = newFormula.replace(l, translation)
					# log("B1 Rewrite (%s) to (%s)" % (ga.formula,newFormula))
					logRewrite("B1", ga, newFormula)


			else:
				# not rewritable by 1:1, or by known source group
				# try to solve easy cases first (AND/OR closure)
				nORs = ga.formula.count('or')
				nANDs = ga.formula.count('and')

				if bool(nORs) != bool(nANDs):	# (AND/OR closure)
					separator = " and " if nANDs else " or "

					if rel.inSourceGroups(ga.loci):		# easy case: sourceGroup exists
						targetGroup = rel.getBestTargetGroup(ga.loci)
						newFormula = separator.join(targetGroup)
						# log("HC Rewrite AND/OR (%s) to (%s)" % (ga.formula,newFormula))
						logRewrite("HC", ga, newFormula)
					else:
						# try to match using High and Best quality
						toTranslate = list(ga.loci)
						selectedTargets = []

						# check 1:1 HQ
						for loci in ga.loci:
							tran = rel.map1to1.get(loci, None)
							if tran:
								toTranslate.remove(loci)
								selectedTargets.append(tran)

						# get best candidates for the rest

						candidates = rel.getSourcesIncluding(toTranslate)

						# candidates is a list of tallySourceGroups: [(v, sourcegroup)]
						# sorted by votes

						# log(">> already mapped <%s>" % (str(selectedTargets)))
						# log(">> About to translate %s using table <%s>" % (str(toTranslate), str(candidates)))

						while len(toTranslate) > 0 and len(candidates) > 0:
							sourceGroup = candidates.pop(0)
							# if there is a target group, use it!
							targets = rel.getTargetTally(sourceGroup)
							if targets:
								target = targets[0]
								for s in sourceGroup[1]:
									if s in toTranslate:
										toTranslate.remove(s)

								combinedTarget = "("+ " or ".join([t for t in target[1]]) + ")"

								selectedTargets.append(combinedTarget)



						# log(">> still to translate: <%s>" % (str(toTranslate)))
						# if len(toTranslate)==0:
						if len(toTranslate) == 0 or (separator == " or " and len(selectedTargets) > 0):
							newFormula = separator.join(selectedTargets)
							# log("BC Rewrite AND/OR (%s) to (%s)" % (ga.formula,newFormula))
							logRewrite("BC", ga, newFormula)
						else:
							# log("LC No match/translation for source group: %s" % (ga.formula))
							logRewrite("LC", ga)
							self.toRemove.append(ga)

				else:
					# try to solve unsolved cases, using BestQuality map (the best we can find)


					# log("TU Unsolved case: %s" % (ga.formula))
					logRewrite("TU", ga)
					# we shouldn't, but we're going to delete them for now
					self.toRemove.append(ga)

			# rewrite the xml model
			if newFormula:

				# check for genes with pipes
				NOTPIPE = "thisisapipe"
				cleanFormula = newFormula.replace('|', NOTPIPE)

				# normalize (to CNF) and clean formula
				cleanFormula = logic.to_cnf(cleanFormula)
				cleanFormula = logic.clean_formula(cleanFormula)
				cleanFormula = logic.raise_same_op(cleanFormula)
				cleanFormula = logic.clean_formula(cleanFormula)
				cleanFormula = cleanFormula.nicerepr()

				# recover pipes
				cleanFormula = cleanFormula.replace(NOTPIPE, '|')

				log("CF\t%s\t%s" % (newFormula, cleanFormula))

				# rewrite xml
				reacId = ga.rid
				line = self.r2formulaNode[reacId]
				line.text = "GENE_ASSOCIATION: %s" % (cleanFormula)

			# next GA!

	# end of rewriting


	def forceGeneAssociations(self, forcedReactions):

		# make sure we pre-processed gene associations
		_, _ = self.getGeneAssociations()
		r2formulaNode = self.r2formulaNode

		for rid, newGA in forcedReactions.iteritems():
			log("in model.force: forcing %s to %s" % (rid, newGA))
			# TODO: this shouldn't be an assert
			assert rid in self.rid2node
			line = r2formulaNode[rid]
			line.text = "GENE_ASSOCIATION: %s" % (newGA)
			# mark this reaction as locked
			self.lockedReactions.append(rid)
			self.lockedReactions.append(self.rid2node[rid])


	########################################################

#			elif rel.inSourceGroups(ga.loci):
#				# let's try with ORgroups and ANDgroups
#				targetGroup=rel.getBestTargetGroup(ga.loci)
#				nORs=ga.formula.count('or')
#				nANDs=ga.formula.count('and')
#
#				if bool(nORs)!=bool(nANDs):		# logical xor
#					separator=" and " if nANDs else " or "
#					newFormula=separator.join(targetGroup)
#					log("HL Rewrite AND/OR (%s) to (%s)" % (ga.formula,newFormula))
#				else:
#					# complex case, so only log for now
#					log("TU Unsolved case: %s" % (ga.formula))
#			else:
#				# we shoulnd't reach this point
#				log("F3 No match/translation for source group: %s" % (ga.formula))


	########################################################

	def removeReactions(self, listToRemove=None):
		"""remove reactions marked as such"""

		assert not listToRemove, "Explicit list to remove: not implemented (using only reactions tagged for removal)"

		#assert self.toRemove, "Calling removeReactions before marking them"
		if not self.toRemove or len(self.toRemove) == 0:
			return

		# TODO: reference count of metabolites to remove unused entries

		for ga in self.toRemove:
			reaction = ga.reactionNode
			if reaction in self.lockedReactions:
				continue
			parent = self.parentMap[reaction]
			parent.remove(reaction)

		# reset
		self.toRemove = []


class Relations(object):

	rels = None
	name = None

	def __init__(self, filename=None):
		if filename:
			self.parseRel(filename)

	def parseRel(self, filename):
		assert filename
		self.name = os.path.basename(filename)

		rels = []
		groupMap = dict()

		for line in open(filename):
			genesOrg1, genesOrg2 = line[:-1].split('\t')

			if genesOrg1:
				genesOrg1 = frozenset(genesOrg1.split(','))
			if genesOrg2:
				genesOrg2 = frozenset(genesOrg2.split(','))

			rels.append((genesOrg1, genesOrg2))
			rels.append((genesOrg2, genesOrg1))
			groupMap[genesOrg1] = genesOrg2
			groupMap[genesOrg2] = genesOrg1

		self.rels = frozenset(rels)
		self.groupMap = groupMap

	def mappedTo(self, group):
		"""get the group this one is mapped to"""
		return self.groupMap.get(group, None)

	def __str__(self):
		elems = [str(x) for x in self.rels]
		return ";".join(elems)

class MultiRelations(object):
	"""Store and study a set of Relation mappings"""

	def __init__(self, rels=None, logFD=None):
		"""rels should be an iterable of Relations"""
		global PTlogFD
		if logFD is not None:
			PTlogFD = logFD

		self._setRels(rels)

		# init other variables
		self.votes = None
		self.map1to1 = None
		self.map1toN = None
		self.sourceToTallyGroup = None

	def debugRels(self):
		for r in self.rels:
			print r

	def _setRels(self, rels):
		self.rels = rels
		self.N = len(rels) if rels else 0

	def loadMulti(self, relFiles):
		rels = [Relations(f) for f in relFiles]
		self._setRels(rels)

	def voteForSharedGroups(self, index=0):
		assert self.rels
		relMethods = self.rels

		countGroups = defaultdict(int)
		groupToMethod = defaultdict(list)

		# to vote for methods, we are going to count the number of times each set appears
		for method in relMethods:
			for rel in method.rels:
				countGroups[rel[index]] += 1
				groupToMethod[rel[index]].append(method)

		return countGroups, groupToMethod


	def voteGroups(self):
		assert self.rels

		# already calculated
		if self.votes:
			return self.votes

		# in case of change of .rel format
		ISOURCE = 1
		ITARGET = 0

		# constructor of a defaultdict to int
		# useful to create defdict(defdict(int)), to be used as counter[A][B]+=1
		def defint():
			return defaultdict(int)

		counter = defaultdict(int)
		pairCounter = defaultdict(defint)
		mappedTo = defaultdict(set)
		votes = dict()
		sourceToTallyGroup = dict()

		# iterate over all rels, in all methods
		for method in self.rels:
			for rel in method.rels:
				sourceGroup = rel[ISOURCE]
				targetGroup = rel[ITARGET]

				# don't care about None->(targetGroup)
				if not sourceGroup:
					continue
				# neither about (sourceGroup)->None
				if not targetGroup:
					continue

				# remember mapping, count the occurrences of source and the ocurrences of source-target
				mappedTo[sourceGroup].add(targetGroup)
				counter[sourceGroup] += 1
				pairCounter[sourceGroup][targetGroup] += 1

		# vote for groups
		# (5,SG1) -> [ (3,TG2), (1,TG1), (1,TG3) ]
		for sourceGroup, sourceVotes in counter.iteritems():
			# print ">>>"+str(sourceGroup)+"<<<>>>"+str(mappedTo[sourceGroup])+"<<<"
			# print "$$$"+str(mappedTo[sourceGroup])+"$$$"
			tallySourceGroup = (sourceVotes, sourceGroup)
			votes[tallySourceGroup] = [(pairCounter[sourceGroup][targetGroup], targetGroup) for targetGroup in mappedTo[sourceGroup]]
			votes[tallySourceGroup].sort(reverse=True)

			# remember this source (makes easier to look for it afterwards)
			sourceToTallyGroup[sourceGroup] = tallySourceGroup

		self.votes = votes
		self.sourceToTallyGroup = sourceToTallyGroup
		return votes

	def inSourceGroups(self, group):
		assert self.sourceToTallyGroup
		return group in self.sourceToTallyGroup


	def getBestTargetGroup(self, group):

		targets = self.getTargetTally(group)

		if targets is None:
			return None

		return targets[0][1]


	def getTargetTally(self, group):
		assert self.sourceToTallyGroup
		assert self.votes
		assert group

		if group in self.votes:
			return self.votes[group]

		st = self.sourceToTallyGroup.get(group, None)

		if st is None:
			return None

		return self.votes[st]


	def getTargetTallyForLoci(self, loci):
		assert self.sourceToTallyGroup

		allTargets = []

		for group, tallyGroup in self.sourceToTallyGroup.iteritems():
			if loci in group:
				allTargets.extend(self.votes[tallyGroup])

		# now allTargets should have a lot of target tallyPgroups!
		allTargets.sort(reverse=True)

		if len(allTargets) == 0:
			allTargets = None
		return allTargets

	def getAllSourceGroups(self):
		assert self.sourceToTallyGroup
		return self.sourceToTallyGroup.keys()

	def getSourcesIncluding(self, geneList):
		selectedSources = []
		geneSet = frozenset(geneList)

		for sourceGroup, sourceTallyGroup in self.sourceToTallyGroup.iteritems():
			commonSet = sourceGroup & geneSet
			if len(commonSet) > 0:
				selectedSources.append(sourceTallyGroup)
		selectedSources.sort(reverse=True)
		return selectedSources


	def verifyIfMappingsAgree(self, groups, groupToMethods, maxLen=99999):

		for group in groups:

			# skip groups larger than threshold
			if len(group) > maxLen:
				continue

			# get all mappings in method
			mappedTo = set()

			for method in groupToMethods[group]:
				targetGroup = method.mappedTo(group)
				if 0 < len(targetGroup) < maxLen:
					mappedTo.add(method.mappedTo(group))

			if len(mappedTo) > 1:
				log("MapMismatch: group [%s] mapped to: [%s]" % (str(group), str(mappedTo)))

	def unravel1to1(self):

		# get as many trusted 1:1 relationship between the two organisms
		assert self.N > 0
		minVotes = (self.N/2) + 1

		# get number of votes for each scaffold group (index=1), and a mapping between
		# each of those groups to the methods that produce that map
		votes = self.voteGroups()

		# store 1:1 source to target mappings (with votes!)
		map1to1 = dict()

		# get 1to1 mappings, with sourceGroups that pass the threshold, and the most voted target first!

		def goodSingle(x):
			return x[0] >= minVotes and len(x[1]) == 1

		for source, targets in votes.iteritems():
			if goodSingle(source):
				for target in targets:
					if goodSingle(target):
						# 1:1 found!
						# map1to1[source]=target
						map1to1[list(source[1])[0]] = list(target[1])[0]
						break	# go to next source
		# remember results
		self.map1to1 = map1to1

	def unravelDuplicates(self):
		"""Find 2:1, 1:2 mappings"""

		assert self.map1to1, "unravel1to1 must be called before further unraveling"

		assert self.N > 0
		minVotes = (self.N/2) + 1

		# get number of votes for each scaffold group (index=1), and a mapping between
		# each of those groups to the methods that produce that map
		votes = self.voteGroups()

		def goodGroup(x, n):
			return x[0] >= minVotes and len(x[1]) == n

		# first find 2:1 mappings
		for source, targets in votes.iteritems():
			if goodGroup(source, 2):
				for target in targets:
					if goodGroup(target, 1):
						# 2:1 found!
						self.map1to1[list(source[1])[0]] = list(target[1])[0]
						self.map1to1[list(source[1])[1]] = list(target[1])[0]
						break	# go to next source

		# now find 1:2 mappings
		if not self.map1toN:
			self.map1toN = dict()

		for source, targets in votes.iteritems():
			if goodGroup(source, 1):
				for target in targets:
					if goodGroup(target, 2):
						# 1:2 found!
						self.map1toN[list(source[1])[0]] = list(target[1])
						break	# go to next source

		# that's all
		return


class GeneAssociation(object):
	"""Store a gene association"""
	def __init__(self, reactionNode, model):
		# super(GeneAssociation, self).__init__()
		self.reactionNode = reactionNode
		self.rid = reactionNode.get('id', 'INVALID')
		self.formula = model.r2formula.get(self.rid, None)
		self.loci = model.r2loci.get(self.rid, None)

	def getReactionName(self):
		assert self.reactionNode is not None
		return self.reactionNode.get('name', 'INVALID')

class GAGroup(set):
	"""store a set of gene associations"""
	def __init__(self):
		super(GAGroup, self).__init__()


def main():
	pass

if __name__ == '__main__':
	main()

