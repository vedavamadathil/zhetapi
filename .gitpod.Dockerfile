FROM gitpod/workspace-full

USER root

RUN apt-get -yq update \
	&& apt-get install -yq gcc-8 g++-8 \
	&& apt-get install -yq valgrind \
	&& apt-get install -yq libboost-all-dev \
	&& apt-get install -yq asciidoctor \
	&& apt-get install -yq libcurl4-gnutls-dev \
	&& apt-get install -yq doxygen \
	&& apt-get install -yq texlive-latex-base \
			texlive-fonts-recommended \
			texlive-fonts-extra \
			texlive-latex-extra \
	&& apt-get install -eq graphvi
	&& apt-get clean \
	&& rm -rf /var/lib/apt/lists/*
