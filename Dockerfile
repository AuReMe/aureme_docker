# Need a Pathway-tools installer in the same folder.
# Use it with the command mpwt -f folder
FROM ubuntu:16.04
MAINTAINER "Meziane AITE"
LABEL Version="1.2.3"
LABEL Description="Traceability, reproducibility and wiki-exploration for “à-la-carte” reconstructions of GEM."

# Install common dependencies.
RUN apt-get -y update && \
	apt-get install -y \
    	curl \
	wget \
    	make \
    	csh \
    	git \
	vim \
	python3 \
    	python3-dev \
    	python2.7 \
    	python2.7-dev \
    	python3-pip
RUN curl https://bootstrap.pypa.io/get-pip.py | python2.7;\
	pip3 install --upgrade pip
RUN cpan Bio::SearchIO

#Install Python packages
RUN pip3 install padmet \
	requests \
	meneco \
	MeneTools \
	matplotlib
RUN pip install biopantograph

#Creating folder hierarchy. 
RUN mkdir -p /programs/ /home/data

#clone required repository
RUN cd /programs; git clone https://gitlab.inria.fr/maite/padmet-utils.git

#Copy files
COPY run_template /home/data/run_template
COPY database /home/data/database
COPY programs /programs
COPY aureme /bin/
RUN chmod +x /bin/aureme

