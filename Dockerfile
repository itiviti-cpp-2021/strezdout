FROM ubuntu:groovy

LABEL maintainer="renbou"

RUN apt update && apt install gcc-8 g++-8 cmake -y && apt install openjdk-15-jdk -y
RUN update-alternatives --install /usr/bin/c++ c++ /usr/bin/g++-8 60
RUN update-alternatives --install /usr/bin/cc cc /usr/bin/gcc-8 60

WORKDIR app
COPY app.sh strezdout.jar ./
COPY resources ./resources

ENTRYPOINT ["bash", "app.sh"]
