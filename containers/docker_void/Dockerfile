
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
		-G wheel \
		tpuser \
	&& \
	echo 'tpuser:transpixel' | chpasswd \
	&& \
	echo 'root:transpixel' | chpasswd 

# ---
# --- System Config:
# ---

# Configure basic terminal environment
# Versions:
## ... bash-4.4.023_1
## ... sudo-1.8.26_1
## ... man-db-2.8.4_1
## ... ncurses-6.1_2
RUN \
	xbps-install -S -y \
		bash \
		sudo \
		man-db \
		ncurses \
	&& \
	xbps-remove -Oo

# Configure software development tools
# Initial Versions:
## ... git-2.19.2_1
## ... make-4.2.1_3
## ... cmake-3.13.0_1
## ... scons-3.0.1_1
## ... vim-8.1.0451_1
RUN \
	xbps-install -S -y \
		git \
		make \
		cmake \
		scons \
		vim \
	&& \
	xbps-remove -Oo

# Configure compiler(s)
# Initial Versions:
## ... gcc-8.2.0_6
RUN \
	xbps-install -S -y \
		gcc \
	&& \
	xbps-remove -Oo

# Install development dependencies
## Initial Verions:
## ... boost-1.68.0_2
## ... libopencv-3.4.3_2
## ... eigen3.2-3.2.10_2
RUN \
	xbps-install -S -y \
		boost \
		opencv \
		eigen \
	&& \
	xbps-remove -Oo


# ---
# --- Prepare Container Startup Configuration
# ---

# for interactive terminal environment
ENTRYPOINT [ "/bin/sh", "-c" ]
CMD [ "/bin/bash" ]

