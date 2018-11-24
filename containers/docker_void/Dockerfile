
FROM voidlinux/voidlinux:latest

# Change to this (otherwise trivial) env-var value will
# force docker to reset its cache (and rebuild image from scratch)
ENV DOCKER_CACHE 2018.11.19

# ---
# --- User Config (for ssh access)
# ---

# Configure development users
RUN \
	groupadd --gid 2001 transpixel \
	&& \
	useradd \
		--uid 2001 \
		--gid 2001 \
		--create-home \
		-s /bin/bash \
		tpuser \
	&& \
	echo 'tpuser:transpixel' | chpasswd \
	&& \
	echo 'root:transpixel' | chpasswd 

# ---
# --- System Config:
# ---

RUN \
	xbps-install -S -y bash \
	&& \
	xbps-remove -Oo

# Install s/w in one go to minimize container size
#RUN \
#	apt-get -qq update \
#	&& \
#	apt-get install -y \
#		openssh-server \
#		git \
#		scons \
#		vim \
#		libboost-graph-dev \
#		libopencv-dev \
#		libeigen3-dev \
#		libboost-filesystem-dev \
#	&& \
#	apt-get clean

ENTRYPOINT [ "/bin/sh", "-c" ]
CMD bash

