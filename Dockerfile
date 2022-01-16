FROM ubuntu:16.04

SHELL ["/bin/bash", "-c"]
RUN sed -i ~/.profile -e 's/mesg n || true/tty -s \&\& mesg n/g'
RUN apt update -y && apt upgrade -y \
    && apt install -y libssl-dev curl 
RUN apt-get update && apt-get install build-essential -y && apt-get install pkg-config -y
RUN apt-get install cmake git libgmp3-dev -y
RUN apt-get install libprocps-dev -y && apt-get install python-markdown -y && apt-get install libboost-all-dev -y 
RUN apt install vim -y



ENV HOME /root
ENV NVM_DIR $HOME/.nvm
ENV NODE_VERSION 16.3.0
ENV NODE_PATH $NVM_DIR/versions/node/v$NODE_VERSION/lib/node_modules
ENV PATH      $NVM_DIR/versions/node/v$NODE_VERSION/bin:$PATH

RUN curl -o- https://raw.githubusercontent.com/nvm-sh/nvm/v0.38.0/install.sh | bash \
    && . $NVM_DIR/nvm.sh \
    && . $HOME/.bashrc \
    && nvm install $NODE_VERSION \
    && nvm alias default $NODE_VERSION \
    && nvm use default

COPY . /root/sabres/
WORKDIR /root/sabres/

RUN npm install
RUN cd libsnark-test && git submodule update --init --recursive && mkdir build && cd build && cmake .. && make


ENTRYPOINT [ "./entrypoint.sh"]