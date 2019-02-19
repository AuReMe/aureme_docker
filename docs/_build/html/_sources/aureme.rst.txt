AuReMe documentation


How to use the AuReMe workspace (default workflow)
==================================================

Requirements
------------

1. Create your Docker container as explained in the previous step
   **“Running a Docker container”**, start the container and go inside.

Start a new study
'''''''''''''''''

2. Use the following command to start a new study. Choose an identifier
   for this study (ex: replace ***test*** by your organism name). In
   order to illustrate this documentation, we will use ***test*** as a
   run identifier.

..

    Now you will find on your own computer (host), in your **bridge**
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
'''''''''''''''''''''''''''''

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
''''''''''''''''

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
'''''''''''''''''''

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
''''''''''''''''''''''''''''''''''''

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
''''''''''''''''''

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

Orthology-based inputs
''''''''''''''''''''''

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

Orthology-based run
'''''''''''''''''''

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
'''''''''''''''''''''

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

Annotation-based inputs
'''''''''''''''''''''''

1. Put the output of Pathway Tools (the whole PGDB directory) in the
   folder ***annotation_based_reconstruction***

2. If you have run several times Pathway Tools and want to use all of
   these annotations, just copy-paste the other PGDB folders in the
   ***annotation_based_reconstruction*** directory.

.. _run-1:

Annotation-based run
''''''''''''''''''''

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
''''''''''''''

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

Manual curation 
----------------

This step can be done several times and at any moment of the workflow.

1. Describe the manual curation(s) you want to apply by filling the
   corresponding form(s) as explained below.

Important note: It is highly recommanded to create a new form file
(.csv) each time you want to apply other changes, in order to keep
tracks of them.

Add a reaction from the database or delete a reaction in a network
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

a. Copy from the folder **manual_curationdatatemplate** the file
   **reaction_to_add_delete.csv** and paste it into the
   **manual_curationdata** directory (this way on Linux operating
   systems):

b. Fill this file (follow the exemple in the template).

Create new reaction(s) to add in a network
''''''''''''''''''''''''''''''''''''''''''

a. Copy from the folder **manual_curationdatatemplate** the file
   ***reaction_creator.csv*** and paste it into the
   **manual_curationdata** directory (this way on Linux operating
   systems):

b. Fill this file (follow the exemple in the template).

Apply changes
'''''''''''''

1. To apply the changes described in the *my_form_file.csv* form file,
   run this command:

..

    WARNING: If you don’t precise any **NEW_NETWORK** name, the current
    network will be overwritten.

    Note that all the manual curations made are stored in history files
    in the **manual_curationhistory** directory. You can use them to do
    the same corrections on other networks for example.

