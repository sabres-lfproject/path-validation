FROM ubuntu:16.04

SHELL ["/bin/bash", "-c"]
RUN apt update -y && apt upgrade -y \
    && apt install -y libssl-dev curl

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

ENTRYPOINT [ "./entrypoint.sh"]