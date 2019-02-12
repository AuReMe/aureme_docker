The AuReMe Workspace

Table des matières
==================

`How to use Docker 3 <#how-to-use-docker>`__

`Running a Docker container 3 <#running-a-docker-container>`__

`Some tips about Docker 3 <#some-tips-about-docker>`__

`How to use the AuReMe workspace (default workflow)
4 <#how-to-use-the-aureme-workspace-default-workflow>`__

`Requirements 4 <#requirements>`__

`Start a new study 4 <#start-a-new-study>`__

`Define the reference database 4 <#define-the-reference-database>`__

`The default workflow 5 <#the-default-workflow>`__

`Data organization 5 <#data-organization>`__

`Bridge structure 5 <#bridge-structure>`__

`Provide input files 6 <#provide-input-files>`__

`Check input files validity 7 <#check-input-files-validity>`__

`Orthology-based reconstruction 7 <#orthology-based-reconstruction>`__

`Method: Pantograph 7 <#method-pantograph>`__

`Annotation-based reconstruction 8 <#annotation-based-reconstruction>`__

`Method: Pathway Tools 8 <#method-pathway-tools>`__

`Merge metabolic networks 9 <#merge-metabolic-networks>`__

`Gap-filling 9 <#gap-filling>`__

`Method: Meneco 9 <#method-meneco>`__

`Manual curation 10 <#manual-curation>`__

`Add a reaction from the database or delete a reaction in a network
10 <#add-a-reaction-from-the-database-or-delete-a-reaction-in-a-network>`__

`Create new reaction(s) to add in a network
11 <#create-new-reactions-to-add-in-a-network>`__

`Apply changes 11 <#apply-changes>`__

`FAQ 12 <#faq>`__

`Can I have a sample of AuReMe? 12 <#can-i-have-a-sample-of-aureme>`__

`How to convert files to different formats?
12 <#how-to-convert-files-to-different-formats>`__

`How to manage growth medium? 12 <#how-to-manage-growth-medium>`__

`How to manage metabolic network compartment?
12 <#how-to-manage-metabolic-network-compartment>`__

`How to manage the log files? 13 <#how-to-manage-the-log-files>`__

`How to reproduce studies? 13 <#how-to-reproduce-studies>`__

`How to create a new ‘à-la-carte’ workflow?
13 <#how-to-create-a-new-à-la-carte-workflow>`__

`How to choose another reference database?
14 <#how-to-choose-another-reference-database>`__

`What is checked in my input files?
14 <#what-is-checked-in-my-input-files>`__

`How to regenerate a new database version?
14 <#how-to-regenerate-a-new-database-version>`__

`How to map a metabolic network on another database?
14 <#how-to-map-a-metabolic-network-on-another-database>`__

`How to generate reports on results?
15 <#how-to-generate-reports-on-results>`__

`How to generate Wiki? 15 <#how-to-generate-wiki>`__

`How to connect to Pathway-tools?
16 <#how-to-connect-to-pathway-tools>`__

`The Docker container 16 <#_Toc527496019>`__

`What are “artefacts”? 17 <#what-are-artefacts>`__

`How to process Flux Balance Analysis?
17 <#how-to-process-flux-balance-analysis>`__

`How to set an objective reaction?
17 <#how-to-set-an-objective-reaction>`__

How to use Docker
=================

Requirements: Docker (v 1.10 at least)

To install Docker, please follow the instructions on
`docker.com <https://www.docker.com/>`__, considering your operating
system*.

\*On Mac OS: requires at least Yosemite 10.10.4

\*On Windows: requires at least Windows 8

Running a Docker container
--------------------------

1. Launch the Docker machine (see the instruction on
       `docker.com <https://www.docker.com/>`__). For example:

-  On Fedora: **sudo systemctl start docker**

-  On Mac OS and Windows: run the Docker launcher

2. Download the AuReMe Docker image

3. To verify that the image has been downloaded correctly, check it in
       the list of your local images:

4. Create a folder that will serve as a bridge to share data from/to the
       Docker container. Let us call it ***bridge*** for instance.

5. Create a Docker container from the following image with this command:

The path given for –v is the one to the shared directory on your host
machine

**This path has to end on the directory name** (without any **/** at the
end)

**The path has to be complete** (from **/users** or from **C:\\\** for
Windows users)

After the ‘:’ is the name of the mirror directory in the Docker
container. Please do not change it.

For Windows users, be careful, you have to indicate your path this way:

You can create as many container as you wish, as long as you give them
different names.

Your AuReMe container is now running and correctly installed.
Congratulations! You are now inside the container ***aureme-cont***.

Some tips **about** Docker
--------------------------

-  To exit the container, tape **exit.**

-  To list all your containers:

Remark that you can see, with this command, the state of your containers
in the STATUS column: **up** (running, you can connect to it),
**exited** (stopped, need to be started again)

-  To start or stop the container (from your host):

-  If you want to go inside a started/running container:

-  To delete a container: **docker rm *container_id* (**\ or ***name*)**

-  To delete an image: **docker rmi *image_id* (**\ or ***name*)**

This is impossible if any existing container has been created from it.
Delete all dependent containers first.

How to use the AuReMe workspace (default workflow)
==================================================

Requirements
------------

1. Create your Docker container as explained in the previous step
   **“Running a Docker container”**, start the container and go inside.

Start a new study
~~~~~~~~~~~~~~~~~

2. Use the following command to start a new study. Choose an identifier
   for this study (ex: replace ***test*** by your organism name). In
   order to illustrate this documentation, we will use ***test*** as a
   run identifier.

..

    Now you will find on your own computer (host), in your ***bridge***
    directory, a folder ***test*** with many subdirectory and files.
    This is your work directory, on which AuReMe is going to run.

    Notice that from now until the end of the process, every command
    will be stored as a log in the ***bridgetestlog.txt*** file. The
    whole output of these commands will also be stored in the
    ***bridgetestfull_log.txt*** file.

    If you wish NOT to store such logs, you can use the **quiet**
    argument in your command(s). This will redirect the output on the
    terminal

    For example:

    For further details on the log files, please see the **“FAQ How to
    manage the log files”** chapter.

3. To get an overview of AuReMe, you can get a sample by using this
   command.

Define the reference database
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

4. The final step is to define which reference database to use. The
   available databases are listed in your terminal when you create a new
   study. If needed, use this command to display them again.

..

    This reference database is needed to:

-  Be able to match all the identifiers of the entities of metabolic
       networks

-  Gap-fill the metabolic network in the gap-filling step

-  Uniforms the data in one unique database

..

    To select one, replace the corresponding path in the configuration
    file: ***config.txt***, in the ***DATA_BASE*** variable. Or comment
    the line if you don’t want/can’t use a database.

    The ***config.txt*** file is stored at the root of your ***test***
    folder.

The default workflow
--------------------

By default, the AuReMe workspace includes an automatic workflow for
metabolic network reconstruction. This workflow runs several
pre-installed tools and generates diverse output files. The process can
be either run entirely in a single command, or run step by step to
personalize it or do some intermediary analysis.

|image0|\ For instance, if you run the ***draft*** command (see **“Merge
metabolic networks”**), it will run all the previous steps automatically
as described in the following figure. This figure details the steps of
the default workflow.

Data organization
-----------------

Bridge structure
~~~~~~~~~~~~~~~~

    The ***bridge*** directory will store all your input data you will
    provide, and all the result files the workflow is going to create.

    ***analysis***: all output files of the analysis processes

    ***annotation_based_reconstruction***: if you want to use annotated
    genomes (to run the annotation-based reconstruction part of the
    workflow), put here all the output files of the annotation tool. For
    instance, with Pathway Tools, copy-paste the whole PGDB directory
    (see below “Annotation-based reconstruction” for more details).

    ***database***: if you want to use your own database put in this
    folder your database in padmet format, if you have a sbml convert
    this file to padmet (see **“FAQ How to convert files to different
    formats”**)

    ***gapfilling/original_output***: if you run the metabolic network
    reconstruction with gap-filling, will contain all the output files
    of gap-filling tools before any post-process from AuReMe.

    |image1|\ ***genomic-data***: the directory in which to put the
    genomic data on your studied organism, that is to say either a
    Genbank (.gbk) or a proteome (.faa).

    ***growth_medium***: description of the set of metabolites that is
    available to initiate the metabolism (growth medium), that is to say
    the seed compounds (.txt) (see **“FAQ How to manage growth
    medium?”**).

    ***manual_curation***: all the file to describe the manual curation
    you want to apply on your metabolic network (either adding, deleting
    or modifying reactions).

    ***networks***: all the metabolic networks used or created during
    the reconstruction process

    ***networks* *external_network***: put here all existing metabolic
    networks (.sbml) you want to use. Enables to merge them with the
    ones created thanks to other methods.

    ***networks* *output_annotation_based_reconstruction***: will
    contain the processed network from the annotation based
    reconstruction, after the pre-processing of the data from the
    ***annotation_based_reconstruction*** directory (if you filled this
    one).

    ***networks* *output_orthology_based_reconstruction***: will contain
    the processed network from the orthology based reconstruction, after
    the pre-processing of the data from the
    ***orthology_based_reconstruction*** directory (if you have run this
    part of the workflow).

    ***orthology_based_reconstruction***: if you want to use model
    organisms (to run orthology-based reconstruction part of the
    workflow), put here the proteome (.faa or .gbk) and the metabolic
    network (.sbml) of your model (see below “Orthology-based
    reconstruction” for more details).

    ***targets_compounds***: description of the set target compounds
    (.txt), that is to say metabolites whose production is supposed to
    be achieved by the metabolism of the species under study (components
    of the biomass reactions or other metabolites).

Provide input files
~~~~~~~~~~~~~~~~~~~

    First of all, you have to provide to AuReMe all the input files
    needed for the different steps you want to run in the workflow. The
    steps can be optional or run several times, at different phases of
    the process. However, you have to store the input data for each
    steps, observing the architecture described above for the
    ***bridge*** directory (see **“Data organization Bridge
    structure”**).

    Here is the list of input you have to provide to run the pre-set
    default workflow. If you want to run only part of it, please see the
    corresponding sections and the chapter **“How to create your
    ‘à-la-carte’ workflow”**.

-  **See “Orthology-based reconstruction Inputs”**

-  **See “Annotation-based reconstruction Inputs”**

-  **External source for reconstruction**

..

    If you already have one or several external metabolic networks for
    your studied species and you want to improve them, just copy-paste
    them (SBML format) in the ***networksexternal_network*** folder.

|image2|\ Check input files validity
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

1. IMPORTANT: Always check the validity of the inputs before running any
   workflow task, and after having put every input files needed for the
   steps of the workflow. This will verify the format and consistency of
   your data for a better quality result. Moreover, it will generate all
   the supplementary files needed by the workflow tools and put them
   into the corresponding folders. For more information about input
   files validity see **“FAQ What is checked in my input files?”**.

..

    For this purpose, use this command:

Orthology-based reconstruction
------------------------------

Method: Pantograph
~~~~~~~~~~~~~~~~~~

Input files:

|image3|- Required for the orthology-based reconstruction (method:
Pantograph):

- Genbank or Proteome of your studied organism (.gbk or .faa)

- Genbank or Proteome of your reference organism (.gbk or .faa)

- Metabolic network of your reference organism (.sbml)

- (option) a dictionary file if genes ids used in metabolic network are
different with gbk/faa (.txt)

Result file:

/test

\|--orthology_based_reconstruction

\| \|-- *model_a*

\| \|-- **original_output_pantograph\_\ *model_a*.sbml**

\|-- networks

\|-- orthology_based_reconstruction

\|-- pantograph

\|-- **output_pantograph\_**\ *model_a*\ **.sbml**

Inputs
^^^^^^

1. Put all the available genomic data of the studied organism in the
   folder ***genomic_data***, either a Genbank (.gbk) or a Fasta (.faa)
   file. WARNING: give them these exact names (respectively):
   GBK_study.gbk and FAA_study.faa.

2. For each reference organism you want to use, create a folder in the
   folder ***orthology_based_reconstruction***. Give it the name of your
   model organism (e.g. ***model_a***).

..

    On a Linux operating system, here is the command to create a new
    folder named ***model_a***:

3. In each folder, put:

   -  the Genbank file of your model organism, with the exact name
          GBK_model.gbk

..

    OR the proteome of your model organism, with the exact name
    FAA_model.faa

-  the metabolic network of your model organism, with the exact name
       metabolic_model.sbml

4. | The genome (or proteome) and the metabolic network of your model
     organism have to contain the same kind genes (or proteins)
     identifiers to be comparable. If not enough genes (or proteins) are
     in common between the two files, the process will stop to avoid
     poor quality data production.
   | If you want to pursue on the process, please provide a dictionary
     file between the gene (or protein) identifiers of these two files.
     Name this dictionary ***dict_genes.txt***. Here is the dictionary
     file format asked (tabulation separated values):

Run
^^^

5. Important: Remember to check the validity of the inputs before
   running any workflow task. If you want to run only the
   orthology-based reconstruction, use now this command:

6. To run only the orthology-based reconstruction, use this command:

7. IMPORTANT: Because the metabolic network from the reference organism
   could came from different databases, it’s critical to check the
   database of each network and if needed convert the network to your
   reference database selected (see **“How to use the AuReMe workspace
   (default workflow) Define the reference database”**).

..

    The previous command will check the database of the file
    output_pantograph_mode_a.sbml, if the database is different for the
    reference, use the next command to create a mapping file to metacyc
    database. For more information about sbml mapping see **“FAQ How to
    map a sbml to another database?”**.

Annotation-based reconstruction
-------------------------------

Method: Pathway Tools
~~~~~~~~~~~~~~~~~~~~~

|image4|

Input files:

- Required for the annotation-based reconstruction (method: Pathway
Tools):

The output of Pathway tools (PGDB folder)

Result file:

/test

\|-- networks

\|-- annotation_based_reconstruction

\|-- pathwaytools

\|-- **output_pathwaytools\_**\ *genome_a*\ **.padmet**

\|-- **output_pathwaytools\_**\ *genome_b*\ **.padmet**

.. _inputs-1:

Inputs
^^^^^^

1. Put the output of Pathway Tools (the whole PGDB directory) in the
   folder ***annotation_based_reconstruction***

2. If you have run several times Pathway Tools and want to use all of
   these annotations, just copy-paste the other PGDB folders in the
   ***annotation_based_reconstruction*** directory.

.. _run-1:

Run
^^^

3. Important: Remember to check the validity of the inputs before
   running any workflow task. If you want to run only the
   annotation-based reconstruction, use now this command:

4. To run only the annotation-based reconstruction, use this command.

Merge metabolic networks
------------------------

|image5|

Input files:

- metabolic networks in the ***networks*** directory

Result files:

/test

\|-- netowrks

\|-- **draft.padmet**

To merge all available networks from the ***networks*** directory into
one metabolic network, merging all data on the studied species, run this
command:

Note that you can also add external metabolic network to create the
draft (see **“Data organization Provide input files External source for
reconstruction”**).

IMPORTANT: Before merging your networks, check if not already done if
all the sbml are using the reference database. Also check the
compartment ids used in each of them, delete and change compartment if
need.

For example: if a sbml is using KEGG database but your reference
database is metacyc, you will have to map this sbml to create a mapping
file which will be used automatically in the merging process.

If a sbml contains a compartment id like ‘C_c’ and another contains ‘c’,
although they correspond to the same compartment ‘cytosol’ because of
different ids, a compound in ‘C_c’ is not the same as a compound in ‘c’,
therefore there will be a loss of connectivity in the network. see
**“FAQ How to map a sbml to another database?” and “FAQ How to manage
compartment?”**

Gap-filling
-----------

Method: Meneco
~~~~~~~~~~~~~~

|image6|\ Input files:

- Required for the gap-filling (method: Meneco):

- A metabolic network reference database (.padmet or .sbml)

(metacyc 20.5, 22.0, BIGG and ModelSeed are available by default)

- Seed and target metabolites (.txt)

- A metabolic network to fill (typically created during the previous
steps)

Result files:

/test

\|-- netowrks

\|-- *network_name*\ **.sbml**

\|-- *network_name*\ **.padmet**

\|-- gapfilling

\|-- original_output

\| \|-- **meneco_output\_** *network_name*\ **.txt**

\|-- **gapfilling_solution\_** *network_name*\ **.csv**

Input
^^^^^

1. You must have selected a reference database to fill-in the potential
   gaps in the metabolic network. If it is not done yet, please see
   **“Requirements Define the reference database ”**

2. Put the seeds file (named seeds.txt) in the ***growth_medium***
   folder. The seed compounds are the description of the set of
   metabolites that is available to initiate the metabolism (growth
   medium).

..

    Here is as example of the seed file format:

3. Set the growth medium using this command:

..

    For more details on the medium settings, see **“FAQ How to manage
    growth medium?”**.

    WARNING: If you don’t precise any **NEW_NETWORK** name, the current
    network will be overwritten.

4. Put the target file (named targets.txt) in the
   ***targets_compounds*** folder. The targets are metabolites whose
   production is supposed to be achieved by the metabolism of the
   species under study (components of the biomass reactions or other
   metabolites).

..

    Here is as example of the seed file format:

5. You will have to indicate which metabolic network you want to
   gap-fill with the Meneco software. If you want to gap-fill a network
   created in the previous steps, there is nothing to do. Otherwise, put
   the network you want to gap-fill (PADMET format) in the
   ***networks*** directory.

.. _run-2:

Run
^^^

6. (optional step) To generate the gap-filling solution run this
   command:

..

    Note: Do not forget the quotation marks.

    It will calculate the gap-filling solution on the *network_name*
    network (in the ***networks*** directory) and put it into the
    ***gapfilling*** directory as gapfilling_solution_network_name.csv

7. To generate the gap-filled network (and run step 6), run this
   command:

..

    Note: Do not forget the quotation marks.

    It will calculate the gap-filling solution (if it is not yet done)
    on the *network_name* network (in the ***networks*** directory) and
    put it into the ***gapfilling*** directory. Then it will generate
    the metabolic network (*new_network_name*), completed with the
    gap-filling solution, in the ***networks*** directory.

Note that you can first generate the solution, modify it, then generate
the gap-filled network.

    WARNING: If you don’t precise any **NEW_NETWORK** name, the current
    network will be overwritten.

**Manual curation** 
--------------------

This step can be done several times and at any moment of the workflow.

1. Describe the manual curation(s) you want to apply by filling the
   corresponding form(s) as explained below.

Important note: It is highly recommanded to create a new form file
(.csv) each time you want to apply other changes, in order to keep
tracks of them.

Add a reaction from the database or delete a reaction in a network
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

a. Copy from the folder **manual_curationdatatemplate** the file
   **reaction_to_add_delete.csv** and paste it into the
   **manual_curationdata** directory (this way on Linux operating
   systems):

b. Fill this file (follow the exemple in the template).

Create new reaction(s) to add in a network
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

a. Copy from the folder **manual_curationdatatemplate** the file
   ***reaction_creator.csv*** and paste it into the
   **manual_curationdata** directory (this way on Linux operating
   systems):

b. Fill this file (follow the exemple in the template).

Apply changes
~~~~~~~~~~~~~

1. To apply the changes described in the *my_form_file.csv* form file,
   run this command:

..

    WARNING: If you don’t precise any **NEW_NETWORK** name, the current
    network will be overwritten.

    Note that all the manual curations made are stored in history files
    in the **manual_curationhistory** directory. You can use them to do
    the same corrections on other networks for example.

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
