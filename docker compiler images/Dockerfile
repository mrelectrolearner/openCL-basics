# Use a base image with GCC 4.8 for ARM
FROM  ubuntu:16.04


VOLUME /usr/src/myapp

ARG SOCEDSINTALLER=https://download.altera.com/akdlm/software/acdsinst/18.1std/625/ib_installers/SoCEDSSetup-18.1.0.625-linux.run
ARG AOCLINTALLER=https://cdrdv2.intel.com/v1/dl/getContent/665448/665475?filename=AOCL-18.1.0.625-linux.tar

# Install the required ARM cross-compiler toolchain (GCC 4.8)
RUN apt-get update && apt-get install -y \
    wget\
    build-essential  \
    gcc-4.8 \
    g++-4.8 \
    gcc-4.8-arm-linux-gnueabihf \
    locales  \
    openjdk-8-jdk \
    libglib2.0-0 \
    g++-4.8-arm-linux-gnueabihf \
    cmake \
    opencl-headers \
    clinfo \
    ocl-icd-libopencl1 \
    && rm -rf /var/lib/apt/lists/*


# Set environment variables for the compiler
ENV CC=arm-linux-gnueabihf-gcc-4.8 
ENV CXX=arm-linux-gnueabihf-g++-4.8


# Create a directory for your C program
WORKDIR /usr/src


# Copy the installer into the container
RUN mkdir  -p /usr/src/installers/aocl 
RUN wget $AOCLINTALLER
RUN tar -xvf 665475?filename=AOCL-18.1.0.625-linux.tar -C /usr/src/installers/aocl/   \
    && rm 665475?filename=AOCL-18.1.0.625-linux.tar 

RUN wget $SOCEDSINTALLER 

RUN ls -R

RUN  mkdir /usr/src/installers/eds \
    && mv SoCEDSSetup-18.1.0.625-linux.run /usr/src/installers/eds/

# fire Copy required fields for the configuration into the container
RUN ls -R

COPY ./terasic /usr/src/programs/intelFPGA/18.1/hld/board/terasic
COPY  env_config /usr/src/env/

# Intall the OpenCL intel SDK
RUN  ./installers/aocl/setup.sh  --unattendedmodeui minimal --mode unattended  --installdir /usr/src/programs/intelFPGA/18.1 --accept_eula 1

RUN ["chmod", "+x", "/usr/src/installers/eds/SoCEDSSetup-18.1.0.625-linux.run" ]
RUN  ./installers/eds/SoCEDSSetup-18.1.0.625-linux.run  --unattendedmodeui minimal --mode unattended  --installdir /usr/src/programs/intelFPGA/18.1 --accept_eula 1 

RUN ./programs/intelFPGA/18.1/embedded/ds-5_installer/install.sh --i-agree-to-the-contained-eula --destination /usr/src/programs  --no-interactive

# Config environment variables

ENV INTELFPGAOCLSDKROOT=/usr/src/programs/intelFPGA/18.1/hld
ENV AOCL_BOARD_PACKAGE_ROOT=$INTELFPGAOCLSDKROOT/board/terasic/de10_nano
ENV QUARTUS_ROOTDIR=/usr/src/programs/intelFPGA/18.1/quartus 

SHELL ["/bin/bash", "-c"]
RUN cat ./env/env_config >> /etc/profile
SHELL ["/bin/bash", "-c"]
RUN source /etc/profile

# config OpenCL Intel FPGA SDK 
SHELL ["/bin/bash", "-c"]
RUN source $INTELFPGAOCLSDKROOT/init_opencl.sh
RUN $INTELFPGAOCLSDKROOT/bin/aoc -list-boards

RUN rm $QUARTUS_ROOTDIR/linux64/libccl_curl_drl.so \
        $QUARTUS_ROOTDIR/linux64/libcrypto.so.1.0.0 \
        $QUARTUS_ROOTDIR/linux64/libcurl.so.4 \
        $QUARTUS_ROOTDIR/linux64/libssl.so.1.0.0

CMD echo $AOCL_BOARD_PACKAGE_ROOT
