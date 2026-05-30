FROM debian:latest

ENV DEBIAN_FRONTEND=noninteractive
ENV PICO_SDK_PATH=/pico/pico-sdk
ENV FATFS_PATH=/pico/fatfs/source
ARG USERNAME

USER root

RUN groupadd --gid 1000 ${USERNAME} || true
RUN useradd -s /bin/bash -m -u 1000 -g 1000 ${USERNAME} || usermod -s /bin/bash -l ${USERNAME} $(id -u -n 1000)
RUN mkdir /etc/sudoers.d/
RUN echo "${USERNAME} ALL=(ALL) NOPASSWD: ALL" > /etc/sudoers.d/${USERNAME}
RUN chmod 0440 /etc/sudoers.d/${USERNAME}

WORKDIR /tmp

RUN apt-get update
RUN apt-get install -y sudo minicom curl wget git gdb-multiarch cmake make pkgconf autoconf libreadline-dev tk-dev \
    build-essential libtool automake libftdi-dev libusb-1.0-0-dev gcc texinfo checkinstall \
    libssl-dev libsqlite3-dev libgdbm-dev libc6-dev libbz2-dev libffi-dev zlib1g-dev udev \
    gcc-arm-none-eabi libnewlib-arm-none-eabi libstdc++-arm-none-eabi-newlib libjim-dev \
    python3 python3-minimal python3-pkg-resources python3-pygments python3-yaml
RUN apt-get -y upgrade
RUN apt-get -y clean
RUN apt-get -y autoremove


WORKDIR /pico

RUN git clone https://github.com/abbrev/fatfs

RUN git clone --depth 1 --branch 2.2.0 https://github.com/raspberrypi/pico-sdk.git pico-sdk \
    && cd pico-sdk \
    && git submodule update --init

RUN git clone https://github.com/raspberrypi/picotool.git \
    && cd picotool \
    && mkdir build \
    && cd build \
    && cmake -DPICO_SDK_PATH=$PICO_SDK_PATH ..
    && make -j4 \
    && make install

WORKDIR /

USER ${USERNAME}
