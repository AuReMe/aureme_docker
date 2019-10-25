SHELL := /bin/bash 
# very important to stop make from deleting intermediate files
.SECONDARY :
include config.txt
#---------------
#Initialization of a run by copying the folder aureme_template into the shared folder as RUN_ID value for new folder name, then change the value of run_id in config.txt.
#Prompt all available database
init:
	@echo "------>RUNNING STEP : Working directory initialization: run "$(RUN_ID)
	@make -s getdb
	@cp -r /home/data/run_template/aureme_template /shared/$(RUN_ID)
	@sed -i '0,/RUN_ID=.*/{s/RUN_ID=.*/RUN_ID=$(RUN_ID)/}' /shared/$(RUN_ID)/config.txt
	@chmod -R 777 /shared/$(RUN_ID)
#---------------


#---------------
# MISC #
# Open data access from host
R:
	@chmod -R 777 .

#### get available database in Aureme  ####
#display all available database, i.e all padmet files in /home/data/database/*
#to choose a database to use in workflow, just modify in the config file the line DATA_BASE=.. by one path displayed
getdb:
	@echo "Available database in Aureme:"
	@for f in $$(find /home/data/database/ -name "*.padmet"); do echo "$${f/.padmet/}";done

#### Verify inputs validity ####
check_input: 
	@echo "------>RUNNING STEP : checking all inputs validity"
	@echo "------>RUNNING STEP : checking database"
	@if [ -e $(DATA_BASE).padmet ] ; then if ! [ -e $(DATA_BASE).sbml ]; then make -s $(DATA_BASE).sbml;fi;fi
	@if [ -e $(DATA_BASE).sbml ] ; then echo "DATABASE: OK"; else echo "No given DATABASE";fi
	@make -s check_studied_organism_input
	@make -s check_model_organism_input
	@make -s check_gap_filling_input
	@make -s R

check_studied_organism_input:
	@echo "------>RUNNING STEP : checking if FAA or GBK was given for studied organism"
	@if [ -e $(GBK_STUDY) ] ; then echo "GBK studied organism: OK";\
	if ! [ -e $(FAA_STUDY) ]; then make -s gbk_to_faa GBK_FILE=$(GBK_STUDY) OUTPUT=$(FAA_STUDY);fi; else echo "No given GBK ($(GBK_STUDY)) for studied organism";fi
	@if [ -e $(FAA_STUDY) ] ; then echo "FAA studied organism: OK"; else echo "No given FAA ($(FAA_STUDY)) for studied organism or unable to convert GBK to FAA";fi

check_model_organism_input:
	@echo "------>RUNNING STEP : checking (if exist) each folder in orthology based reconstruction"
	@for dir in $(ORTHOLOGY_MODEL_FOLDER)/*; do if [ -d "$${dir}" ] && [ $$dir != $(ORTHOFINDER_WORKDIR) ] ; \
	then echo "------>RUNNING STEP : checking model organism data validity for $$dir";\
	make -s faa_validity MODEL_FOLDER=$$dir FAA_MODEL=$$dir/$$(basename $$dir).faa METABOLIC_MODEL=$$dir/$$(basename $$dir).sbml;\
	fi; done

sbml_validity:
	@echo "------>RUNNING STEP : checking sbml validity of $(METABOLIC_MODEL)"
	@echo "#TODO: check if acces to gene assoc from notes, if not, try functions of sbml 3, convert to old, if not Error"

#check if genes ids in the model metabolic network are the same than in the faa file. If lower than the CUTOFF, raise ERROR and break the workflow
faa_validity: $(FAA_MODEL)
	@echo "------>RUNNING STEP : checking fasta model validity of $(FAA_MODEL)"
ifeq ("$(wildcard $(DICT_GENES))","")
	@python3.7 $(PADMET_U)/connection/check_orthology_input.py --model_metabolic=$(METABOLIC_MODEL) --model_faa=$(FAA_MODEL) --cutoff=$(CUTOFF) $(V)
else
	mv $(FAA_MODEL) $(FAA_MODEL)_old
	@python3.7 $(PADMET_U)/connection/check_orthology_input.py --model_metabolic=$(METABOLIC_MODEL) --model_faa=$(FAA_MODEL)_old --dict_ids_file=$(DICT_GENES) --cutoff=$(CUTOFF) --output=$(FAA_MODEL) $(V)
endif

check_gap_filling_input:
	@echo "------>RUNNING STEP : checking gap-filling inputs"
	@if [ -e $(SEEDS).txt ] ; then make -s $(SEEDS).sbml; echo "SEEDS: OK"; else echo "No given SEEDS";fi 
	@if [ -e $(TARGETS).txt ] ; then make -s $(TARGETS).sbml; echo "TARGETS: OK"; else echo "No given TARGETS";fi
ifeq ($(WITH_ARTEFACTS),TRUE)
	@if [ -e $(SEEDS).txt ]; then make -s $(SEEDS_ARTEFACTS).sbml; echo "SEEDS with init compounds: OK"; else echo "No given SEEDS and/or artefacts";fi
else
	@echo "No given artefacts, with_artefacts != TRUE"
endif



#### Manual curation  ####
#To process to the manual curation of a network, copy the template (rxn_creator.csv and/or reaction_to_add_delete.csv) from ./manual_curation/data/template to ./manual_curation/data
#fill the template(s) then use the cmd: make -s curation NETWORK=netowrk_to_update NEW_NETWORK=the_new_network (if NEW_NETWORK is none, will overwrite the network)
curation:
	@echo "------>RUNNING STEP : Updating network: $(NETWORK) to $(NEW_NETWORK)"
	@python3.7 $(PADMET_U)/management/manual_curation.py --padmetSpec=$(NETWORKS_FOLDER)/$(NETWORK).padmet  --data=$(CURATION_DATA_FOLDER)/$(DATA) --padmetRef=$(DATA_BASE).padmet --output=$(NETWORKS_FOLDER)/$(NEW_NETWORK).padmet $(V)
	@make -s R


#---------------


#---------------
# ANNOTATION-BASED RECONSTRUCTION  #
annotation_based: 
	@echo "------>RUNNING STEP : Annotation-based reconstruction, method=$(ANNOTATION_METHOD)"
	@make -s $(ANNOTATION_METHOD)
	@make -s R

#for folder in annotation_based_reconstruction/ if output not already created, then pgdb_to_padmet.py and padmet_to_sbml.py. output= (annotation_method_output)_(folder_name).sbml
pathwaytools:
ifeq ($(PWYTOOLS_INSTALLED),TRUE)
	@echo "------>RUNNING STEP :  Running pathwaytools"
	@for dir in $(ANNOTATION_FOLDER)/*; do if [ -d "$${dir}" ] && ! [ -e $(PATHWAYTOOLS_OUTPUT)_$$(basename $$dir).sbml ]; then echo "CMD PWYTOOLS PATHWAYTOOLS_RUN=$$dir/"; fi; done
endif
	@for dir in $(ANNOTATION_FOLDER)/*;\
	do if [ -d "$${dir}" ] && ! [ -e $(PATHWAYTOOLS_OUTPUT)_$$(basename $$dir).padmet ];\
	then echo "------>RUNNING STEP :  Extracting data from pathwaytools output";\
	make -s pgdb_to_padmet PGDB_DIR=$$dir/ G=-g OUTPUT=$(PATHWAYTOOLS_OUTPUT)_$$(basename $$dir).padmet SOURCE=$$(basename $$dir);fi;done
#---------------


#---------------
# ORTHOLOGY #
orthology_based:
	@echo "------>RUNNING STEP : Orthology-based reconstruction, method=$(ORTHOLOGY_METHOD)"
	@make -s $(ORTHOLOGY_METHOD)
	@make -s R

### Create FAA_study/model from GBK_study/model if FAA_study not already in genomic_data and FAA_model not in the folder orth_based_reconstruction/model ###
$(FAA_STUDY):
	@echo "------>RUNNING STEP : Creating $(FAA_STUDY) from $(GBK_STUDY)"
	@make -s gbk_to_faa GBK_FILE=$(GBK_STUDY) OUTPUT=$(FAA_STUDY)

$(FAA_MODEL):
	@echo "------>RUNNING STEP : Creating $(FAA_MODEL) from $(GBK_MODEL)"
	@make -s gbk_to_faa GBK_FILE=$(GBK_MODEL) OUTPUT=$(FAA_MODEL)

#### Orthofinder ####
#Orthofinder take in input a folder of faa files. or a previous run folder and an other folder of new models to add.
orthofinder:
	@if ! [ -e $(orthofinder_output) ];\
	then echo "------>RUNNING STEP : Running orthofinder ";\
	mkdir -p $(ORTHOFINDER_WORKDIR);\
	echo "Copy faa of studied organism";\
	cp $(FAA_STUDY) $(ORTHOFINDER_WORKDIR);\
	for dir in $(ORTHOLOGY_MODEL_FOLDER)/*;\
	do if [ -d "$${dir}" ] && [ $$dir != $(ORTHOFINDER_WORKDIR) ];\
	then echo "Copy faa of $$dir";\
	cp $$dir/$$(basename $$dir).faa $(ORTHOFINDER_WORKDIR);fi;done;\
	orthofinder -f $(ORTHOFINDER_WORKDIR);\
	cp -r $(ORTHOFINDER_WORKDIR)/OrthoFinder/*/Orthologues $(ORTHOFINDER_WORKDIR);fi
	@make extract_orthofinder
	@make -s R

extract_orthofinder:
	@echo "------>RUNNING STEP : Creating models from $(orthofinder_output)"
	@python3.7 $(PADMET_U)/connection/extract_orthofinder.py --sbml=$(BASE) --workflow=$(WORKFLOW) --orthologues=$(orthofinder_output) --study_id=$(RUN_ID) --output=$(ORTHOLOGY_OUTPUT_FOLDER)/orthofinder $(V)


#---------------


#---------------
# Draft network generation #
#### merge 1 - n metabolic network in 1 draft in padmet format####
draft: $(DRAFT).padmet
$(DRAFT).padmet:
	@echo "------>RUNNING STEP : merging metabolic network into one "
	@make -s annotation_based
	@make -s orthology_based
	@python3.7 $(PADMET_U)/connection/padmet_to_padmet.py --padmetRef=$(DATA_BASE).padmet --to_add=$(ANNOTATION_OUTPUT_FOLDER)/pathwaytools --output=$(DRAFT).padmet $(V);\
	python3.7 $(PADMET_U)/connection/sbml_to_padmet.py --padmetRef=$(DATA_BASE).padmet --sbml=$(ORTHOLOGY_OUTPUT_FOLDER)/orthofinder --padmetSpec=$(DRAFT).padmet $(V) --source_tool=orthofinder --source_category=orthology;\
	python3.7 $(PADMET_U)/connection/sbml_to_padmet.py --padmetRef=$(DATA_BASE).padmet --sbml=$(EXTERNAL_FOLDER) --padmetSpec=$(DRAFT).padmet $(V) $(ALL_RXN) --source_category=manual;
	@make -s R
#----------

#------------
# Growth medium #
get_medium:
	@python3.7 $(PADMET_U)/management/padmet_medium.py --padmetSpec=$(NETWORKS_FOLDER)/$(NETWORK).padmet

set_medium:
	@if [ -e $(SEEDS).txt ] ; then echo "------>RUNNING STEP : Adding import reactions for growth medium";\
	echo "Updating $(NETWORK) to $(NEW_NETWORK)";\
	python3.7 $(PADMET_U)/management/padmet_medium.py --padmetSpec=$(NETWORKS_FOLDER)/$(NETWORK).padmet --padmetRef=$(DATA_BASE).padmet --seeds=$(SEEDS).txt --output=$(NETWORKS_FOLDER)/$(NEW_NETWORK).padmet $(V);\
	else echo "Unable to add imports reactions, no seeds given";fi 
	#make -s padmet_to_sbml NETWORK=$(NEW_NETWORK); make -s R; else echo "Unable to add imports reactions, no seeds given";fi 

del_medium:
	@echo "Updating $(NETWORK) to $(NEW_NETWORK)";\
	python3.7 $(PADMET_U)/management/padmet_medium.py -r --padmetSpec=$(NETWORKS_FOLDER)/$(NETWORK).padmet --output=$(NETWORKS_FOLDER)/$(NEW_NETWORK).padmet $(V);\
	make -s R

#---------------


#---------------
# GAP-FILLING  #
# solution of the method & gap-filled network#
gap_filling: gap_filling_solution
	@python3.7 $(PADMET_U)/management/manual_curation.py --padmetSpec=$(NETWORKS_FOLDER)/$(NETWORK).padmet --padmetRef=$(DATA_BASE).padmet --output=$(NETWORKS_FOLDER)/$(NEW_NETWORK).padmet --data=$($(GAP_FILLING_METHOD)_solution) $(V) --category="GAP-FILLING" --tool=$(GAP_FILLING_METHOD)
	@make -s R
# Only the solution of the method #
gap_filling_solution: $(GAP_FILLING_METHOD)

#Create sbml version of seeds, seeds_artefacts, targets and data_base
$(SEEDS).sbml:
	@make -s compounds_to_sbml CPD=$(SEEDS)

$(SEEDS_ARTEFACTS).sbml:
	@if [ -e $(SEEDS).txt ]; then sort $(SEEDS).txt $(ARTEFACTS).txt | uniq > $(SEEDS_ARTEFACTS).txt; make -s compounds_to_sbml CPD=$(SEEDS_ARTEFACTS);fi

$(TARGETS).sbml:
	@make -s compounds_to_sbml CPD=$(TARGETS)

$(NETWORKS_FOLDER)/$(NETWORK).sbml:
	@make -s padmet_to_sbml NETWORK=$(NETWORK)

$(DATA_BASE).sbml:
	@python3.7 $(PADMET_U)/connection/sbmlGenerator.py --padmet=$(DATA_BASE).padmet --output=$(DATA_BASE).sbml --mnx_chem_prop=$(MNX_CPD_PROP) --mnx_chem_xref=$(MNX_CPD) $(V) --sbml_lvl=$(LVL)

#### Meneco ####
meneco: $(meneco_solution) 
$(meneco_solution): $(meneco_original_output)
	@echo "------>RUNNING STEP : creating file $(meneco_solution)"
	@python3.7 $(PADMET_U)/connection/enhanced_meneco_output.py $(V) --meneco_output=$(meneco_original_output) --padmetRef=$(DATA_BASE).padmet --output=$(meneco_solution)
	@make -s R

$(meneco_original_output): $(NETWORKS_FOLDER)/$(NETWORK).sbml $(TARGETS).sbml $(SEEDS).sbml
	@echo "------>RUNNING STEP : creating file $(meneco_original_output)"
ifeq ($(WITH_ARTEFACTS),TRUE)
	@$(eval MENECO_SEEDS=$(SEEDS_ARTEFACTS))
	@make -s $(SEEDS_ARTEFACTS).sbml 
else
	@$(eval MENECO_SEEDS=$(SEEDS))
endif
	@python3.7 /usr/local/bin/meneco.py -d $(NETWORKS_FOLDER)/$(NETWORK).sbml -r $(DATA_BASE).sbml -s $(MENECO_SEEDS).sbml -t $(TARGETS).sbml > $(meneco_original_output)
	@rm -f asp_py*
	@make -s R
#---------------

#---------------


#---------------
# CONVERT RECIPES #
# GBK to FAA #
gbk_to_faa:
	@echo "------>RUNNING STEP : gbk to faa"
	@python3.7 $(PADMET_U)/connection/gbk_to_faa.py --gbk=$(GBK_FILE) --output=$(OUTPUT) $(V)

# PGDB to padmet  #
pgdb_to_padmet:
	@echo "------>RUNNING STEP : pgdb to padmet"
	@python3.7 $(PADMET_U)/connection/pgdb_to_padmet.py --output=$(OUTPUT) --pgdb=$(PGDB_DIR) --padmetRef=$(DATA_BASE).padmet --source=$(SOURCE) $(V) --extract-gene $(NO_ORPHAN)

# padmet to sbml #
padmet_to_sbml:
	@echo "------>RUNNING STEP : creating file $(NETWORK).sbml"
	python3.7 $(PADMET_U)/connection/sbmlGenerator.py --padmet=$(NETWORKS_FOLDER)/$(NETWORK).padmet --output=$(NETWORKS_FOLDER)/$(NETWORK).sbml --mnx_chem_prop=$(MNX_CPD_PROP) --mnx_chem_xref=$(MNX_CPD) $(V) --sbml_lvl=$(LVL)
	@make -s R

# Compounds txt to sbml #
compounds_to_sbml:
	@echo "------>RUNNING STEP : creating file $(CPD).sbml"
	@python3.7 $(PADMET_U)/connection/sbmlGenerator.py $(V) --compound=$(CPD).txt --output=$(CPD).sbml

# Mapping sbml #
which_db:
	@FILE=$$(find $(NETWORKS_FOLDER)/ -name $(SBML));\
	if [ $$FILE ]; then echo "------>RUNNING STEP : Check database of $(SBML))";\
	python3.7 $(PADMET_U)/exploration/convert_sbml_db.py --sbml=$$FILE --mnx_folder=$(MNX_FOLDER) --to-map=$(TO_MAP) $(V);\
	make -s R;\
	else echo "$(SBML) does not exist";fi
	
sbml_mapping:
	@FILE=$$(find $(NETWORKS_FOLDER)/ -name $(SBML));\
	if [ $$FILE ]; then echo "------>RUNNING STEP : creating mapping file $$(sed "s/.sbml/_dict.csv/" <<< $(SBML)) for $(SBML)";\
	python3.7 $(PADMET_U)/exploration/convert_sbml_db.py $(V) --sbml=$$FILE --mnx_folder=$(MNX_FOLDER) --db_out=$(DB) --to-map=$(TO_MAP) --output=$$(sed "s/.sbml/_dict.csv/" <<< $$FILE);\
	make -s R;\
	 else echo "$(SBML) does not exist";fi

#---------------

#---------------
# COMPARTMENT #
get_compart:
	@python3.7 $(PADMET_U)/management/padmet_compart.py --padmet=$(NETWORKS_FOLDER)/$(NETWORK).padmet

del_compart:
	@python3.7 $(PADMET_U)/management/padmet_compart.py --padmet=$(NETWORKS_FOLDER)/$(NETWORK).padmet --remove=$(COMPART) --output=$(NETWORKS_FOLDER)/$(NEW_NETWORK).padmet $(V)
	@make -s R

change_compart:
	@python3.7 $(PADMET_U)/management/padmet_compart.py --padmet=$(NETWORKS_FOLDER)/$(NETWORK).padmet --old=$(OLD) --new=$(NEW) --output=$(NETWORKS_FOLDER)/$(NEW_NETWORK).padmet $(V)
	@make -s R


#---------------
# ANALYSIS #
#### Report ####
report:
	@echo "------>RUNNING STEP : creating reports for $(NETWORK)"
	@python3.7 $(PADMET_U)/exploration/report_network.py --padmetSpec=$(NETWORKS_FOLDER)/$(NETWORK).padmet --padmetRef=$(DATA_BASE).padmet --output_dir=$(REPORT_DIR)/$(NETWORK)/
	@make -s R

set_fba:
	@echo "------>RUNNING STEP : Setting reaction to test as $(ID) for $(NETWORK)"
	@python3.7 $(PADMET_U)/connection/sbmlGenerator.py --padmet=$(NETWORKS_FOLDER)/$(NETWORK).padmet --output=$(NETWORKS_FOLDER)/$(NETWORK).sbml --mnx_chem_prop=$(MNX_CPD_PROP) --mnx_chem_xref=$(MNX_CPD) $(V) --obj_fct=$(ID) --sbml_lvl=$(LVL)
	@make -s R

summary:
	@echo "------>RUNNING STEP : summary for $(NETWORK)"
	@if ! [ -e $(NETWORKS_FOLDER)/$(NETWORK).sbml ]; then make -s padmet_to_sbml $(NETWORK);fi
ifeq ($(WITH_ARTEFACTS),TRUE)
	@$(eval MENECO_SEEDS=$(SEEDS_ARTEFACTS))
	@make -s $(SEEDS_ARTEFACTS).sbml 
else
	@$(eval MENECO_SEEDS=$(SEEDS))
endif
	@python3.7 $(PADMET_U)/exploration/flux_analysis.py --sbml=$(NETWORKS_FOLDER)/$(NETWORK).sbml --seeds=$(MENECO_SEEDS).sbml --targets=$(TARGETS).sbml 2>analysis/flux_analysis/$(NETWORK)_log.txt 1>analysis/flux_analysis/$(NETWORK).txt
	@make -s R

menecheck:
	@echo "------>RUNNING STEP : Topological analysis for $(NETWORK)"
	@if ! [ -e $(NETWORKS_FOLDER)/$(NETWORK).sbml ]; then make -s padmet_to_sbml $(NETWORK);fi
ifeq ($(WITH_ARTEFACTS),TRUE)
	@$(eval MENECO_SEEDS=$(SEEDS_ARTEFACTS))
	@make -s $(SEEDS_ARTEFACTS).sbml 
else
	@$(eval MENECO_SEEDS=$(SEEDS))
endif
	@menecheck.py -d $(NETWORKS_FOLDER)/$(NETWORK).sbml -s $(MENECO_SEEDS).sbml -t $(TARGETS).sbml > analysis/topological_analysis/$(NETWORK).txt
	@make -s R

#### Wiki ####
#To use wiki you must first build the wiki_docker image (just once ! check with docker images if the image is already built
wiki_pages:
	@echo "------>RUNNING STEP : creating wiki pages for $(NETWORK)"
	@if [ -e $(DATA_BASE).padmet ];then \
	python3.7 $(PADMET_U)/connection/wikiGenerator.py $(V) --padmetRef=$(DATA_BASE).padmet --padmet=$(NETWORKS_FOLDER)/$(NETWORK).padmet --output=$(WIKI_PAGES)/$(NETWORK) --wiki_id=$(ID) --log_file=$(BASE)/log.txt; else python3.7 $(PADMET_U)/connection/wikiGenerator.py $(V) --padmet=$(NETWORKS_FOLDER)/$(NETWORK).padmet --output=$(WIKI_PAGES)/$(NETWORK) --wiki_id=$(ID) --log_file=$(BASE)/log.txt;fi
	@make -s R


#### Askomics ####
#TSV files for askomics
tsv:
	@echo "------>RUNNING STEP : creating TSV files for $(NETWORK)"
	@python3.7 $(PADMET_U)/connection/padmet_to_tsv.py $(V) --padmetRef=$(DATA_BASE).padmet --padmetSpec=$(NETWORKS_FOLDER)/$(NETWORK).padmet --output_dir=$(ASKOMICS)/$(NETWORK)
	@make -s R

#---------------
#### Ways to delete generated files ####
clean:
	@rm log.txt; echo "### LOG ###" >> log.txt
	@rm full_log.txt; echo "### FULL LOG ###" >> full_log.txt
	@make -s R
