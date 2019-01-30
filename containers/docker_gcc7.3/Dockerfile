
FROM gcc:7.3
# Change to this (otherwise trivial) env-var value will
# force docker to reset its cache (and rebuild image from scratch)
ENV DOCKER_CACHE 2019.01.30

# ---
# --- System Config:
# ---

# Install s/w in one go to minimize container size
RUN \
	apt-get -qq update \
	&& \
	apt-get install -y \
		openssh-server \
		git \
		scons \
		vim \
		libboost-graph-dev \
		libeigen3-dev \
		libboost-filesystem-dev \
	&& \
	apt-get clean

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
		tpuser

#		-G wheel \

# ---
# --- SSH access
# ---

RUN \
	apt-get install -y \
		openssh-server \
	&& \
	mkdir /var/run/sshd \
	&& \
	echo 'tpuser:transpixel' | chpasswd \
	&& \
	echo 'root:transpixel' | chpasswd \
	&& sed -i 's/^#*PermitRootLogin .*$/PermitRootLogin yes/' \
		/etc/ssh/sshd_config

# SSH login fix. Otherwise user is kicked off after login
RUN \
	sed 's@session\s*required\s*pam_loginuid.so@session optional pam_loginuid.so@g' -i /etc/pam.d/sshd

# - no idea... apparently some voodoo that helps with ssh
ENV NOTVISIBLE "in users profile"
RUN echo "export VISIBLE=now" >> /etc/profile

# enable ssh server as access method
# - i.e. by default, create a development container for use via ssh
EXPOSE 22
CMD ["/usr/sbin/sshd", "-D"]

