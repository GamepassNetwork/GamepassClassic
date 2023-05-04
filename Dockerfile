FROM amd64/ubuntu:18.04 AS base

EXPOSE 5315/tcp
EXPOSE 3632/tcp

ENV DEBIAN_FRONTEND=noninteractive

#Add ppa:bitcoin/bitcoin repository so we can install libdb4.8 libdb4.8++
RUN apt-get update && \
	apt-get install -y software-properties-common && \
	add-apt-repository ppa:bitcoin/bitcoin

#Install runtime dependencies
RUN apt-get update && \
	apt-get dist-upgrade -yqq && \
        apt-get install -y --no-install-recommends \
	bash net-tools libminiupnpc10 \
	libevent-2.1 libevent-pthreads-2.1 \
	libdb4.8 libdb4.8++ \
	libboost-system1.65 libboost-filesystem1.65 libboost-chrono1.65 \
	libboost-program-options1.65 libboost-thread1.65 \
	libzmq5 && \
	apt-get clean

FROM base AS build

#Install build dependencies
RUN apt-get update && \
	apt-get install -y --no-install-recommends \
	bash net-tools build-essential libtool autotools-dev automake git \
	pkg-config libssl-dev libevent-dev bsdmainutils python3 \
	libboost-system1.65-dev libboost-filesystem1.65-dev libboost-chrono1.65-dev \
	libboost-program-options1.65-dev libboost-test1.65-dev libboost-thread1.65-dev \
	libzmq3-dev libminiupnpc-dev libdb4.8-dev libdb4.8++-dev && \
	apt-get clean


#Build Gamepass from source
COPY . /home/gamepass/build/Gamepass/
WORKDIR /home/gamepass/build/Gamepass
RUN chmod +x ./autogen.sh && chmod +x share/genbuild.sh && ./autogen.sh && ./configure --disable-tests --with-gui=no && make

FROM base AS final

#Add our service account user
RUN useradd -ms /bin/bash gamepass && \
	mkdir /var/lib/gamepass && \
	chown gamepass:gamepass /var/lib/gamepass && \
	ln -s /var/lib/gamepass /home/gamepass/.gamepass && \
	chown -h gamepass:gamepass /home/gamepass/.gamepass

VOLUME /var/lib/gamepass

#Copy the compiled binaries from the build
COPY --from=build /home/gamepass/build/Gamepass/src/gamepassd /usr/local/bin/gamepassd
COPY --from=build /home/gamepass/build/Gamepass/src/gamepass-cli /usr/local/bin/gamepass-cli

WORKDIR /home/gamepass
USER gamepass

CMD /usr/local/bin/gamepassd -datadir=/var/lib/gamepass -printtoconsole -onlynet=ipv4
