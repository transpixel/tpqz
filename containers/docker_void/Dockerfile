
# + 214 MB
FROM voidlinux/voidlinux

# Change to this (otherwise trivial) env-var value will
# force docker to reset its cache (and rebuild image from scratch)
ENV DOCKER_CACHE 2018.05.22

# ---
# --- System Config:
# ---

# ---
# --- User Config (for ssh access)
# ---

# configure general system access
# + .003 MB
RUN \
	# allow general password login (e.g. for users via ssh)
	sed \
		's/pam_permit.so/pam_unix.so/' \
		-i /etc/pam.d/chpasswd \
	# add root access in case needed (e.g. for development experimentation)
	&& \
	echo 'root:admin' | chpasswd \
	# tweak sshd to allow root login
	&& \
	mkdir /var/run/sshd \
	&& \
		sed -i \
		's/^#*PermitRootLogin .*$/PermitRootLogin yes/' \
		/etc/ssh/sshd_config

# configure development user
# + .130 MB
RUN \
	# setup development group
	groupadd --gid 2001 tpdev \
	# setup development user
	&& \
	useradd \
		--uid 2001 \
		--gid 2001 \
		--create-home \
		-s /bin/bash \
		-G wheel \
		tpuser \
	&& \
	echo 'tpuser:transpix' | chpasswd

# general build tools (used below)
# +  49 MB
RUN \
	xbps-install -S -y \
		vim \
		wget \
		make \
	&& \
	xbps-remove -Oo

# source code compiler(s)
# + 202 MB
RUN \
	xbps-install -S -y \
		gcc \
	&& \
	xbps-remove -Oo

# boost libraries (using -graph, -filesystem)
# only header files (and maybe serialization library) are needed for BGL
# + 213 MB
RUN \
	xbps-install -S -y \
		# PIG !!
		boost-devel \
	&& \
	xbps-remove -Oo

# image processing
# +   5 MB
RUN \
	xbps-install -S -y \
		libpng-devel \
		libjpeg-turbo-devel \
	&& \
	xbps-remove -Oo

# project specific build tools
# + 176 MB
RUN \
	xbps-install -S -y \
		git \
		scons \
	&& \
	xbps-remove -Oo

# update dynamic load paths (e.g. for results from source builds above)
# + .027 MB
RUN \
	ldconfig

# openCV - trasitory - to be replaced
# + 445 MB
RUN \
	xbps-install -S -y \
		# HUGE PIG !!
		libopencv-devel \
	&& \
	xbps-remove -Oo

# numeric libraries (used by ceres)
# +  18 MB
RUN \
	xbps-install -S -y \
		libstdc++ \
		lapack \
		eigen \
	&& \
	xbps-remove -Oo

# patch - missing from libtiff install?
RUN ln -s /usr/lib/libtiff.so.5 /usr/lib/libtiff.so

# enable ssh server as access method
# and configure default behavior as ssh-accessible container
# + .004 MB
RUN ssh-keygen -A

EXPOSE 22
CMD ["/usr/sbin/sshd", "-D"]

