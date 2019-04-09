
Pantograph
----------

Pantograph is a toolbox for the reconstruction, curation and validation of metabolic models. This is the unstable, development branch of Pantograph. For the stable 0.1 line and documentation, and for many of the scripts not included here, please check the website cited below.

Usage:

        $ pantograph -x scaffold.xml -i inparanoid.data -m orthomcl.data -s org1 -t org2 -o out.xml -l out.log

Where *scaffold.txt* is an existing, well-curated SBML metabolic model, used as a template for the reconstruction; *inparanoid.data* is the output from Inparanoid, comparing the genes from the template and the reconstructed model; *orthomcl.data* is the output from OrthoMCL, where *org1* is the template model and *org2* is the reconstructed model; *out.xml* is the SBML reconstructed model and *out.log* is the reconstruction log. You can reconstruct a model using any of the two orthology methods (Inparanoid or OrthoMCL), although better results are obtained using both.

Pantograph has been tested under Linux and OS X, but not under Windows.


For more information, see our website: http://pathtastic.gforge.inria.fr

If you use Pantograph for your own reconstructions, please cite:
	Nicolas Loira, Anna Zhukova and David J Sherman. *Pantograph: A template-based method for genome-scale metabolic model reconstruction.* J Bioinform Comput Biol, 13(2): 1550006, 2015.
	DOI: 10.1142/S0219720015500067

For any question, please contact:
Nicolas Loira
nloira@gmail.com


