FROM ubuntu:xenial

RUN apt-get update
RUN apt-get install -y software-properties-common python-software-properties
RUN add-apt-repository ppa:dmpas/e8
RUN apt-get update

RUN apt-get install ctool1cd

ENTRYPOINT /usr/bin/ctool1cd
