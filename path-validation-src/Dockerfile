FROM ubuntu:16.04

RUN apt update
RUN apt install -y cmake build-essential libssl-dev

COPY . /usr/src/path-validation
WORKDIR /usr/src/path-validation
RUN mkdir build
WORKDIR /usr/src/path-validation/build
RUN mv ../run.sh .
RUN cmake ../ && make

ENTRYPOINT [ "bash", "run.sh"]
CMD ["-p", "1"]
