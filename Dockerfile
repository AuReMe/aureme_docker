# Need a Pathway-tools installer in the same folder.
# Use it with the command mpwt -f folder
FROM ubuntu:16.04
MAINTAINER "Meziane AITE"
LABEL Version="1.2.5"
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
	unzip \
	java-common \
	default-jre \
        ncbi-blast+ \
        mcl \
        libxm4 \
	libgd2-xpm-dev \
	libxml-libxml-perl \ 
	libgd-gd2-perl \
	libgd-perl \
        r-base \
	python3 \
    	python3-dev \
    	python2.7 \
    	python2.7-dev \
    	python3-pip
RUN echo "[ncbi]\nData=/usr/bin/data" > ~/.ncbirc
RUN curl https://bootstrap.pypa.io/get-pip.py | python2.7;\
	pip3 install --upgrade pip
RUN cpan Bio::SearchIO

#Install Python packages
RUN pip3 install padmet \
	requests \
	meneco \
	MeneTools \
	matplotlib \
	eventlet
RUN pip2 install biopantograph

# Install OrthoFinder.
# Echo 'export LANG="C.UTF-8"' to resolve unicode error with Godocker.
RUN mkdir /programs/ /programs/diamond/ /shared/;\
    cd /programs;\
    wget https://github.com/davidemms/OrthoFinder/releases/download/v2.2.7/OrthoFinder-2.2.7.tar.gz;\
    tar xzf OrthoFinder-2.2.7.tar.gz;\
    rm OrthoFinder-2.2.7.tar.gz;\
    wget http://www.atgc-montpellier.fr/download/sources/fastme/fastme-2.1.5.tar.gz;\
    tar xzf fastme-2.1.5.tar.gz fastme-2.1.5/binaries/fastme-2.1.5-linux64;\
    mv fastme-2.1.5/binaries/fastme-2.1.5-linux64 /usr/local/bin/fastme;\
    rm -rf fastme-2.1.5*;\
    wget https://mmseqs.com/latest/mmseqs-static_avx2.tar.gz;\
    tar xvzf mmseqs-static_avx2.tar.gz;\
    rm mmseqs-static_avx2.tar.gz;\
    cd diamond;\
    wget https://github.com/bbuchfink/diamond/releases/download/v0.9.22/diamond-linux64.tar.gz;\
    tar xzf diamond-linux64.tar.gz;\
    echo 'export PATH="$PATH:/programs/OrthoFinder-2.2.7:"\nexport PATH="$PATH:/programs/mmseqs2/bin/:"\nexport PATH="$PATH:/programs/diamond:"\nexport LANG="C.UTF-8"' >> ~/.bashrc


#Creating folder hierarchy. 
RUN mkdir -p /programs/ /home/data

#clone required repository
RUN cd /programs; git clone https://github.com/AuReMe/padmet-utils

#Copy files
COPY run_template /home/data/run_template
COPY database /home/data/database
COPY programs /programs
COPY aureme /bin/
RUN chmod +x /bin/aureme

