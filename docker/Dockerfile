FROM shfiorenza/dotfinder_base:latest
WORKDIR /build
RUN git clone --recursive --single-branch --branch master https://github.com/shfiorenza/dotFinder.git &&\
    cd dotFinder &&\
    ./install.sh -I
RUN rm -rf /build
WORKDIR /mnt
ENTRYPOINT []

