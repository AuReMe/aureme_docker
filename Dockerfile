FROM ubuntu:18.04
MAINTAINER "Meziane AITE"
LABEL Version="2.4"
LABEL Description="Traceability, reproducibility and wiki-exploration for “à-la-carte” reconstructions of GEM."

# To fix the issue with locale language.
ENV LANG C.UTF-8
ENV PYTHONIOENCODING=utf-8


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
	python3-pip \
	python3.7 \
    	python3.7-dev

RUN echo "[ncbi]\nData=/usr/bin/data" > ~/.ncbirc

#Install Python packages
RUN python3.7 -m pip install requests \
	meneco \
	MeneTools \
	eventlet \
	padmet==4.0

#Install exonerate
COPY exonerate-2.2.0 /bin/
RUN echo 'export PATH="$PATH:/bin/exonerate-2.2.0"' >> ~/.bashrc

# Install OrthoFinder.
RUN mkdir /programs/ /programs/diamond/ /shared/;\
    cd /programs;\
    wget https://github.com/davidemms/OrthoFinder/releases/download/2.3.3/OrthoFinder-2.3.3.tar.gz;\
    tar xzf OrthoFinder-2.3.3.tar.gz;\
    rm OrthoFinder-2.3.3.tar.gz;\
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
    rm diamond-linux64.tar.gz

RUN echo 'export PATH="$PATH:/programs/OrthoFinder-2.3.3:/programs/mmseqs2/bin/:/programs/diamond:"' >> ~/.bashrc

#Creating folder hierarchy. 
RUN mkdir -p /home/data

#clone required repository
RUN cd /programs; git clone https://github.com/AuReMe/padmet-utils

#Copy files
COPY run_template /home/data/run_template
COPY database /home/data/database
COPY aureme /bin/
RUN chmod +x /bin/aureme

