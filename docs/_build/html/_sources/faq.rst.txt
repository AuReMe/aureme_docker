
FAQ
===

Can I have a sample of AuReMe?
------------------------------

To get an overview of AuReMe, you can get a sample by using this
command:

You will get a folder named ‘aureme_sample’ in your bridge directory.
This folder contains all input and output files as if you had run the
entire metabolic network reconstruction workflow for the example files
about *Tisochrysis lutea (brown algae)*.

Look at the logs file to understand the different commands used in the
reconstruction process.

Note: if you do not want to pollute your log files when testing things
in your sample run, do not forget to use the **quiet** argument in your
command(s) if you wish NOT to store any log, this way:

How to convert files to different formats?
------------------------------------------

The AuReMe workspace natively provides several functions for formats
conversion, through the PADMet Python package. The available convertors
are:

-  From sbml to padmet format:

..

    This command will convert all sbml in networks folder of
    ‘\ *test*\ ’ to one padmet. If you want to convert one sbml to
    padmet format, simply put this file in networks folder of your run
    and make sure there is no other sbml file, then run the command. If
    you want to merge many sbml to one padmet, add all of them in
    networks folder then run the command. Ensure that there is no other
    files in network folder before running the command, in the case of
    sbml they could be added and in other case a reading error could
    occur.

-  From padmet to sbml format:

..

    This command will convert the padmet file *my_network.padmet* from
    networks folder of ‘\ *test*\ ’ to create a sbml file
    *my_network.sbml*. By default the sbml level is set to ‘\ *3*\ ’,
    you can change the default value in the config.txt file or with the
    argment LVL (3 or 2)

-  From txt to sbml format:

..

    This command will convert a txt file containing compounds ids to a
    sbml file */path/to/txt_file.sbml*. The txt file must contain one
    compound id by line and optionally the compartment of the id which
    by default is ‘c’. Example of file:

-  From GFF/GBK to FAA format:

**NOTE**: AuReMe integrate some scripts from padmet-utils tools. for
example, gbk_to_faa command use the script
/programs/padmet-utils/connection/gbk_to_faa.py. Not all functions are
encapsulated in AuReMe, there is a lot of scripts that could be helpful.
For more information, see https://gitlab.inria.fr/maite/padmet-utils.

How to manage growth medium?
----------------------------

In AuReMe, a compound is defined as a part of the growth medium (or
‘seeds’ for gap-filling tools) if this compound is in the compartment
‘C-BOUNDARY’.

|image7|\ The growth medium is linked to the metabolic network by two
reactions, a non-reversible reaction named
‘TransportSeed-\ *compound-id*\ ’ which transport a compound of the
growth medium from the compartment ‘C-BOUNDARY’ to the ‘e’
(extra-cellular) and a reversible reaction named
‘ExchangeSeed-\ *compound-id’* which exchange the same compound from ‘e’
to the ‘c’ (cytosol). When creating a sbml file, the compounds in the
‘C-BOUNDARY’ compartment will be set as ‘BOUNDARY-CONDITION=TRUE’ to
allow flux (see
`http://sbml.org/Documents/FAQ#What_is_this_.22boundary_condition.22_business.3F <http://sbml.org/Documents/FAQ#What_is_this_.22boundary_condition.22_business.3F>`__).

Note: Some metabolic networks manage the growth medium with a reversible
reaction which consume nothing and produce a compound in the ‘c’
compartment. We chose not to do the same for clarity and because it made
some dedicated tools for metabolic network crash.

-  Get the list of compounds corresponding to the growth medium of a
       network in padmet format:

..

    Return a list of compounds or an empty list

-  Set the growth medium of a network in padmet format:

..

    This command will remove the current growth medium if existing, then
    create the new growth medium by adding the required reactions as
    described before.

-  Delete the growth medium of a network in padmet format:

..

    This function will remove all reactions consuming/producing a
    compound in ‘C-BOUNDARY’ compartment.

WARNING: If you don’t precise any **NEW_NETWORK** name, the current
network will be overwritten.

How to manage metabolic network compartment?
--------------------------------------------

In a metabolic network a compound can occur in different compartment.
Given a reaction transporting CA\ :sup:`2+` from ‘e’ (extra-cellular
compartment) to ‘c’ (cytosol compartment), the compartments involved are
‘e’ and ‘c’. It is important to properly manage the compartments defined
in a network to ensure a correct connection of the reactions. In some
case metabolic networks can use different id to define a same
compartment like ‘C_c’, ‘C’, ‘c’ for cytosol, merging those networks
could leak to a loss of network connectivity. A reaction producing
CA\ :sup:`2+` in ‘c’ and a reaction consuming CA\ :sup:`2+` in ‘C_c’ are
actually not connected, hence the interest of the metabolic network
compartment management commands of AuReMe.

-  Get the complete list of compartment from a network in padmet format:

..

    Return a list of compartment or an empty list

-  Change the id of a compartment from a network in padmet format:

..

    This command will change the id of the compartment ‘\ *old_id*\ ’ to
    ‘\ *new_id*\ ’. This command is required if different ids are used
    to define a same compartment, example changing ‘C_c’ to ‘c’, or
    ‘C-c’ to ‘c’…

-  Delete the growth medium of a network in padmet format:

..

    This function will remove all reactions consuming/producing a
    compound in ‘\ *compart_id*\ ’ compartment.

WARNING: If you don’t precise any **NEW_NETWORK** name, the current
network will be overwritten.

How to manage the log files?
----------------------------

By default, the system registers all the executed commands as a log in
the ***bridgetestlog.txt*** file. The whole output of these commands
will also be stored in another file: the ***bridgetestfull_log.txt***
file.

If you DO NOT wish to store such logs, you can use the **quiet**
argument in your command(s). For example:

It is possible to re-run a previous command by copying the corresponding
command line in the ***bridgetestlog.txt*** file, and pasting it in the
Docker container terminal.

To be able to reproduce the whole workflow applied in a previous study,
please see the **“FAQ Ho to reproduce studies ”** section.

How to reproduce studies?
-------------------------

If you want to re-run the complete workflow of a pre-run study, built
with AuReMe:

-  first of all please create a new study (as described in the
   **“Requirements** **Define the reference database ”** section) by
   running the init command:

   (You can choose any run name you want, except pre-existing runs.
   Please, avoid other special character than ‘_’ and numbers)

   It generates a new folder named *my_run2* in the *bridge* directory.

-  Now, copy all the input data from the previous study in this new
   folder (please, follow the folder architecture described in the
   **“Data organization”** section).

-  Copy also the ***log.txt*** file in the ***bridgemy_run2***
   directory. In this log file, change every occurrence of the previous
   run name by ***my_run2***.

-  Execute this log file.

How to create a new ‘à-la-carte’ workflow?
------------------------------------------

If you want to add a new step in the workflow or add a new method, it is
possible to customize AuReMe. For that it is necessary to update the
Makefile in your run. Here is an example of how to do it.

-  Add a new method:

First, install your tool by following the documentation associated. For
the example we will add a new tool for orthology-based reconstruction
‘new_tool’ which use the same input as Pantograph (a metabolic network
in sbml format, a gbk of the reference species and the gbk of the study
species) and generate the same output (a metabolic network in sbml
format).

Secondly we will update the Makefile by adding these lines:

Basically this command says that for each folder in
orthology_based_reconstruction (variable declared in config.txt), if the
expected output is not already created, run new_tool.

Finally, to select this method in your new workflow, change in the file
config.txt the variable ORTHOLOGY_METHOD=pantograph by
ORTHOLOGY_METHOD=new_tool

-  Add a new step or function:

Just update the Makefile by adding a new step and use it with this
command

How to choose another reference database?
-----------------------------------------

It is possible to select a reference database among several. You can
display the list of all available databases by using this command:

The reference database is needed to:

-  be able to match all the identifiers of the entities of metabolic
   networks

-  gap-fill the metabolic network in the gap-filling step

To select one, replace the corresponding path in the configuration file:
***config.txt***, in the ***DATA_BASE*** variable. Or you can comment
the line if you don’t want/can’t use a database. The ***config.txt***
file is stored at the root of your ***bridge*** folder (see **“Running a
Docker container 4.”**).

What is checked in my input files?
----------------------------------

Before running any command in AuReMe, it is highlight recommended to use
the command ‘check_input’. This command checks the validity of the input
files and can also create required files. Concretely this command:

-  Checks database: If database was specified in the config.txt file
   (see the **“FAQ How to choose another reference database studies ”**
   section). If so, checks if a sbml version exist and create it on the
   other hand.

-  Checks studied organism data: Search if there is a genbank (gbk/gff)
   ‘GBK_study.gbk’ and proteome (faa) ‘FAA_study.faa’ in genomic_data
   folder. If there is only a genbank, create the proteome (command
   ‘gbk_to_faa). If there is only the proteome or any of them, just
   continue the checking process. Note that the proteome is only
   required for the orthology-based reconstruction, method: Pantograph.

-  |image8|\ Checks orthology-based reconstruction data: for each folder
   found in ‘orthology_based_reconstruction’ folder checks in each of
   them if there is proteome ‘FAA_model.faa’ and a metabolic network
   ‘metabolic_model.sbml’, if there is no proteome but a genbank file
   ‘GBK_study.faa’, create the proteome (command ‘gbk_to_faa). Finally,
   the command compares the ids of genes/proteins between the proteome
   and the metabolic network.

If cutoff… important because… dict file to create a new proteome file …

-  Checks annotation-based reconstruction data: for each folder found in
   annotation_based_reconstruction’ folder checks in each of them if
   it’s a PGBD from pathway then create (if not already done) a padmet
   file ‘output_pathwaytools_’folder_name’.padmet in
   networks/output_annotation_based_reconstruction folder.

-  Checks gap-filling data: In order to gap-fill a metabolic network,
   Pantograph required as input, a file ‘seeds.sbml’ describing the
   seeds (the compounds available for the network), another describing
   the targets (the compounds that the network have to be able to
   reach), the metabolic network to fill and the database from where to
   draw the reactions all in sbml format. It’s possible to start from
   txt files for seeds ‘seeds.txt’ and targets ‘targets.txt’, each file
   containing the ids of the compounds, one by line. The command will
   then convert them to sbml (command ‘compounds_to_sbml’).

Note that by default, AuReMe will integrate the artefacts
‘default_artefacts_metacyc_20.0.txt’ to the seeds to create a file
‘seeds_artefacts.txt’ and ‘seeds_artefacts.sbml’. For more information
about the artefacts see **“FAQ What are ‘artefacts’ ”** section

Example:

**[output] **

INSERT SCREEN FROM check_input log

What is the Makefile?
---------------------

What is the config.txt file?
----------------------------

How to regenerate a new database version?
-----------------------------------------

Voir les notes de Jeanne sur le problème de Sebastian

How to map a metabolic network on another database?
---------------------------------------------------

Metabolic networks can be products of varied databases. If you want to
merge efficiently information about metabolic networks coming from
different databases, you will need to map the metabolic network(s) to a
common database. To do so, a solution is provided be AuReMe.

Note: to use this method, the metabolic network to map needs to be in
the SBML format and stored in the ***networks*** folder.

-  | First of all, you need to know the origin database of the data. To
     recognize the database used in an SBML file, use the ***which_db***
     command:
   | Example:

   **[output] **

-  When you know the origin database of the data, you have to generate
   the mapping dictionary from this database to the new one:

   Example:

   **[output] **

   In this example, the system has found more than just one mapping for
   the *R_R00494_c* reaction and the *S_Starch_p* compound. It did not
   manage to choose between the propositions: the mapping will not be
   added to the output mapping. If you want to force the mapping, you
   have to modify the mapping file manually.

-  Once you have created a mapping dictionary file, it will be
   automatically applied across the workflow to translate the data.

How to generate reports on results?
-----------------------------------

Create reports on the *network_name* network (in the ***networks***
directory). The reports is created in the ***analysisreports***
directory.

Crée 4 fichiers bridge/test/analysis/report/network_name:

-  All_genes :

Id common name linked reactions (;)

-  All_metabolites

dbRef_id common name Produced (p), Consumed (c), Both (cp)

-  All_pathways

dbRef_id common name Number of reaction found Total number of reaction
Ratio

-  All_reactions

nbRef_id common name formula (with ID) formula (with common name) in
pathway associated genes categories

How to generate Wiki?
---------------------

Voir la formation de Méziane

|image9|

1. Create a wiki

   a. Create the wiki pages. The pages will be in
      analysis/wiki_pages/network_name

Wiki_Docker is an image that allows to automatize the creation of wiki
in containers.

-  Run the next commands from your machine and not from the AuReMe
   container.

   b. Download the wiki docker image.

   c. Run and setup a container with wiki docker. Follow the
      instructions to setup correctly the wiki.

   d. Send the pages and the configuration to the wiki

How to connect to Pathway-tools?
--------------------------------

-  Create PGDB from output of AuReMe

What are “artefacts”?
---------------------

How to process Flux Balance Analysis?
-------------------------------------

Notes Mez

To set the objective reaction, please see the following FAQ section.

How to set an objective reaction?
---------------------------------

Notes Mez

.. |image0| image:: media/image1.png
   :width: 4.10069in
   :height: 4.27986in
.. |image1| image:: media/image2.png
   :width: 3.04028in
   :height: 5.33542in
.. |image2| image:: media/image3.png
   :width: 1.35354in
   :height: 2.16535in
.. |image3| image:: media/image4.png
   :width: 1.35347in
   :height: 2.16528in
.. |image4| image:: media/image5.png
   :width: 1.35347in
   :height: 2.03774in
.. |image5| image:: media/image6.png
   :width: 1.35347in
   :height: 1.99306in
.. |image6| image:: media/image7.png
   :width: 1.35383in
   :height: 2.16535in
.. |image7| image:: media/image8.png
   :width: 7.08611in
   :height: 0.38056in
.. |image8| image:: media/image9.png
   :width: 6.65625in
   :height: 0.82014in
.. |image9| image:: media/image10.png
   :width: 1.35383in
   :height: 2.16535in
