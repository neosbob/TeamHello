FROM ubuntu:14.04

RUN apt-get update
RUN apt-get install -y build-essential
RUN apt-get install -y libboost-all-dev
RUN apt-get install -y make
RUN apt-get install -y g++
RUN apt-get install -y php5-cli php5-mysql mysql-server

WORKDIR /opt/webserver
COPY . /opt/webserver
RUN make clean && make

CMD tar -cf - webserver
